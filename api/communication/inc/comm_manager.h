#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <uv.h>

#include "comm_util.h"

static int bt_sock;
static int ipc_sock;


void bt_init(){
	struct sockaddr_rc loc_addr = {0};

	bt_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(bt_sock < 0){
		comm_log("Failed to initialize bt socket");
		exit(1);
	}

	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;
	bind (bt_sock, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
}

void ipc_init(){
	struct sockaddr_un loc_addr = {0};

	ipc_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(ipc_sock < 0){
		comm_log("Failed to initialize ipc socket");
		exit(1);
	}	
	
	loc_addr.sun_family = AF_UNIX;
	loc_addr.rc_
}

int main()
{
	bt_sock = socket(AF
}

