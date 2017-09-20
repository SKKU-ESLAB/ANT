extern "C"{

#define BT_DEBUG 1

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <comm_bt.h>
#include <sys/socket.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <string.h>

int bt_dynamic_bind_rc(int sock)
{
	int err;
	int tmp_port;
	struct sockaddr_rc sockaddr;
	bdaddr_t my_bdaddr_any = {0,0,0,0,0,0};
	sockaddr.rc_family = AF_BLUETOOTH;
	sockaddr.rc_bdaddr = my_bdaddr_any;
	sockaddr.rc_channel = (uint8_t) 0;
	
	for ( tmp_port = 1; tmp_port < 31; tmp_port++ ){
		sockaddr.rc_channel = tmp_port;
		err = bind(sock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_rc));
		if( !err ){
			return tmp_port;
		}

		if( errno == EINVAL){
			break;
		}
	}
	if ( tmp_port == 31 ) {
		err = -1;
		errno = EINVAL;
	}
	if(errno == EINVAL)
		return -1;

	return err;
}

sdp_session_t *bt_register_service(uint8_t *sint, int port)
{
	const char *service_name = "ANT Service";
	const char *service_dsc = "ANT Desc";
	const char *service_prov = "ANT Prov";
	sdp_session_t *t_session = 0;
	int i;

	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;
	sdp_list_t *l2cap_list = 0,
			   *rfcomm_list = 0,
			   *root_list = 0,
			   *proto_list = 0,
			   *access_proto_list = 0;
	sdp_data_t *channel = 0;

	sdp_record_t *record = sdp_record_alloc();

	// set the general service ID

	sdp_uuid128_create( &svc_uuid, sint );
	//for(i=0; i<16; i++){
	//	printf("%c %x|", svc_uuid.value.uuid128.data[i], svc_uuid.value.uuid128.data[i]);
	//}
	//printf("\n");

	sdp_set_service_id( record, svc_uuid );

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups( record, root_list );

	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append( 0, &l2cap_uuid );
	proto_list = sdp_list_append( 0, l2cap_list );

	// set rfcomm information
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &port);
	rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
	sdp_list_append( rfcomm_list, channel );
	sdp_list_append( proto_list, rfcomm_list );

	// attach protocol information to service record
	access_proto_list = sdp_list_append( 0, proto_list );
	sdp_set_access_protos( record, access_proto_list );

	// set the name, provider, and description
	sdp_set_info_attr(record, service_name, service_prov, service_dsc);

	// connect to the local SDP server, register the service record, and disconnect
	bdaddr_t my_bdaddr_any = {0, 0, 0, 0, 0, 0};
	bdaddr_t my_bdaddr_local = {0, 0, 0, 0xff, 0xff, 0xff};
	t_session = sdp_connect( &my_bdaddr_any, &my_bdaddr_local, SDP_RETRY_IF_BUSY );
	if(NULL == t_session)
		return NULL;
	int err = sdp_record_register(t_session, record, 0);

	sdp_data_free( channel );
	sdp_list_free( l2cap_list, 0 );
	sdp_list_free( rfcomm_list, 0 );
	sdp_list_free( root_list, 0 );
	sdp_list_free( access_proto_list, 0 );

	return t_session;
}


