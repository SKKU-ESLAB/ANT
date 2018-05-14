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

#include <rfcomm_over_bt.h>
#include <tcp_server_over_eth.h>
#include <tcp_server_over_wfd.h>
#include <thread>
#include <communicator.h>
#include <network_adapter.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace cm;

void receiving_thread() {
  void *buf = NULL;
  while (true) {
    int ret = Communicator::get_instance()->recv_data(&buf);
    printf("Recv %d> %s", ret, reinterpret_cast<char *>(buf));
    free(buf);
  }
}

int main() {
  Communicator *cm = Communicator::get_instance();
  RfcommServerOverBtAdapter ca(1234, "150e8400-e29b-41d4-a716-446655440000");
  RfcommServerOverBtAdapter na3(3333, "150e8400-1234-41d4-a716-446655440000");
  RfcommServerOverBtAdapter na31(3334, "6be60100-3629-11e7-a919-92ebcb67fe33");
  RfcommServerOverBtAdapter na32(3335, "6be60470-3629-11e7-a919-92ebcb67fe33");
  TCPServerOverEthAdapter na(2345, 2345);
  TCPServerOverEthAdapter na4(5555, 5555);
  TCPServerOverEthAdapter na5(6666, 6666);
  TCPServerOverWfdAdapter na2(3456, 3456, "OPEL");

  ca.set_control_adapter();
  na3.set_data_adapter();
  na31.set_data_adapter();
  na32.set_data_adapter();

  //na.set_data_adapter();
  //na4.set_data_adapter();
  //na5.set_data_adapter();
  na2.set_data_adapter();

  char sending_buf[8192];
  int ret;

  std::thread(receiving_thread).detach();

  while (true) {
    printf("Send > ");
    fgets(sending_buf, sizeof(sending_buf), stdin);
    ret = cm -> send_data(sending_buf, strlen(sending_buf)+1);
  }

  return 0;
}
