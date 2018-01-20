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
#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>

using namespace cm;

FILE *fp;
struct timeval start, end;
std::mutex lock;
std::condition_variable end_lock;


void receiving_thread() {
  void *buf = NULL;
  printf("receiving thread created! tid: %d\n", (unsigned int)syscall(224));

  while (true) {
//    fp = fopen("log.txt","a");

    int ret = Communicator::get_instance()->recv_data(&buf);
    printf("Recv %d> %s\n\n", ret, reinterpret_cast<char *>(buf));

   
    gettimeofday(&end, NULL);
    printf("%ld %ld \n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);  
//    fprintf(fp,"%ld\n",1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec));
  

    if(buf){
      free(buf);
    }

//    fclose(fp); 

    end_lock.notify_one();
    
  }

}

int main() {
  Communicator *cm = Communicator::get_instance();
  RfcommServerOverBt ca(1234, "150e8400-e29b-41d4-a716-446655440000");
  RfcommServerOverBt na3(3333, "150e8400-1234-41d4-a716-446655440000");
  RfcommServerOverBt na31(3334, "6be60100-3629-11e7-a919-92ebcb67fe33");
  RfcommServerOverBt na32(3335, "6be60470-3629-11e7-a919-92ebcb67fe33");

  TCPServerOverEthAdapter na(2345, 2345);
  TCPServerOverEthAdapter na4(5555, 5555);
  TCPServerOverEthAdapter na5(6666, 6666);

  TCPServerOverWfdAdapter na2(3456, 3456, "OPEL");

  //ca.set_control_adapter();
  na.set_control_adapter();

  na3.set_data_adapter();
  //na31.set_data_adapter();
  //na32.set_data_adapter();

  //na.set_data_adapter();
  //na4.set_data_adapter();
  //na5.set_data_adapter();
  
  na2.set_data_adapter();

  int iter = 0;
  int iter2 = 0;
  char sending_buf[8192];
  int ret, numbytes;
  int fd, count; 
  char *buffer; 
  char input[100];
  char file_dir[200];
  char* temp_buf;
 


  std::thread(receiving_thread).detach();

 
  int i;
  for(i=0; i<1; i++){
    sleep(2);
    temp_buf = (char*)calloc(5*1024, sizeof(char));
    cm -> send_data(temp_buf, 5*1024);
    sleep(10);
  }


  printf("now!\n");
  sleep(3);

  while (true) {
    std::unique_lock<std::mutex> lck(lock);

    printf("file to send-> "); 
    //usleep(5000);
    //sleep(1);
  
/*
    //scanf("%s",input);
    //sprintf(file_dir, "/home/pi/HOME/data/%s",input);
    sprintf(file_dir, "/home/pi/HOME/data/20k.mp4");
*/

    if(iter < 10 && iter2 == 0) {
      sleep(13);

      sprintf(file_dir, "/home/pi/data/10k");
    }
    else {
      //sleep(1);
      usleep(1000);
      sprintf(file_dir, "/home/pi/data/1m");

      if(iter2 == 5){
        iter = 0;
        iter2 = 0;
      } else {
        iter2++;
      }

    }

    //fp = fopen("log.txt","a");
    fd = open(file_dir, O_RDONLY); 
    if(fd < 0){
      fprintf(stderr, "error! can't find the file\n");
    } else {
    
      buffer = (char*) calloc (20*1024*1024, sizeof(char));
      if(buffer == NULL){
        fprintf(stderr, "error while allocating buffer!\n");
        exit(1);
      } else {
        count = read(fd, buffer, 20*1024*1024);
        printf("read!: %d\n", count);

        gettimeofday(&start, NULL);
        ret = cm -> send_data(buffer, count); 

        free(buffer);
      }
      close(fd); 
    }
    
    iter++;
    
    end_lock.wait(lck);
    //fclose(fp);

    //ret = cm -> send_data(sending_buf, strlen(sending_buf)+1);
  }

  return 0;
}
