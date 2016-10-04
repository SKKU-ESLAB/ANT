#include <test_adapter.h>
#include <thread>
#include <communicator.h>
#include <network_adapter.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace cm;

static NetworkAdapter *na = NULL;

void dev_stat_cb(DevState stat) {
  if (stat == kDevOn)
    na->dev_switch(kDevCon, NULL);
}

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
  TestAdapter ta;
  na = &ta;

  na->dev_switch(kDevOn, dev_stat_cb);

  char sending_buf[4096];
  int ret;

  std::thread(receiving_thread).detach();

  while (true) {
    printf("Send > ");
    fgets(sending_buf, sizeof(sending_buf), stdin);
    ret = cm -> send_data(sending_buf, strlen(sending_buf)+1);
  }

  return 0;
}