int bt_connect(uint8_t *tmp_uuid, int sock)
{
	inquiry_info *ii = NULL;
	int i, max_rsp, err;
	int dev_id, hci_sock, len, flags;
	uuid_t svc_uuid;
	sdp_list_t *response_list = NULL, *search_list, *attrid_list;
	sdp_session_t *session;
	uint32_t range = 0x0000ffff;
	uint8_t rfcomm_channel = 0;
	struct sockaddr_rc addr = {0};
	int status;
	int num_rsp;
	err = BT_S_OK;


	inquiry_info *tmp_bt_scan_list;
	dev_id = hci_get_route(NULL);
	hci_sock = hci_open_dev(dev_id);
	if( dev_id < 0 || hci_sock < 0 ){
		if(BT_DEBUG)
			printf("No BT Device\n");
		return -BT_E_FAIL;
	}

	len = MAX_BT_SCAN_LEN;
	max_rsp = MAX_BT_SCAN_RSP;
	flags = IREQ_CACHE_FLUSH;
	tmp_bt_scan_list = (inquiry_info *)malloc(max_rsp * sizeof(inquiry_info));
	if(BT_DEBUG)
		printf("connect:hci_inquiry...\n");
	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	
	if(num_rsp < 0){
		return -BT_E_FAIL;
	}
	
	sdp_uuid128_create( &svc_uuid, tmp_uuid);
//	for(i=0; i<16; i++){
//		printf("%c %x|", svc_uuid.value.uuid128.data[i], svc_uuid.value.uuid128.data[i]);
//	}
//	printf("\n");

	search_list = sdp_list_append(NULL, &svc_uuid);

	bdaddr_t my_bdaddr_any = {0,0,0,0,0,0};
	char bdad[256];
	for (i=0; i<num_rsp; i++){
		ba2str(&(ii+i)->bdaddr, bdad);
		if(BT_DEBUG)
			printf("connecting:%s\n", bdad);
		session = sdp_connect(&my_bdaddr_any, &((ii+i)->bdaddr), SDP_RETRY_IF_BUSY);
		//session = sdp_connect(BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
		if(NULL == session){
			if(BT_DEBUG)
				printf("session not connected\n");
			continue;
		}
		else
			if(BT_DEBUG)
				printf("session connected %s\n", bdad);
		search_list = sdp_list_append(NULL, &svc_uuid);
		attrid_list = sdp_list_append(NULL, &range);

		err = sdp_service_search_attr_req(session, search_list,\
				SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
		sdp_list_t *r = response_list;

		for(; r; r = r->next){
			sdp_record_t *rec = (sdp_record_t *) r->data;
			sdp_list_t *proto_list;

			if (sdp_get_access_protos(rec, &proto_list) == 0) {
				sdp_list_t *p = proto_list;

				// go through each protocol sequence
				for( ; p ; p = p->next ) {
					sdp_list_t *pds = (sdp_list_t*)p->data;

					// go through each protocol list of the protocol sequence
					for( ; pds ; pds = pds->next ) {

						// check the protocol attributes
						sdp_data_t *d = (sdp_data_t*)pds->data;
						int proto = 0;
						for( ; d; d = d->next ) {
							switch( d->dtd ) {
								case SDP_UUID16:
								case SDP_UUID32:
								case SDP_UUID128:
									proto = sdp_uuid_to_proto( &d->val.uuid );
									break;
								case SDP_UINT8:
									if( proto == RFCOMM_UUID ) {
										rfcomm_channel = d->val.int8;
									}
									break;
							}
						}
					}
					sdp_list_free( (sdp_list_t*)p->data, 0 );
				}
			}
		}

		if(rfcomm_channel > 0)
			break;
	}

	if(rfcomm_channel > 0){
		if(BT_DEBUG)
			printf("rfcomm channel has been connected %d\n", rfcomm_channel);
		addr.rc_family = AF_BLUETOOTH;
		addr.rc_channel = rfcomm_channel;
		str2ba(bdad, &addr.rc_bdaddr);
	//	addr.rc_bdaddr = *BDADDR_LOCAL;

		status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

		if(status != 0){
			if(BT_DEBUG)
				printf("status : %d(%s)\n", status, strerror(errno));
			err = -BT_E_FAIL;
		}
		else
			err = BT_S_OK;
	}
	else{
		if(BT_DEBUG)
			printf("T_T no rfcomm channel found : %d\n", rfcomm_channel);

		err = -BT_E_FAIL;

	}

	close(hci_sock);

	free(ii);

	return err;
}
}
