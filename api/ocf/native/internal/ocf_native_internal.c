#include "ocf_native_internal.h"
#include "../../../common/native/ant_common.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// OCF thread
pthread_t g_ocf_thread;

void *ocf_thread_fn(void *arg) {
  // On OCF Thread
  // TODO: implement it
  printf("OCF thread launched\n");
}

void ocf_initialize_internal() { 
  pthread_create(&g_ocf_thread, NULL, &ocf_thread_fn, NULL);
}

void initOCF(void) {
  // Empty function
}