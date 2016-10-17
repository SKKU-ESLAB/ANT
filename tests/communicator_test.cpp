//#include <test_adapter.h>
#include <tcp_server_over_eth.h>
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
  TCPServerOverEthAdapter ca(1234, 1234);
  TCPServerOverEthAdapter na(2345, 2345);
  TCPServerOverEthAdapter na2(3456, 3456);

  ca.set_control_adapter();
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
