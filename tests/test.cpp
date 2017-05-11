//#include <test_adapter.h>
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
  RfcommServerOverBt ca(1234, "150e8400-e29b-41d4-a716-446655440000");
  RfcommServerOverBt na3(3333, "150e8400-1234-41d4-a716-446655440000");
  TCPServerOverEthAdapter na(2345, 2345);
  TCPServerOverWfdAdapter na2(3456, 3456, "OPEL");

  ca.set_control_adapter();
  na3.set_data_adapter();
  na.set_data_adapter();
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
