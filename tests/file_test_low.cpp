#include <rfcomm_over_bt.h>
#include <tcp_server_over_eth.h>
#include <tcp_server_over_wfd.h>
#include <thread>
#include <communicator.h>
#include <network_adapter.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>

using namespace cm;

#define DEBUG_SHOW_DATA 0
#define DEBUG_SHOW_TIME 0

#if DEBUG_SHOW_TIME == 1
struct timeval start, end;
#endif

FILE *fp;
std::mutex lock;
std::condition_variable end_lock;

void receiving_thread() {
  void *buf = NULL;
  printf("[INFO] Receiving thread created! tid: %d\n", (unsigned int)syscall(224));

  while (true) {
    int ret = Communicator::get_instance()->recv_data(&buf);
#if DEBUG_SHOW_DATA == 1
    printf("Recv %d> %s\n\n", ret, reinterpret_cast<char *>(buf));
#endif
#if DEBUG_SHOW_TIME == 1
    gettimeofday(&end, NULL);
    printf("%ld %ld \n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);  
#endif

    if(buf) free(buf);
    end_lock.notify_one();
  }
}

int main() {
  Communicator *cm = Communicator::get_instance();
  TCPServerOverEthAdapter ethAdapter(2345, 2345);
  RfcommServerOverBt btAdapter(3333, "150e8400-1234-41d4-a716-446655440000");
  TCPServerOverWfdAdapter wfdAdapter(3456, 3456, "OPEL");

  printf("Step 1. Initializing Network Adapters\n");

  printf("  a) Control Adapter: TCP over Ethernet\n");
  ethAdapter.set_control_adapter();
  printf("  b) Data Adapter: RFCOMM over Bluetooth\n");
  btAdapter.set_data_adapter();
  printf("  c) Data Adapter: TCP over Wi-fi Direct\n");
  wfdAdapter.set_data_adapter();

  int iter = 0;
  int iter1 = 0;
  int iter2 = 0;
  char sending_buf[8192];
  int ret, numbytes;
  int fd, count; 
  char *buffer; 
  char input[100];
  char file_name[200];
  char file_dir[200];
  char* temp_buf;

  std::thread(receiving_thread).detach();

#define TEST_DATA_SIZE (5*1024)
  printf("Step 2. Send Test Data (%dB)\n", TEST_DATA_SIZE);
  int i;
  for(i=0; i<1; i++) {
    sleep(2);
    temp_buf = (char*)calloc(TEST_DATA_SIZE, sizeof(char));
    cm->send_data(temp_buf, TEST_DATA_SIZE);
    sleep(10);
    free(temp_buf);
  }

  printf("now!\n");
  sleep(3);

#define DATA_PATH "/home/redcarrottt/data"
#define SMALL_FILE_NAME "10k"
#define LARGE_FILE_NAME "1m"
#define BUFFER_SIZE (20*1024*1024)
  printf("Step 3. Send Mixed Workload\n");
  while (true) {
    std::unique_lock<std::mutex> lck(lock);
    if(iter1 < 10 && iter2 == 0) {
      sleep(13);
      sprintf(file_name, "%s", SMALL_FILE_NAME);
    } else {
      usleep(1000);
      sprintf(file_name, "%s", LARGE_FILE_NAME);
      if(iter2 == 5){
        iter1 = 0;
        iter2 = 0;
      } else {
        iter2++;
      }
    }
    if(iter == 100) {
      printf("Finish Workload\n");
      break;
    }
    sprintf(file_dir, "%s/%s", DATA_PATH, file_name);

    printf(" * Iteration %d (File: %s)\n", iter, file_dir);
    printf("  - Send File: %s\n", file_dir); 

    fd = open(file_dir, O_RDONLY); 
    if(fd < 0) {
      fprintf(stderr, "[Error] Cannot find the file: %s\n", file_dir);
    } else {
      buffer = (char*) calloc (BUFFER_SIZE, sizeof(char));
      if(buffer == NULL){
        fprintf(stderr, "[Error] Buffer allocation failed: size=%d\n", BUFFER_SIZE);
        exit(1);
      } else {
        count = read(fd, buffer, BUFFER_SIZE);
#if DEBUG_SHOW_DATA == 1
        printf("  - Read data: size=%d\n", count);
#endif

#if DEBUG_SHOW_TIME == 1
        gettimeofday(&start, NULL);
#endif
        // Send Data
        ret = cm->send_data(buffer, count); 

        free(buffer);
      }
      close(fd); 
    }
    iter++;
    iter1++;
    end_lock.wait(lck);
  }

  return 0;
}
