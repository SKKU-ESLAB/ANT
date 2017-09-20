#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <uv.h>

#include "comm_util.h"

#define IPC_PATH "/tmp/ant_comm_manager"

static int bt_sock;
static int un_sock;


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
	if(bind (bt_sock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0){
		comm_log("Failed to bind bt socket");
		exit(1);
	}
}

void un_init(){
	struct sockaddr_un un_addr = {0};

	un_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(un_sock < 0){
		comm_log("Failed to initialize un socket");
		exit(1);
	}

	un_addr.sun_family = AF_UNIX;
	strcpy(un_addr.sun_path, IPC_PATH);

	unlink(IPC_PATH);

	if( bind(un_sock, (struct sockaddr *)&un_addr, sizeof(un_addr)) < 0){
		comm_log("Failed to bind un socket");
		exit(1);
	}
}


int main()
{

	return 0;
}
