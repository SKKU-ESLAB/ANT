/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
 *  
 * [Contact]
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tcp_server_over_wfd.h>
#include <wifi_control.h>

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <chrono>

namespace cm {
TCPServerOverWfdAdapter::TCPServerOverWfdAdapter(uint32_t id, int port,
                                                 const char *wfd_dev_name){
  this->port = port;
  this->dev_id = id;
  net_dev_type = kWifiDirect;

  snprintf(dev_name, sizeof(dev_name), "TCPServer/Wfd");
  strncpy(this->wfd_dev_name, wfd_dev_name, 256);

  sent_data = 0;
  cli_sock = 0;

  memset(&saddr, 0, sizeof(saddr));
}

uint16_t TCPServerOverWfdAdapter::get_id(void) {
  return dev_id;
}

bool TCPServerOverWfdAdapter::device_on(void) {
  return true;
}

bool TCPServerOverWfdAdapter::device_off(void) {
  return true;
}

bool TCPServerOverWfdAdapter::make_connection(void) {
  char buf[512];
  int ret;

  LOG_VERB("WFD Server up");

  ret = wifi::wifi_turn_on();
  if(ret < 0) return false;

  //Add P2P Group and be Group Owner
  ret = wifi::wifi_direct_server_up(wfd_dev_name);
  if (ret < 0) return false;

  //Send WFD Device ADDR
  ret = wifi::wifi_get_p2p_device_addr(buf, 256);
  if (ret < 0) {
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }
  LOG_VERB("WFD Server added : %s", buf);
  send_ctrl_msg(buf, strlen(buf));

  //Send WFD PIN
  ret = wifi::wifi_direct_server_reset(buf, 256);
  if (ret < 0) {
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }
  LOG_VERB("WFD Server PIN: %s", buf);
  send_ctrl_msg(buf, strlen(buf));

  // Wait for the P2P Client
  if (dev_connected_wait() == false) {
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }
  LOG_VERB("Get server's IP\n"); 
  // Get server's IP address and send IP addr
  int ip_wait_it;
  for (ip_wait_it = 0; ip_wait_it < 30; ip_wait_it++) {
    ret = wifi::wifi_direct_ip_addr(buf, 256);
    if (ret == 0) {
      LOG_VERB("IP:%s", buf);
      break;
    }

    usleep(300000);
  }

  if (ret < 0) {
    LOG_ERR("IP is not set");
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }
  send_ctrl_msg(buf, strlen(buf));

  // Wait for TCP connection (buf = IP Addr String)
  serv_sock = socket(AF_INET, SOCK_STREAM, 0);

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr(buf);
  saddr.sin_port = htons((uint16_t)port);
  int reuse = 1;

  if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                 sizeof(int)) == -1) {
    LOG_ERR("Socket setup failed");
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }
  
  int err = bind(serv_sock, (struct sockaddr *)&saddr, sizeof(saddr));
  if (err < 0) {
    LOG_ERR("Socket bind failed");
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }

  err = listen(serv_sock, 5);
  if (err < 0) {
    LOG_ERR("Socket listen failed");
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }

  int caddr_len = sizeof(caddr);
  LOG_VERB("Accepting client...");
  cli_sock = accept(serv_sock, (struct sockaddr *)&caddr, (socklen_t *)&caddr_len);
  if (cli_sock < 0) {
    LOG_ERR("Accept failed %s", strerror(errno));
    wifi::wifi_direct_server_down();
    wifi::wifi_turn_off();
    return false;
  }

  wifi::wifi_dhcp_close();

  return true;
}

bool TCPServerOverWfdAdapter::dev_connected_wait() {
  std::unique_lock<std::mutex> lck(dev_wait_lock);
  LOG_VERB("Wait for WFD connected");
  std::cv_status ret;
  ret = dev_connected.wait_for(lck, std::chrono::seconds(30));

  if (ret == std::cv_status::timeout) {
    LOG_WARN("Timed out");
    return false;
  }

  return true;
}

bool TCPServerOverWfdAdapter::close_connection() {
  close(cli_sock);
  close(serv_sock);
  LOG_VERB("Socket closed");
  
  cli_sock = 0;
  serv_sock = 0;

  int ret = wifi::wifi_direct_server_down();
  if (ret < 0) return false;
  LOG_DEBUG("Wi-fi direct server down");

  sleep(2);

  ret = wifi::wifi_turn_off();
  if(ret < 0) return false;
  LOG_DEBUG("Wi-fi direct turn off");

  return true;
}

int TCPServerOverWfdAdapter::send_impl(const void *buf, size_t len) {
  int sent = 0;
  //fp = fopen("wfd.log", "a");

  if (cli_sock <= 0)
    return -1;

  while (sent < len) {
    int sent_bytes = write(cli_sock, buf, len);
    if (sent_bytes <= 0) {
      LOG_WARN("Cli sock closed");
      return -1;
    }
    //LOG_DEBUG("WFD] sent: %d\n", sent_bytes);
    sent += sent_bytes;
  }

  sent_data += sent;
  //fprintf(fp,"WFD sent data: %d\n", sent_data);

  //fclose(fp);
  return sent;
}

int TCPServerOverWfdAdapter::recv_impl(void *buf, size_t len) {
  int recved = 0;

  if (cli_sock <= 0) return -1;

  while (recved < len) {
    int recv_bytes = read(cli_sock, buf, len);
    if (recv_bytes <= 0) {
      LOG_WARN("Cli sock closed");
      return -1;
    }

    recved += recv_bytes;
    LOG_DEBUG("WFD %d] recv : %d\n", port, recv_bytes);
  }

  return recved;
}

void TCPServerOverWfdAdapter::on_control_recv(const void *buf, size_t len) {
  dev_connected.notify_all();
}

}
