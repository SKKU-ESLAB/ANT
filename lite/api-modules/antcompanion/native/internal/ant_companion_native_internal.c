#include "ant_companion_native_internal.h"
#include "../../../common/native/ant_common.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void ant_companion_getMyIPAddress_internal(const char *interfaceName,
                                           char *resultIPAddress) {
  int fd;
  struct ifreq ifr;
  const char *ipAddress;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  /* I want to get an IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;

  /* I want IP address attached to "eth0" */
  strncpy(ifr.ifr_name, interfaceName, IFNAMSIZ - 1);

  ioctl(fd, SIOCGIFADDR, &ifr);

  close(fd);

  ipAddress = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
  strncpy(resultIPAddress, ipAddress, strlen(ipAddress) + 1);
  return;
}

void initANTCompanion(void) {
  // Empty function
}