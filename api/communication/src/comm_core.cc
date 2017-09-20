#include <string.h>
#include <errno.h>
#include <comm_util.h>
#include <comm_core.h>
#include <comm_bt.h>
#include <crc_ccit.h>

static uint32_t last_req_id;

/* ANT_Header Implementation */
ANT_Header::ANT_Header()
{
	req_id = 0;
	data_len = 0;
	chksum = 0;
	initialized = FALSE;
	type = PACKET_TYPE_MSG;
}
int ANT_Header::init_from_buff(uint8_t *buff)
{
	if(NULL == buff){
		comm_log("buff is null");
		return -COMM_E_INVALID_PARAM;
	}
	req_id = btohl( *( (uint32_t *)(buff) ) );
	data_len = btohl( *( (uint32_t *)(buff+4) ) );
	type = btohs(*(uint16_t *)(buff+8));
	err = btohs(*(uint16_t *)(buff+10));
	chksum = btohs(*(uint16_t *)(buff+12));
	if(PACKET_TYPE_FILE & type){
		memcpy(data_info.finfo.fname, buff+14, 24);
		data_info.finfo.fsize = btohl( *( (uint32_t *) (buff + 38)) );
		data_info.finfo.offset = btohl( *( (uint32_t *) (buff+42) ) );
		memcpy(destFname, buff+46, 70);
	}
	else if (PACKET_TYPE_MSG & type){
		memcpy(data_info.minfo.dest_intf, buff+14, 16);
		memcpy(data_info.minfo.src_intf, buff+30, 16);
	}

	initialized = TRUE;

	return COMM_S_OK;
}
int ANT_Header::init_to_buff(uint8_t *buff)
{
	uint32_t b_req_id = htobl(req_id);
	uint32_t b_dat_len = htobl(data_len);
	uint16_t b_type = htobs(type);
	uint16_t b_err = htobs(err);
	uint16_t b_chksum = htobs(chksum);

	if(NULL == buff){
		comm_log("buff is null");
		return -COMM_E_INVALID_PARAM;
	}

	memcpy(buff, (uint8_t *)&b_req_id, 4);
	memcpy(buff+4,(uint8_t *)&b_dat_len, 4);
	memcpy(buff+8, (uint8_t *)&b_type, 2);
	memcpy(buff+10, (uint8_t *)&b_err, 2);
	memcpy(buff+12, (uint8_t *)&chksum, 2);

	if(PACKET_TYPE_FILE & type){
		uint32_t b_offset = htobl(data_info.finfo.offset);
		memcpy(buff+14, (uint8_t *)data_info.finfo.fname, 24);
		memcpy(buff+38, (uint8_t *)&(data_info.finfo.fsize), 4);
		memcpy(buff+42, (uint8_t *)&b_offset, 4);
		memcpy(buff+46, (uint8_t *)destFname, 24);
	}
	else if (PACKET_TYPE_MSG & type) {
		memcpy(buff+14, data_info.minfo.dest_intf, 16);
		memcpy(buff+30, data_info.minfo.src_intf, 16);
	}

	return COMM_S_OK;
}
//

/* ANT_MSG Implementation */
ANT_MSG::ANT_MSG()
{
	op_sock = NULL;
	op_header = new ANT_Header();
	data = NULL;
	op_header->err = SOCKET_ERR_NONE;
	priv = NULL;
}
ANT_MSG::ANT_MSG(ANT_Socket *op_sock)
{
	this->op_sock = op_sock;
	dynamic_sock_get(&(this->op_sock));
	op_header = new ANT_Header();
	data = NULL;
	op_header->err = SOCKET_ERR_NONE;
}
ANT_MSG::~ANT_MSG()
{
	if(NULL != op_sock){
		dynamic_sock_put(&op_sock);
	}
	delete op_header;
	if(NULL != data)
		free(data);
}
ANT_Header* ANT_MSG::get_header()
{
	return op_header;
}
uint8_t* ANT_MSG::get_data()
{
	return data;
}
uint16_t ANT_MSG::get_err()
{

	return op_header->err;
}
void ANT_MSG::set_data(uint8_t *data_p, 
		uint32_t data_len)
{
	data = data_p;
	op_header->data_len = data_len;
	op_header->initialized = TRUE;
}
uint32_t ANT_MSG::get_req_id()
{
	if(!op_header->isInitialized())
		return 0;
	return op_header->req_id;
}
uint32_t ANT_MSG::get_data_len()
{
	if(!op_header->isInitialized())
		return 0;
	return op_header->data_len;
}
bool ANT_MSG::is_file()
{

	if(!op_header->isInitialized())
		return FALSE;

	if(PACKET_TYPE_FILE & op_header->type)
		return TRUE;
	else return FALSE;
}
bool ANT_MSG::is_special()
{
	/*	if(!op_header->isInitialized())
		return FALSE;
		return ((op_header->data_len) >> 31);
	 */
	if(!op_header->isInitialized())
		return FALSE;
	if(PACKET_TYPE_SPE & op_header->type)
		return TRUE;
	else return FALSE;
}
void ANT_MSG::set_special()
{
	op_header->type |= PACKET_TYPE_SPE;
}
bool ANT_MSG::is_msg()
{
	if(!op_header->isInitialized())
		return FALSE;

	if(PACKET_TYPE_MSG & op_header->type)
		return TRUE;

	else return FALSE;
}
bool ANT_MSG::is_ack()
{
	bool res = FALSE;
	if(op_header->isInitialized()){
		if(PACKET_TYPE_ACK & op_header->type)
			res = TRUE;
	}

	return res;
}
uint32_t ANT_MSG::get_file_offset()
{
	if(!op_header->isInitialized())
		return FALSE;
	return (op_header->data_info).finfo.offset;
}
uint32_t ANT_MSG::get_file_size()
{
	if(!op_header->isInitialized())
		return 0;
	return (op_header->data_info).finfo.fsize;
}
char *ANT_MSG::get_file_name()
{
	if(!op_header->isInitialized())
		return NULL;
	return (op_header->data_info).finfo.fname;
}

void ANT_MSG::set_req_id(uint32_t req)
{
	op_header->req_id = req;
}
void ANT_MSG::set_file(file_info_t *finfo)
{
	if(NULL != finfo ){
		strncpy((op_header->data_info).finfo.fname, finfo->fname, 24);
		(op_header->data_info).finfo.offset = finfo->offset;
		op_header->data_info.finfo.fsize = finfo->fsize;
	}
	op_header->type &= PACKET_TYPE_ACK;
	op_header->type |= PACKET_TYPE_FILE;
}
void ANT_MSG::set_msg(msg_info_t *minfo)
{
	if(NULL != minfo){
		strncpy((op_header->data_info).minfo.dest_intf, minfo->dest_intf, 16);
		strncpy((op_header->data_info).minfo.src_intf, minfo->src_intf, 16);
	}
	op_header->type &= PACKET_TYPE_ACK;
	op_header->type |= PACKET_TYPE_MSG;
}
void ANT_MSG::set_ack()
{
	op_header->type |= PACKET_TYPE_ACK;
}
void ANT_MSG::set_err(uint16_t err)
{
	this->op_header->err = err;
}
void ANT_MSG::complete_header()
{
	op_header->initialized = TRUE;
}
ANT_Socket *ANT_MSG::get_op_sock()
{
	return op_sock;
}
char *ANT_MSG::get_dfile_name()
{
	if(!op_header->isInitialized())
		return NULL;
	return op_header->destFname;
}
void ANT_MSG::set_dfile_name(char *src)
{
	memcpy(op_header->destFname, src, 24);
}
//

/* queue_data_t Implementation */
queue_data_t::queue_data_t()
{
	op_msg = new ANT_MSG();

	comm_queue_init_node(&queue);
	cv_num = -1;
	sock_pos = -1;
	buff_len = 0;
	attached = 0;
	handler = NULL;
	buff = NULL;
}
queue_data_t::queue_data_t(ANT_Socket *op_sock)
{
	op_msg = new ANT_MSG(op_sock);
	comm_queue_init_node(&queue);
	cv_num = -1;
	sock_pos = -1;
	buff_len = 0;
	attached = 0;
	handler = NULL;
	buff = NULL;
}
queue_data_t::~queue_data_t()
{
	attached = -1;
	if(NULL != op_msg){
		delete op_msg;

	}
	if(NULL != buff){
		free(buff);
	}
}
void queue_data_t::call_handler(void)
{
	ANT_Header *op_header;

	if(NULL == handler)
		return;

	op_header = op_msg->get_header();
	if(!op_header->isInitialized())
		return;

	handler(op_msg, op_msg->get_err());
}
//

/* Generic_queue implementation */
ANT_Comm_Queue::ANT_Comm_Queue()
{
	uv_mutex_init(&queue_mutex);
	comm_queue_init(&queue);
	len = 0;
}
uint32_t ANT_Comm_Queue::get_len()
{
	return len;
}

/*
   Sync Case
   Two atomic operations are always done concurrently.
   1. data in queue, but attached = false
   2. data not in queue, but attached = true
   Which one is fine?
   var "attached" is used to prevent attached data from being deleted.
   When "attached" is FALSE, it is ok to delete the data.
   When "attached" is TRUE, however, the data MUST not be deleted.
   Therefore, case 2 is fine because the data is not deleted at least, and "attached" will be false sooner.

   "attached" should be TURE before the data is inserted.
   "attached" should be FALSE after! the data is removed.
 */
int ANT_Comm_Queue::enqueue(IN queue_data_t *data)
{
	data->attached++;

	uv_mutex_lock(&queue_mutex);
	comm_queue_insert_tail(&queue, &(data->queue));
	len++;
	uv_mutex_unlock(&queue_mutex);

	return COMM_S_OK;
}
queue_data_t* ANT_Comm_Queue::dequeue()
{
	comm_queue_t *tmp_queue = NULL;
	queue_data_t *res = NULL;

	if(len == 0)
		return NULL;

	uv_mutex_lock(&queue_mutex);
	tmp_queue = comm_queue_head(&queue);
	res = comm_queue_data(tmp_queue, queue_data_t, queue);
	comm_queue_remove(tmp_queue);
	len--;
	uv_mutex_unlock(&queue_mutex);

	res->attached--;	
	//Now it is OK to delete the data, but it is not deleted in this method.
	return res;
}
bool ANT_Comm_Queue::isEmptyQueue()
{
	return (0 == len);
}
//

/* ANT Socket implementation */
static int init_mutex;
static uv_mutex_t sock_op_lock;

bool dynamic_sock_get(ANT_Socket **sock)
{
	bool res = FALSE;
	if(!init_mutex){
		uv_mutex_init(&sock_op_lock);
	}
	uv_mutex_lock(&sock_op_lock);
	do{
		if(NULL == sock || NULL == *sock)
			break;

		res = (*sock)->get();
	}while(0);
	uv_mutex_unlock(&sock_op_lock);

	return res;
}
bool dynamic_sock_put(ANT_Socket **sock)
{
	bool res = FALSE;
	if(!init_mutex){
		uv_mutex_init(&sock_op_lock);
	}
	uv_mutex_lock(&sock_op_lock);
	do{
		if(NULL == sock || NULL == *sock)
			break;

		res = (*sock)->put();

		if(res && (*sock)->get_ref_cnt() == 0){
			comm_log("Socket deleting %x-%d", *sock, (*sock)->get_sock_fd());
			delete (*sock);
			*sock = NULL;
		}
	}while(0);
	uv_mutex_unlock(&sock_op_lock);

	return res;
}
ANT_Socket::ANT_Socket(int sock_fd, uint8_t type)
{
	sock = sock_fd;
	connection_type = type;
	ref_cnt = 0;
}
ANT_Socket::~ANT_Socket()
{
	int cnt = 0;
	while(ref_cnt > 0){
		comm_log("socket remove failed.");
		sleep(1);
		cnt++;
		if(cnt == 5){
			comm_log("5 times failed...force to close socket");
			break;
		}

	}
	close(sock);
}
uint8_t ANT_Socket::get_ref_cnt()
{
	return ref_cnt;
}
int ANT_Socket::get_sock_fd()
{
	return sock;
}
void ANT_Socket::set_sock_fd(int sock_fd)
{
	sock = sock_fd;
}
bool ANT_Socket::get()
{
	if((uint8_t)(ref_cnt+1) < ref_cnt){
		comm_log("Overflow");
		ref_cnt--;
		return FALSE;
	}

	ref_cnt++;
	comm_log("REFCNT:%d", ref_cnt);

	return TRUE;
}
bool ANT_Socket::put()
{
	if(ref_cnt == 0){
		comm_log("ref_cnt, but put? error");
		return FALSE;
	}

	ref_cnt--;
	comm_log("REFCNT:%d", ref_cnt);
	if(ref_cnt == 0)
		comm_log("Must not happen");

	return TRUE;
}
int ANT_Socket::Read(OUT uint8_t *buff, IN int size)
{
	int rCount = 0;
	//int res;
	//if(FALSE == get()){
	//comm_log("Get socket failed");
	//return -1;
	//}
	rCount = read(sock, buff, size);
	//res = put();
	if(rCount < 0){
		comm_log("Read failed %s", strerror(errno));
		return -1;
	}
	if(rCount == 0)
		return 0;
	//if(FALSE == res){
	//comm_log("Put socket failed");
	//return -1;
	//}

	return rCount;
}

int ANT_Socket::Write(IN uint8_t *buff, IN int size)
{
	int wCount = 0;
	//int res;
	//if(FALSE == get())
	//return -1;
	wCount = write(sock, buff, size);
	//res = put();
	if(size > 0 && wCount <= 0){
		return wCount;
	}

	//if(FALSE == res)
	//return -1;

	return wCount;
}
//

/* BT_Operations implementation */
BT_Operations::BT_Operations()
{
	port = 0;
	session = NULL;
}
BT_Operations::~BT_Operations()
{
	if(NULL != session)
		sdp_close(session);

}

int BT_Operations::dynamic_bind_rc(int sock)
{
	return bt_dynamic_bind_rc(sock);
}

sdp_session_t* BT_Operations::register_service(char *intf_name, \
		int port)
{
	return bt_register_service((uint8_t*)intf_name, port);
}

int BT_Operations::openBT(int sock, char *intf_name)
{
	port = dynamic_bind_rc(sock);
	if(!(port >= 1 && port <= 30)){
		comm_log("dynamic binding failed");
		return -1;
	}
	else
		comm_log("Bound socket %d to port %d", sock, port);
	session = register_service(intf_name, port);
	if(NULL == session){
		comm_log("session creation failed");
		return -1;
	}
	else
		comm_log("Session created");

	return COMM_S_OK;
}
int BT_Operations::connectBT(char *intf_name, OUT int sock)
{
	//uint32_t uuid[4];
	//name2uuid(intf_name, uuid);
	return bt_connect((uint8_t*)intf_name, sock);
}
//

/* socket_set Implementation */

socket_set::socket_set()
{
	int i;
	uv_mutex_init(&socket_set_mutex);
	for(i=0; i<MAX_CLIENTS; i++)
		sockets[i] = NULL;
	socket_set_bitmap = 0;
	socket_set_len = 0;
}
socket_set::~socket_set()
{
	while(socket_set_len > 0){
		int pos = find_first_zero_bit(socket_set_bitmap);
		remove(pos);
	}

}

int socket_set::insert(IN ANT_Socket *op_sock)
{
	int pos;

	uv_mutex_lock(&socket_set_mutex);
	do{
		pos = find_first_zero_bit(socket_set_bitmap);
		if(pos < 0 || pos >= 30){
			pos = -1;
			break;
		}
		if(NULL != sockets[pos]){
			pos = -1;
			break;
		}
		socket_set_len++;
		socket_set_bitmap |= 0x01 << pos;

		sockets[pos] = op_sock;
	}while(0);
	uv_mutex_unlock(&socket_set_mutex);

	return pos;
}

int socket_set::insert(IN int sock_fd, uint8_t type)
{
	int pos;

	uv_mutex_lock(&socket_set_mutex);
	do{
		pos = find_first_zero_bit(socket_set_bitmap);
		if(pos < 0 || pos >= 30){
			pos = -1;
			break;
		}
		if(NULL != sockets[pos]){
			pos = -1;
			break;
		}
		socket_set_len++;
		socket_set_bitmap |= 0x01 << pos;

		sockets[pos] = new ANT_Socket(sock_fd, type);
		if(FALSE == dynamic_sock_get(&sockets[pos])){
			comm_log("What..");
			delete sockets[pos];
			sockets[pos] = NULL;
		}
	}while(0);
	uv_mutex_unlock(&socket_set_mutex);

	return pos;
}

void socket_set::remove(IN int sock_no)
{
	uv_mutex_lock(&socket_set_mutex);
	comm_log("Removing the socket %d", sock_no);

	do{
		socket_set_bitmap &= ~(0x01 << sock_no);
		if(NULL == sockets[sock_no]){
			break;
		}
		socket_set_len--;
		if(FALSE == dynamic_sock_put(&sockets[sock_no])){
			comm_log("What..");
			delete sockets[sock_no];
			sockets[sock_no] = NULL;
		}
		sockets[sock_no] = NULL;
	}while(0);

	uv_mutex_unlock(&socket_set_mutex);
	return;
}

ANT_Socket *socket_set::get(int pos)
{
	ANT_Socket *res = NULL;
	uv_mutex_lock(&socket_set_mutex);
	res = sockets[pos];
	uv_mutex_unlock(&socket_set_mutex);

	return res;
}

int socket_set::length()
{
	return socket_set_len;
}
/* req_set Implementation */
req_set::req_set(ANT_Comm_Queue *ackQueue)
{
	int i;
	for(i=0; i<MAX_REQ_LEN; i++){
		tout[i] = -1;
		qdts[i] = NULL;
	}
	this->ackQueue = ackQueue;
	uv_cond_init(&inserted);
	uv_mutex_init(&lock);
}
req_set::~req_set()
{
	int i;
	for(i=0; i<MAX_REQ_LEN; i++){
		if(qdts[i]->attached==0)
			delete qdts[i];
		qdts[i] = NULL;
	}
}
int req_set::wait(uint32_t timeout)
{
	int i, res = -1;
	uv_mutex_lock(&lock);
	res = uv_cond_timedwait(&inserted, &lock, timeout*SECFROMNANO);
	if(res < 0){
		for(i=0; i<MAX_REQ_LEN; i++){
			if(qdts[i] != NULL){
				tout[i]--;
				if(tout[i] == 0){
					//REQ Processing in need?
					char *to_str = "Time out...";
					ANT_MSG *op_msg = qdts[i]->op_msg;
					op_msg->set_data((uint8_t*)to_str, strlen(to_str)+1);
					op_msg->set_err(SOCKET_ERR_TOUT);
					op_msg->set_special();
					qdts[i]->attached--;
					ackQueue->enqueue(qdts[i]);
					qdts[i] = NULL;
					tout[i] = -1;
				}
			}
		}
	}
	uv_mutex_unlock(&lock);
}

// Read qdt has no 
void req_set::signal(int reqId, queue_data_t *qdt, int refresh)
{
	int i, res = 0;
	if(refresh){
		uv_mutex_lock(&lock);
		for(i=0; i<MAX_REQ_LEN; i++){
			if(qdts[i] != NULL && qdts[i]->op_msg->get_req_id() == reqId){
				tout[i] = MAX_FILE_TIMEOUT;
				break;
			}
		}
		uv_mutex_unlock(&lock);
	}
	else{
		comm_log("got?");
		uv_mutex_lock(&lock);
		for(i=0; i<MAX_REQ_LEN; i++){
			if(qdts[i] != NULL && qdts[i]->op_msg->get_req_id() == reqId){
				comm_log("oops?got?");
				if(NULL == qdt){
					comm_log("If not refreshed, then must put queue data first");
				}
				else{
					res = 1;
					qdt->handler = qdts[i]->handler;
					qdt->op_msg->priv = qdts[i]->op_msg->priv;
					ackQueue->enqueue(qdt);
					comm_log("%s enqueued msg", qdt->op_msg->get_data());
					qdts[i]->attached--;
					if(qdts[i]->attached==0)delete qdts[i];
					qdts[i] = NULL;
					tout[i] = -1;
				}
			}
		}
		if(res == 1)
			uv_cond_signal(&inserted);
		uv_mutex_unlock(&lock);
	}

}
int req_set::insert(queue_data_t *qdt)
{
	int i, res = -1;
	uv_mutex_lock(&lock);
	do{
		if(!qdt)
			break;
		for(i=0; i<MAX_REQ_LEN; i++){
			if(NULL == qdts[i]){
				comm_log("insert:got?");
				qdt->attached++;
				res = 0;
				qdts[i] = qdt;
				tout[i] = MAX_FILE_TIMEOUT;
				break;
			}
		}
	}while(0);
	uv_mutex_unlock(&lock);

	return res;
}

/* cv_set Implementation */
/*
cv_set::cv_set()
{
	int i;
	uv_mutex_init(&cv_bitmap_mutex);
	cv_bitmap = 0;
	cv_len = 0;
	for(i=0; i<MAX_REQ_LEN; i++){
		uv_mutex_init(&cv_mutex[i]);
		uv_cond_init(&cv[i]);
		uv_cond_init(&notWaiting[i]);
		req[i] = 0;
		status[i] = CV_STAT_INIT;
	}
}
cv_set::~cv_set()
{
	while(cv_bitmap){
		uv_mutex_lock(&cv_bitmap_mutex);
		int pos = find_first_zero_bit(~cv_bitmap);
		if(pos < 0){
			uv_mutex_unlock(&cv_bitmap_mutex);
			continue;
		}
		uv_cond_signal(&cv[pos]);
		cv_bitmap &=~( 0x01 << pos );
		uv_mutex_unlock(&cv_bitmap_mutex);

	}
}
int cv_set::getLen()
{
	int res = -1;
	uv_mutex_lock(&cv_bitmap_mutex);
	res = cv_len;
	uv_mutex_unlock(&cv_bitmap_mutex);
	return res;
}

int cv_set::insert(uint32_t reqid, Comm_Handler handler)
{
	int pos;
	uint8_t stat;
	if(~cv_bitmap == 0){
		comm_log("It's full");
		return -1;
	}

	if(reqid == 0 || handler == NULL){
		comm_log("reqid ==0 or handler == NULL");
		return -1;
	}

	if(search(reqid, &stat) >= 0){
		comm_log("reqid duplicated(%d:%d)", reqid,stat);
		return -1;
	}

	uv_mutex_lock(&cv_bitmap_mutex);

	do{
		if(cv_len == MAX_REQ_LEN)
			break;
		pos = find_first_zero_bit(cv_bitmap);
		if(pos < 0)
			break;

		uv_mutex_lock(&cv_mutex[pos]);
		cv_bitmap |= 0x01 << pos;
		req[pos] = reqid;
		handlers[pos] = handler;
		comm_log("Inserted handler(%d:%d)-%x:%x", pos, req[pos], handler, handlers[pos]);
		status[pos] = CV_STAT_READY;
		cv_len++;
		uv_mutex_unlock(&cv_mutex[pos]);

	}while(0);

	uv_mutex_unlock(&cv_bitmap_mutex);

	return pos;
}
int cv_set::remove(uint32_t reqid)
{
	int i, wt = 0, res = -1;
	uv_mutex_lock(&cv_bitmap_mutex);
	do{
		for(i=0; i<MAX_REQ_LEN; i++){
			uv_mutex_lock(&cv_mutex[i]);
			if(reqid == req[i]){
				if(status[i] == CV_STAT_WAIT){
					uv_cond_signal(&cv[i]);
					uv_cond_wait(&notWaiting[i], &cv_mutex[i]);
				}
				else if(status[i] == CV_STAT_READY)
					wt = 1;
				req[i] = 0;
				res = 0;
				handlers[i] = NULL;
				cv_bitmap &= ~(0x01 << i);
				status[i] = CV_STAT_INIT;
			}
			uv_mutex_unlock(&cv_mutex[i]);
			if(res != -1){
				if(wt) cv_len--;
				break;
			}
		}
	}while(0);
	uv_mutex_unlock(&cv_bitmap_mutex);

	return res;
}
int cv_set::gc(void)
{
	int i, res = -1;
	uv_mutex_lock(&cv_bitmap_mutex);
	do{
		for(i=0; i<MAX_REQ_LEN; i++){
			uv_mutex_lock(&cv_mutex[i]);
			if(status[i] == CV_STAT_REM){
				req[i] = 0;
				res = 0;
				handlers[i] = NULL;
				cv_bitmap &= ~(0x01 <<i);
				status[i] = CV_STAT_INIT;
			}
			uv_mutex_unlock(&cv_mutex[i]);
		}
		if(res != -1)
			break;
	}while(0);
	uv_mutex_unlock(&cv_bitmap_mutex);

	return res;
}
int cv_set::search(uint32_t reqid, uint8_t *stat=NULL)
{
	int i, res = -1;
	uv_mutex_lock(&cv_bitmap_mutex);
	for(i=0; i<MAX_REQ_LEN; i++){
		uv_mutex_lock(&cv_mutex[i]);
		if(reqid == req[i]){
			res = i;
			if(NULL != stat){
				*stat = status[i];
			}
		}
		uv_mutex_unlock(&cv_mutex[i]);
		if(res != -1)
			break;
	}
	uv_mutex_unlock(&cv_bitmap_mutex);

	return res;
}
int cv_set::signal(uint32_t reqid)
{
	int i, res = -1;
	uv_mutex_lock(&cv_bitmap_mutex);
	for(i=0; i<MAX_REQ_LEN; i++){
		comm_log("signal lock");
		uv_mutex_lock(&cv_mutex[i]);
		comm_log("signal lock");
		comm_log("signal lock released");
		if(reqid == req[i]){
			res = i;
			uv_cond_signal(&cv[i]);
		}
		uv_mutex_unlock(&cv_mutex[i]);
		comm_log("signal lock released");
		if(res != -1)
			break;
	}
	uv_mutex_unlock(&cv_bitmap_mutex);
	return res;
}
int cv_set::getHandler(uint32_t reqid, Comm_Handler *handler_p)
{
	int i, res = -1;
	uv_mutex_lock(&cv_bitmap_mutex);
	for(i=0; i<MAX_REQ_LEN; i++){
		comm_log("handler lock");
		uv_mutex_lock(&cv_mutex[i]);
		comm_log("handler lock released");
		if(reqid == req[i]){
			res = i;
			*handler_p = handlers[i];
		}
		uv_mutex_unlock(&cv_mutex[i]);
		comm_log("handler lock released");
		if(res != -1)
			break;
	}
	uv_mutex_unlock(&cv_bitmap_mutex);
	return res;
}
int cv_set::wait(int i, uint32_t timeout)
{
	int res = -1, tmout = 0;
	if(i < 0 || i >= MAX_REQ_LEN)
		return res;

	uv_mutex_lock(&cv_mutex[i]);
	if(status[i] == CV_STAT_READY){
		res = 0;
		cv_len--;
		status[i] = CV_STAT_WAIT;
		tmout = uv_cond_timedwait(&cv[i], &cv_mutex[i], timeout*SECFROMNANO);
		comm_log("Wait done tmout:%d,pos:%d", tmout, i);
		if(tmout != 0){
			status[i] = CV_STAT_INIT;
			req[i] = 0;
			handlers[i] = NULL;
		}

		uv_cond_signal(&notWaiting[i]);
	}
	uv_mutex_unlock(&cv_mutex[i]);

	if(tmout != 0){
		res = 1;
		uv_mutex_lock(&cv_bitmap_mutex);
		cv_bitmap &=~(0x01<<i);
		uv_mutex_unlock(&cv_bitmap_mutex);
	}

	return res;
}

//When ack comes, enqueue to ack queue and signal to waiting thread.
//Timing for ack to come:
//Insert (Ready) -> Write -> (here) -> Wait (WAIT) -> (here) -> Timeout(REM) -> (here) -> Remove
int cv_set::sch_to_sig(uint32_t reqid, ANT_Comm_Queue *queue, queue_data_t *queue_data)
{
	int i, res = -1;

	uv_mutex_lock(&cv_bitmap_mutex);
	for(i=0; i<MAX_REQ_LEN; i++){
		uv_mutex_lock(&cv_mutex[i]);
		if(reqid == req[i]){
			res = -2;
			comm_log("Stat:%d", status[i]);
			if(CV_STAT_INIT != status[i] && CV_STAT_REM != status[i]){
				res = i;
				if(NULL != queue_data){
					comm_log("Handler Addr:%x", handlers[i]);
					queue_data->handler = handlers[i];
					if(NULL != queue){
						if(COMM_S_OK != queue->enqueue(queue_data))
							res = -1;
						else
							comm_log("queue inserted");
					}
				}
				if(CV_STAT_READY == status[i]){
					// NO need to wait but remove this cv
					cv_len--;
					status[i] = CV_STAT_INIT;
					req[i] = 0;
					handlers[i] = NULL;
					cv_bitmap &=~(0x01<<i);
				}
				else if(CV_STAT_WAIT == status[i]){
					//Normal Case
					comm_log("Hit!");
					status[i] = CV_STAT_INIT;
					req[i] = 0;
					handlers[i] = NULL;
					cv_bitmap &=~(0x01<<i);
					uv_cond_signal(&cv[i]);
				}
			}
			else if(CV_STAT_REM == status[i]){
				comm_log("This is what i expeceted");
			}
		}
		uv_mutex_unlock(&cv_mutex[i]);
		if(res != -1)
			break;
	}
	uv_mutex_unlock(&cv_bitmap_mutex);

	return res;
}
*/
//

/* ANT_Server Implementation */
ANT_Server::ANT_Server(const char *intf_name)
{

	int sock;
	int i;

	if(NULL == intf_name){
		comm_log("Null pointer error");
		return;
	}
	else
		comm_log("%s server opening...", intf_name);

	strncpy(this->intf_name, intf_name, MAX_NAME_LEN);

	server_handler = NULL;

	closing = 0;

	FD_ZERO(&readfds);
	max_fd = 0;
	num_threads = 0;

	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	bt_ops = new BT_Operations();
	if(NULL == bt_ops){
		comm_log("Memory allocation failed");
		return;
	}

	if (bt_ops->openBT(sock, this->intf_name) < 0){
		comm_log("Failed to initailze BT");
	}

	FD_SET(sock, &readfds);
	max_fd = sock;

	server_sock = new ANT_Socket(sock, CONNECTION_TYPE_BT);
	clients = new socket_set();
	//cvs = new cv_set();
	rqs = new req_set(&ack_queue);

	read_req.data = (void *)this;
	write_req.data = (void *)this;
	
	// NULL : Not used
	/*for(i=0; i<MAX_REQ_LEN; i++)
		ra_req[i].data = NULL;
		*/
	ra_req.data = (void *)this;

	write_thread_alive = false;
	read_thread_alive = false;
	ra_thread_alive = false;

	uv_queue_work(uv_default_loop(), &read_req,\
			generic_read_handler, after_read_handler);
	uv_queue_work(uv_default_loop(), &ra_req,\
			generic_ra_handler, after_ra_handler);


}

ANT_Server::ANT_Server(const char *intf_name, Comm_Handler serv_handler)
{

	int sock;
	int i;

	if(NULL == intf_name){
		comm_log("Null pointer error");
		return;
	}
	else
		comm_log("%s server opening...", intf_name);

	strncpy(this->intf_name, intf_name, MAX_NAME_LEN);

	this->server_handler = serv_handler;

	closing = 0;

	FD_ZERO(&readfds);
	max_fd = 0;
	num_threads = 0;

	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	bt_ops = new BT_Operations();
	if(NULL == bt_ops){
		comm_log("Memory allocation failed");
		return;
	}

	if (bt_ops->openBT(sock, this->intf_name) < 0){
		comm_log("Failed to initailze BT");
	}

	FD_SET(sock, &readfds);
	max_fd = sock;

	server_sock = new ANT_Socket(sock, CONNECTION_TYPE_BT);
	clients = new socket_set();
	//cvs = new cv_set();
	rqs = new req_set(&ack_queue);

	read_req.data = (void *)this;
	write_req.data = (void *)this;
	
	// NULL : Not used
	/*for(i=0; i<MAX_REQ_LEN; i++)
		ra_req[i].data = NULL;
		*/
	ra_req.data = (void *)this;

	write_thread_alive = false;
	read_thread_alive = false;
	ra_thread_alive = false;

	uv_queue_work(uv_default_loop(), &read_req,\
			generic_read_handler, after_read_handler);
	uv_queue_work(uv_default_loop(), &ra_req,\
			generic_ra_handler, after_ra_handler);
}



ANT_Server::~ANT_Server()
{
	uv_cancel((uv_req_t*)&read_req);	//Stop reading
	uv_cancel((uv_req_t*)&write_req);	//Stop writing

	if(NULL != server_sock)
		delete server_sock;
	if(NULL != bt_ops)
		delete bt_ops;
	if(NULL != clients)
		delete clients;
	/*if(NULL != cvs)
		delete cvs;
		*/
}

/* Phase 1: Select for if it is server or client
   Phase 2: Process server case
   Phase 3: Process client case
   Phase 3-1: Generic handling path
   Phase 3-2: Pass to write thread to handle the read (Ack)
 */
void ANT_Server::generic_read_handler(uv_work_t *req)
{
	int err_cli_pos = -1;
	static int first_listen = 0;
	ANT_Server *op_server = (ANT_Server *)req->data;
	op_server->read_thread_alive = true;
	op_server->num_threads++;
	comm_log("Read handler up %d", op_server->num_threads);
	if(!first_listen && 0 == op_server->clients->length()){
		comm_log("Listening ...");
		if ( listen(op_server->server_sock->get_sock_fd(), MAX_CLIENTS) < 0 ) {
			comm_log("listen failed");
			return;
		}
		first_listen = 1;
	}

	fd_set readfds = op_server->readfds;
	comm_log("Start Selecting server and clients");
	if(select(op_server->max_fd+1, &readfds, NULL, NULL, NULL) < 0){
		comm_log("select error:%s(%d)", strerror(errno), errno);
		return;
	}
	comm_log("Selecting server and clients");
	do{
		//If server sock has data, then accept new client
		if(FD_ISSET(op_server->server_sock->get_sock_fd(), &readfds)){
			struct sockaddr_rc rem_addr = { 0 };
			socklen_t opt = sizeof(rem_addr);
			char buf[BUFF_SIZE];
			int new_client_fd;

			new_client_fd = accept(op_server->server_sock->get_sock_fd(), (struct sockaddr *)&rem_addr, &opt);

			if(new_client_fd < 0){
				comm_log("Aceept failed");
				break;
			}
			if(op_server->clients->length() >= MAX_CLIENTS){
				comm_log("Too many clients try to connect to this server");
				close(new_client_fd);
				break;
			}


			ba2str(&rem_addr.rc_bdaddr, buf);
			comm_log("Accepted connection from %s, %d\n", buf, new_client_fd);
			memset(buf, 0, sizeof(buf));

			if(op_server->clients->insert(new_client_fd, CONNECTION_TYPE_BT) >= 0){
				FD_SET(new_client_fd, &op_server->readfds);
				if(op_server->max_fd < new_client_fd){
					comm_log("max fd(%d) -> %d", op_server->max_fd, new_client_fd);
					op_server->max_fd = new_client_fd;
				}
			}
			/* Greeting? */
			//write(new_client_fd, "hoho", 5);
		}
	}while(0);

	for (int i = 0; i < MAX_CLIENTS; i++){
		int rCount;
		int res;
		uint8_t buff[ANT_HEADER_SIZE];
		ANT_Socket *op_socket = NULL;
		ANT_Header *op_header = NULL;
		queue_data_t *queue_data = NULL;
		ANT_MSG *op_msg = NULL;
		uint16_t crc16val = 0;

		op_socket = op_server->clients->get(i);
		if(NULL == op_socket){
			continue;
		}

		res = FD_ISSET(op_socket->get_sock_fd(), &readfds);

		if( 0 == res ){
			comm_log("Not selected");
			continue;
		}
		else 
			comm_log("Selected");

		queue_data = new queue_data_t(op_socket);
		if(NULL == queue_data){
			comm_log("Memeory allocation failed");
			//exit(1);
		}
		else
			queue_data->attached++;

		comm_log("Initializing queue data to read...");
		op_msg = queue_data->op_msg;
		op_header = op_msg->get_header();

		comm_log("Start to read header");
		rCount = op_socket->Read(buff, ANT_HEADER_SIZE);
		comm_log("Read Done %d/%d ", rCount, ANT_HEADER_SIZE);

		if(rCount < 0){
			comm_log("Selected, but read error");
			queue_data->attached--;
			if(queue_data->attached==0) delete queue_data;
			else
				comm_log("tried to delete, but attached");

			err_cli_pos = i;
			goto READ_CLIENT_SOCKET_CLOSE;
		}
		else if(rCount == 0){
			comm_log("Socket close %d", i);
			queue_data->attached--;
			if(queue_data->attached==0) delete queue_data;
			else
				comm_log("tried to delete, but attached");

			err_cli_pos = i;
			goto READ_CLIENT_SOCKET_CLOSE;
		}

		comm_log("Initializing header...");
		if(COMM_S_OK != op_header->init_from_buff(buff)){
			comm_log("Failed to initialize header");
			queue_data->attached--;
			if(queue_data->attached==0) delete queue_data;
			else
				comm_log("tried to delete, but attached");

			err_cli_pos = i;
			goto READ_CLIENT_SOCKET_CLOSE;
		}
		comm_log("Initialization succedded");


		if(op_msg->is_file()){
			FILE *fp_tmp;
			uint8_t *data;
			char fname[256];

			if(MAX_DAT_LEN < op_msg->get_data_len()){
				comm_log("Received file data length is greater than MAX_DAT_LEN (%d > %d)", op_msg->get_data_len(), MAX_DAT_LEN);
				queue_data->attached--;
				if(queue_data->attached==0) delete queue_data;
				else
					comm_log("tried to delete, but attached");

				err_cli_pos = i;
				goto READ_CLIENT_SOCKET_CLOSE;
			}
			if(op_msg->get_data_len() > 0){
				data = (uint8_t *)malloc(op_msg->get_data_len());

				rCount = op_socket->Read(data, op_msg->get_data_len());


				if(rCount < 0){
					comm_log("read error");
					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("tried to delete, but attached");

					err_cli_pos = i;
					goto READ_CLIENT_SOCKET_CLOSE;
				}
				else{
					comm_log("Read done %d/%d", rCount, op_msg->get_data_len());
				}

			}
			else
				data = NULL;

			if(!op_msg->is_special()){//If it is the end-of-file, then nothing to do here but bring it to the ack queue later.
				int pathlen, cur;
				char *path;
				//Check!

				//Make new I/O thread?, In this version this just uses this thread to process I/O
				mkdir("./data", 0755);

				// File path parsing //Should be done at client part
				path = op_msg->get_file_name();
				pathlen = strlen(path);
				cur = pathlen;
				while(path[cur] != '/'){
					cur --;
					if(cur < 0)
						break;
				}
				cur++;

				comm_log("File name: %s(%d)\tParsed: %s", op_msg->get_file_name(), op_msg->get_file_offset(), &path[cur]);

				sprintf(fname, "./data/%s", &path[cur]);

				if(op_msg->get_file_offset() == 0)
					fp_tmp= fopen(fname, "w+");
				else
					fp_tmp = fopen(fname, "r+");

				if(NULL == fp_tmp){
					comm_log("%sm??File open error", &path[cur]);
					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("tried to delete, but attached");

					err_cli_pos = i;

					goto READ_CLIENT_SOCKET_CLOSE;

				}

				if(0 != fseek(fp_tmp, op_msg->get_file_offset(), SEEK_SET)){
					comm_log("FSEEK error:%s", fname);
					fclose(fp_tmp);

					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("tried to delete, but attached");

					err_cli_pos = i;

					goto READ_CLIENT_SOCKET_CLOSE;

				}

				if(op_msg->get_data_len()!= fwrite(data, sizeof(char),\
							op_msg->get_data_len(), fp_tmp)){
					comm_log("Fwrite error:%s", fname);
					fclose(fp_tmp);

					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("tried to delete, but attached");

					err_cli_pos = i;
					free(data);

					goto READ_CLIENT_SOCKET_CLOSE;

				}
				else{
					comm_log("fwrite succedded");
					fclose(fp_tmp);
					free(data);
					/*if(op_msg->is_ack()){
					  int ress;
					  comm_log("ack wait update");
					  if(0 <= (res=op_server->cvs->sch_to_sig(op_msg->get_req_id(), NULL, queue_data)))
					  op_server->cvs->insert(op_msg->get_req_id(), queue_data->handler);
					  else
					  comm_log("No wait this");
					  comm_log("ack wait update done(%d)",res);
					  }*/
				}
			}
			else {
				/* Special(Lst msg) */
				op_msg->set_data(data, op_msg->get_data_len());
			}
			{
				int pathlen, cur;
				char *path;
				char fname[256];
				char *tmp_msg = "File:";
				char *msg_data;

				if(op_msg->get_data() != NULL)
					comm_log("Special, but msg-piggy-backing not supproted");
				else{

					//File Path parsing 
					path = op_msg->get_file_name();
					pathlen = strlen(path);
					cur = pathlen;
					while(path[cur] != '/'){
						cur --;
						if(cur < 0)
							break;
					}
					cur++;

					sprintf(fname, "./data/%s", &path[cur]);
					comm_log("File name : %s(%d)", fname, op_msg->get_file_offset());

					msg_data = (char *)malloc(strlen(fname)+strlen(tmp_msg)+1);
					strcpy(msg_data, tmp_msg);
					strcat(msg_data, fname);

					op_msg->set_data((uint8_t *)msg_data, strlen(msg_data) +1);
					comm_log("%s(%d) received", msg_data, op_msg->get_file_offset());
					crc16val = crc16_ccitt((const void *)op_msg->get_data(), op_msg->get_data_len());
					if(op_msg->get_header()->chksum != crc16val){
						comm_log("Chksum error %d, %d", op_msg->get_header()->chksum, crc16val);
						//exit(1);
					}

				}
			}
		}
		else{
			uint8_t *data;
			if(MAX_MSG_LEN < op_msg->get_data_len()){
				comm_log("Received message length is greater than MAX_MSG_LEN (%d > %d)", \
						op_msg->get_data_len(), MAX_MSG_LEN);
				queue_data->attached--;
				if(queue_data->attached==0) delete queue_data;
				else
					comm_log("tried to delete, but attached");

				err_cli_pos = i;
				goto READ_CLIENT_SOCKET_CLOSE;
			}

			if(op_msg->get_data_len() > 0){
				data = (uint8_t *)malloc(op_msg->get_data_len());
				rCount = op_socket->Read(data, op_msg->get_data_len());

				if(rCount <= 0){
					if(rCount == 0)
						comm_log("EOF:Dis Connected");
					else comm_log("Read error");

					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("tried to delete, but attached");

					err_cli_pos = i;
					goto READ_CLIENT_SOCKET_CLOSE;
				}
				else
					comm_log("msg : %s received", (char *)data);
				op_msg->set_data(data, rCount);
				
				crc16val = crc16_ccitt((const void *)op_msg->get_data(), op_msg->get_data_len());
				if(op_msg->get_header()->chksum != crc16val){
					comm_log("Chksum error %d, %d", op_msg->get_header()->chksum, crc16val);
					//exit(1);
				}

			}

		}

	
		// Check the request ID for sync write

		// Case 1, 2: server handler, ack handler
		if(!op_msg->is_ack()){	// There's no write wating for this read.
			comm_log("This will be handled at default handler");
			if(NULL != op_server->server_handler){//case 1 : Process in user-defined server handler.
				comm_log("Server handler exists");
				int ins_err = (op_server->read_queue).enqueue(queue_data);
				if(ins_err != COMM_S_OK){
					if(queue_data->attached==0) delete queue_data;
					comm_log("Noop");
				}
				comm_log("Enqueued to read queue");


				continue;
			}
			comm_log("No server handler exists, droped the data");
		}
		else {
			comm_log("Ack Comes");

			if(op_msg->is_file() && !op_msg->is_special()){
				op_server->rqs->signal(op_msg->get_req_id(), queue_data, 1);
			}
			else
				op_server->rqs->signal(op_msg->get_req_id(), queue_data);
			
			queue_data->attached--;

			if(queue_data->attached==0) delete queue_data;


			/*
			   Ack comes, but no request wating.
			   Too late ack.
			 */
		}
	}

	return;

READ_CLIENT_SOCKET_CLOSE:
	comm_log("Read Client Socket Close Section:");
	if(err_cli_pos == -1){
		comm_log("No error client but jumped here?");
		return;
	}
	FD_CLR(op_server->clients->get(err_cli_pos)->get_sock_fd(), &(op_server->readfds));
	if(NULL != op_server->server_handler){
		queue_data_t *qdt = new queue_data_t();
		ANT_MSG *op_msg = qdt->op_msg;
		op_msg->set_err(SOCKET_ERR_DISCON);
		struct sockaddr_rc rem_addr = {0};
		socklen_t opt = sizeof(rem_addr);
		char *buf = (char *)malloc(BUFF_SIZE);

		getpeername(op_server->clients->get(err_cli_pos)->get_sock_fd(),\
				(struct sockaddr *)&rem_addr, &opt);
		ba2str(&rem_addr.rc_bdaddr, buf);
		strcat(buf, " is disconnected");
		op_msg->set_data((uint8_t *)buf, BUFF_SIZE);
		(op_server->read_queue).enqueue(qdt);
	}

	op_server->clients->remove(err_cli_pos);

	return;	
}

void ANT_Server::after_read_handler(uv_work_t *req, int status)
{
	ANT_Server *op_server = (ANT_Server *)req->data;
	op_server->read_thread_alive = false;
	op_server->num_threads--;
	comm_log("read handler down %d", op_server->num_threads);
	if(UV_ECANCELED == status)
		return;
	while(TRUE){
		queue_data_t *queue_data = NULL;
		if(NULL == op_server->server_sock)
			return;
		if(op_server->read_queue.isEmptyQueue() && !op_server->read_thread_alive){
			uv_queue_work(uv_default_loop(), &op_server->read_req, generic_read_handler, after_read_handler);
			break;
		}

		queue_data = op_server->read_queue.dequeue();

		if(NULL == queue_data)
			continue;
		else
			queue_data->attached++;

		if(NULL != op_server->server_handler)
			op_server->server_handler(queue_data->op_msg, queue_data->op_msg->get_err());

		queue_data->attached--;

		if(queue_data->attached==0){
			delete queue_data;
		}
	}

	return;
}

int ANT_Server::msg_write(IN const char *buf, IN int len,\
		IN ANT_MSG *req_msg /*=NULL*/,\
		IN Comm_Handler ack_msg_handler/*=NULL*/,\
		IN int cli_no/*=0*/,
		IN void *priv)
{
	queue_data_t *queue_data = NULL;
	ANT_Socket *op_socket = NULL;
	ANT_MSG *op_msg = NULL;
	bool empty = FALSE;
	uint8_t *data = NULL;


	if(NULL == buf){
		comm_log("empty buffer");
		return -COMM_E_POINTER;
	}

	if(len <= 0){
		comm_log("len should be > 0");
		return -COMM_E_INVALID_PARAM;
	}

	if(NULL == req_msg || NULL == req_msg->get_op_sock())
		op_socket = clients->get(cli_no);
	else
		op_socket = req_msg->get_op_sock();
	if(op_socket == NULL){
		comm_log("Invalid client");
		return -COMM_E_INVALID_PARAM;
	}

	/* Fill out MSG */

	queue_data = new queue_data_t(op_socket);
	op_msg = queue_data->op_msg;

	if(NULL == req_msg){
		if(0 == last_req_id)
			last_req_id++;
		op_msg->set_req_id(last_req_id++);
		op_msg->set_msg(NULL);
	}
	else{
		op_msg->set_req_id(req_msg->get_req_id());
		op_msg->set_msg(NULL);
		op_msg->set_ack();
	}

	data = (uint8_t *)malloc(len);
	if(NULL == data){
		comm_log("Malloc failed");
		delete(queue_data);
		return -COMM_E_POINTER;
	}
	memcpy(data, buf, len);
	op_msg->set_data(data, len);
	comm_log("set_Ack %d", op_msg->is_ack());
	op_msg->get_header()->chksum = crc16_ccitt((const void*)data, len);
	/*if(NULL != ack_msg_handler){
		cvs->insert(op_msg->get_req_id(), ack_msg_handler);
		int iter_ra;
		for(iter_ra=0; iter_ra<MAX_REQ_LEN; iter_ra++){
			if(ra_req[iter_ra].data == NULL){
				ra_req[iter_ra].data = this;
				break;
			}
		}
		if(MAX_REQ_LEN == iter_ra)
			comm_log("Plz I don't wanna see this");
		else
			uv_queue_work(uv_default_loop(), &ra_req[iter_ra], generic_ra_handler, after_ra_handler);
	}
	*/
	if(NULL != ack_msg_handler){
		queue_data_t *ack_data = new queue_data_t(op_socket);
		ANT_MSG *ack_msg = ack_data->op_msg;
		ack_msg->priv = priv;
		ack_data->handler = ack_msg_handler;
		ack_msg->set_req_id(op_msg->get_req_id());
		ack_msg->complete_header();
		
		comm_log("inserted %d", ack_msg->get_req_id());
		rqs->insert(ack_data);
	}

	queue_data->buff = (uint8_t *)malloc( sizeof(uint8_t) * (ANT_HEADER_SIZE + len) );

	op_msg->get_header()->init_to_buff(queue_data->buff);

	memcpy(queue_data->buff + ANT_HEADER_SIZE, buf, len);

	queue_data->buff_len = ANT_HEADER_SIZE + len;

	empty = write_queue.isEmptyQueue();


	write_queue.enqueue(queue_data);

	if(empty && !write_thread_alive){
		uv_queue_work(uv_default_loop(), &write_req, generic_write_handler, after_write_handler);
	}


	return COMM_S_OK;
}

int ANT_Server::file_write(IN const char *filePath, \
		IN ANT_MSG *req_msg/* = NULL*/,\
		IN Comm_Handler ack_file_handler/* = NULL*/,\
		IN int cli_no/* = 0*/,
		IN void *priv)
{
	queue_data_t *queue_data = NULL;
	ANT_Socket *op_socket = NULL;
	ANT_MSG *op_msg = NULL;
	//long readCount = 0;
	file_info_t finfo;
	FILE *fp_file;
	bool empty = NULL;
	//uint8_t *data = NULL;

	if(NULL == filePath)
	{
		comm_log("filePath NULL");
		return -COMM_E_INVALID_PARAM;
	}

	fp_file = fopen( filePath, "rb");
	if( NULL == fp_file){
		comm_log("File open error %s", filePath);
		return -COMM_E_INVALID_PARAM;
	}

	fseek(fp_file, 0, SEEK_END);
	finfo.fsize = ftell(fp_file);
	rewind(fp_file);
	finfo.offset = 0;
	fclose(fp_file);

	if(strlen(filePath) >= 24){
		comm_log("file Path name is too long");
		return -COMM_E_INVALID_PARAM;
	}
	memcpy(finfo.fname, filePath, strlen(filePath)+1);

	if(NULL == req_msg)
		op_socket = clients->get(cli_no);
	else
		op_socket = req_msg->get_op_sock();
	if(op_socket == NULL){
		comm_log("Invalid client");
		return -COMM_E_INVALID_PARAM;
	}

	/* Fill out File info and data */
	queue_data = new queue_data_t(op_socket);
	op_msg = queue_data->op_msg;

	if(NULL == req_msg){
		if(0 == last_req_id)
			last_req_id++;
		op_msg->set_req_id(last_req_id++);
		op_msg->set_file(&finfo);
	}
	else{
		op_msg->set_req_id(req_msg->get_req_id());
		op_msg->set_file(&finfo);
		op_msg->set_ack();
	}
	op_msg->complete_header();
	comm_log("File info set (size:%d/%d)", op_msg->get_file_size(), finfo.fsize);


	//	if(finfo.fsize < MAX_DAT_LEN){
	//		len = finfo.fsize;
	//	}
	//	else{
	//		len = MAX_DAT_LEN;
	//	}

	/*if(NULL != ack_file_handler){
		if(cvs->insert(op_msg->get_req_id(), ack_file_handler) < 0)
			comm_log("cvs insert error");
		int iter_ra;
		for(iter_ra=0; iter_ra<MAX_REQ_LEN; iter_ra++){
			if(ra_req[iter_ra].data == NULL){
				ra_req[iter_ra].data = this;
				break;
			}
		}
		if(MAX_REQ_LEN == iter_ra)
			comm_log("Plz I don't wanna see this");
		else
			uv_queue_work(uv_default_loop(), &ra_req[iter_ra], generic_ra_handler, after_ra_handler);
	}
	*/
	if(NULL != ack_file_handler){
		queue_data_t *ack_data = new queue_data_t(op_socket);
		ANT_MSG *ack_msg = ack_data->op_msg;
		ack_msg->priv = priv;
		ack_data->handler = ack_file_handler;
		ack_msg->set_req_id(op_msg->get_req_id());
		ack_msg->complete_header();
		comm_log("Inserted : %d,%d", ack_msg->get_req_id(), op_msg->get_req_id());
		rqs->insert(ack_data);
	}


	/*	data = (uint8_t *)malloc(sizeof(uint8_t) * len);
		queue_data->buff = (uint8_t *)malloc( sizeof(uint8_t) * (ANT_HEADER_SIZE + len) );
		op_msg->op_header->init_to_buff(queue_data->buff);

		while( (readCount < (op_header->len)) )
		{
		frval = fread(data+readCount, 1, \
		len - readCount, fp_file);
		if( ferror(fp_file) ) {
		comm_log("Error reading from %s", filePath);
		if(queue_data->attached==0) delete queue_data;
		else
		comm_log("tried to delete, but attached");

		return -COMM_E_FAIL;
		}
		readCount += frval;		
		if( feof(fp_file) ) {
		comm_log("Cannot happen, check the bug read Count %d, fSize %d", readCount, fSize);
		if(queue_data->attached==0) delete queue_data;
		else
		comm_log("tried to delete, but attached");

		return -COMM_E_FAIL;
		}
		}

		op_msg->set_data(data, len);
		memcpy(queue_data->buff+ANT_HEADER_SIZE, data, len);
		queue_data->buff_len = ANT_HEADER_SIZE + len;
	 */

	empty = write_queue.isEmptyQueue();
	write_queue.enqueue(queue_data);

	if(empty && !write_thread_alive)
		uv_queue_work(uv_default_loop(), &write_req, generic_write_handler, after_write_handler);


	return COMM_S_OK;
}

//Dequeue, and write work on a thread
void ANT_Server::generic_write_handler(uv_work_t *req)
{
	int err = SOCKET_ERR_NONE;
	queue_data_t *queue_data;
	ANT_MSG *op_msg;
	int wval;
	ANT_Socket *op_socket;
	int len;
	ANT_Server *op_server = (ANT_Server *)req->data;
	op_server->write_thread_alive = true;
//	cv_set *cvs = op_server->cvs;
	op_server->num_threads++;
	comm_log("write thread up %d", op_server->num_threads);

	queue_data = op_server->write_queue.dequeue();

	if(NULL == queue_data){
		comm_log("Dequeue failed");
		err = SOCKET_ERR_FAIL;
		goto WRITE_HANDLER_ERR;
	}
	else
		queue_data->attached++;
	op_msg = queue_data->op_msg;
	op_socket = op_msg->get_op_sock();

	if(NULL == op_socket){
		comm_log("NULL == opsocket");
		err = SOCKET_ERR_FAIL;
		queue_data->attached--;
		goto WRITE_HANDLER_ERR;
	}

	if(op_msg->is_file()){
		FILE *fp_file = NULL;
		int readCount = 0;

		len = op_msg->get_file_size() - op_msg->get_file_offset();
		if(len > MAX_DAT_LEN)
			len = MAX_DAT_LEN;

		if(len != 0){
			fp_file = fopen(op_msg->get_file_name(), "r");
			if(NULL == fp_file){
				err = SOCKET_ERR_FOPEN;
				queue_data->attached--;
				goto WRITE_HANDLER_ERR;
			}
		}
		else{
			comm_log("Last write gogo");
			op_msg->set_special();
		}

		op_msg->set_data(NULL, len);

		queue_data->buff = (uint8_t *)malloc( ANT_HEADER_SIZE + len);
	
		if(len != 0){
			uint8_t dat[MAX_DAT_LEN] = {0,};
			//readCount = fread(queue_data->buff+ANT_HEADER_SIZE, 1, len, fp_file);

			if(0 != fseek(fp_file, op_msg->get_file_offset(), SEEK_SET)){
				comm_log("FSeek error ");
				fclose(fp_file);
				queue_data->attached--;
				if(queue_data->attached == 0) delete queue_data;
				err = SOCKET_ERR_FOPEN;
				
				goto WRITE_HANDLER_ERR;
			}
			readCount = fread(dat, 1, len, fp_file);
			if( readCount < len ){
				err = SOCKET_ERR_FREAD;
				queue_data->attached--;
				goto WRITE_HANDLER_ERR;
			}

			fclose(fp_file);

			op_msg->get_header()->chksum = crc16_ccitt((const void *)dat, len);
			memcpy(queue_data->buff+ANT_HEADER_SIZE, dat, len);
		}

		queue_data->buff_len = ANT_HEADER_SIZE + len;
		op_msg->get_header()->init_to_buff(queue_data->buff);
	}
	comm_log("Writing to socket ... %s", op_msg->is_msg()? (char *)op_msg->get_data():"This is File");
	wval = op_socket->Write(queue_data->buff, queue_data->buff_len);
	comm_log("%d/%d written", wval, queue_data->buff_len);
	if(wval < (int)queue_data->buff_len){
		err = SOCKET_ERR_FAIL;
		queue_data->attached--;

		goto WRITE_HANDLER_ERR;
	}
	//If it's file, we need to refresh wait status of previous waiting thread.
	//And, we need to prepare next write.
	if(op_msg->is_file()){
		//Check whether next write exists or not
		if(op_msg->get_file_offset() < op_msg->get_file_size()) {
			//Make a queue data for next write
			queue_data_t *new_queue_data = new queue_data_t(op_socket);
			file_info_t finfo;
			ANT_MSG *new_msg = new_queue_data->op_msg;

			memcpy(finfo.fname, op_msg->get_file_name(), 24);
			finfo.fsize = op_msg->get_file_size();
			finfo.offset = op_msg->get_file_offset() + len;

			new_msg->set_req_id(op_msg->get_req_id());
			new_msg->set_file(&finfo);

			if(op_msg->is_ack()){
				new_msg->set_ack();
			}
			new_msg->complete_header();

			/*op_server->cvs->sch_to_sig(op_msg->get_req_id(), NULL, new_queue_data);
			if(0>op_server->cvs->insert(op_msg->get_req_id(), new_queue_data->handler)){
				comm_log("ReqId:%d, Hnadler:%x", op_msg->get_req_id(), new_queue_data->handler);
			}
			*/
			op_server->rqs->signal(op_msg->get_req_id(), NULL, 1);


			op_server->write_queue.enqueue(new_queue_data);
		}
	}
	queue_data->attached--;

	if(queue_data->attached==0)
		delete queue_data;

	return;

WRITE_HANDLER_ERR:
	op_msg->set_err(err);

	//cvs->sch_to_sig(op_msg->get_req_id(), &op_server->ack_queue, queue_data);
	op_server->rqs->signal(op_msg->get_req_id(), queue_data);
	return;
}

void ANT_Server::after_write_handler(uv_work_t *req, int status)
{
	ANT_Server *op_server = (ANT_Server *)req->data;
	op_server->write_thread_alive = false;
	//cv_set *cvs = op_server->cvs;
	op_server->num_threads--;
	comm_log("write thread down %d", op_server->num_threads);
	if(UV_ECANCELED == status)
		return;
	//comm_log("cvs_len:%d", cvs->getLen());
	/*if(cvs->getLen() > 0){
		int iter_ra;
		for(iter_ra=0; iter_ra<MAX_REQ_LEN; iter_ra++){
			if(op_server->ra_req[iter_ra].data == NULL){
				op_server->ra_req[iter_ra].data = op_server;
				break;
			}
		}
		if(MAX_REQ_LEN == iter_ra)
			comm_log("I don't wanna see this");
		else
			uv_queue_work(uv_default_loop(), &op_server->ra_req[iter_ra], generic_ra_handler, after_ra_handler);
	}
	*/
	if(!op_server->write_queue.isEmptyQueue() && !op_server->write_thread_alive){
		uv_queue_work(uv_default_loop(), &op_server->write_req, generic_write_handler, after_write_handler);
	}

	return;
}

void ANT_Server::generic_ra_handler(uv_work_t *req)
{
	ANT_Server *op_server = (ANT_Server *)req->data;
	op_server->ra_thread_alive = true;
	int res;
	op_server->num_threads++;
	comm_log("ra thread up %d", op_server->num_threads);

	while(TRUE){
		res = op_server->rqs->wait(MAX_MSG_TIMEOUT);
		if(res >= 0) //Timed Out data (already in ack queue) or someone inserted ack
			break;
	}
}

void ANT_Server::after_ra_handler(uv_work_t *req, int status)
{
	queue_data_t *queue_data;
	ANT_MSG *op_msg;
	ANT_Server *op_server = (ANT_Server *)req->data;
	op_server->ra_thread_alive = false;
	Comm_Handler server_handler = op_server->server_handler;
	op_server->num_threads--;
  	comm_log("ra thread down(%d),%d", status,op_server->num_threads);
	if(UV_ECANCELED == status){
		comm_log("Canceled");
		return;
	}
	while(TRUE){
		queue_data = op_server->ack_queue.dequeue();
		if(NULL == queue_data){
			if(!op_server->ra_thread_alive)
				uv_queue_work(uv_default_loop(), &op_server->ra_req, generic_ra_handler, after_ra_handler);
			break;
		}
		else{
			queue_data->attached++;
			op_msg = queue_data->op_msg;
			if(queue_data->handler)
				queue_data->call_handler();
			else if(NULL != server_handler)
				server_handler(op_msg, op_msg->get_err());
			else
				comm_log("ra error");
			
			queue_data->attached--;
			//if(op_msg->is_file() && !op_msg->is_special()){
			//	op_server->rqs->insert(queue_data);
			//}
			if(queue_data->attached==0)
				delete queue_data;

		}
	}
	/*
	  
	   if(op_server->num_threads < 0)
	   exit(1);

	   do{
	   if(UV_ECANCELED == status){
	   comm_log("ra error");
	   break;
	   }

	   queue_data = op_server->ack_queue.dequeue();
	   if(NULL == queue_data){
	   comm_log("ra no queue data:Writing? OK!");
	   break;
	   }

	   op_msg = queue_data->op_msg;

	   if(queue_data->handler)
	   queue_data->call_handler();
	   else if(NULL != server_handler)
	   server_handler(op_msg, op_msg->get_err());
	   else
	   comm_log("ra error");

	   delete queue_data;
	   }while(0);

	   req->data =NULL;
	   comm_log("ra data NULL");
	 */

	return;
}

void ANT_Server::SetServerHandler(IN Comm_Handler serv_handler)
{
	server_handler = serv_handler;
}
//

/* ANT_Client Implementation */
ANT_Client::ANT_Client(const char *intf_name, Comm_Handler onConnect)
{
	int i,sock;

	if(NULL == intf_name){
		comm_log("NULL pointer error");
		return;
	}

	strncpy(this->intf_name, intf_name,MAX_NAME_LEN);

	client_handler = NULL;
	this->onConnect = onConnect;

	closing = 0;
	max_fd = 0;
	num_threads = 0;

	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	bt_ops = new BT_Operations();
	if(NULL == bt_ops){
		comm_log("Memory allocation failed");
		return;
	}

	connected = FALSE;

	serv_sock = new ANT_Socket(sock, CONNECTION_TYPE_BT);
//	cvs = new cv_set();
	rqs = new req_set(&ack_queue);

	connect_req.data = (void *)this;
	write_req.data = (void *)this;
	read_req.data = (void *)this;
	/*for(i=0; i<MAX_REQ_LEN; i++)
		ra_req[i].data = NULL;
		*/
	ra_req.data = (void *)this;

	write_thread_alive = false;
	read_thread_alive = false;
	ra_thread_alive = false;

	uv_queue_work(uv_default_loop(), &connect_req,\
			generic_connect_handler, after_connect_handler);
}
ANT_Client::ANT_Client(const char *intf_name, Comm_Handler client_handler,Comm_Handler onConnect)
{
	int i,sock;

	if(NULL == intf_name){
		comm_log("NULL pointer error");
		return;
	}

	strncpy(this->intf_name, intf_name,MAX_NAME_LEN);

	this->client_handler = client_handler;
	this->onConnect = onConnect;


	closing = 0;
	max_fd = 0;
	num_threads = 0;

	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(sock < 0)
		comm_log("Socket init failed");
	else
		comm_log("Socket generated : %d", sock);

	bt_ops = new BT_Operations();
	if(NULL == bt_ops){
		comm_log("Memory allocation failed");
		return;
	}

	connected = FALSE;

	serv_sock = new ANT_Socket(sock, CONNECTION_TYPE_BT);
	//cvs = new cv_set();
	rqs = new req_set(&ack_queue);

	connect_req.data = (void *)this;
	write_req.data = (void *)this;
	read_req.data = (void *)this;
	
	/*for(i=0; i<MAX_REQ_LEN; i++)
		ra_req[i].data = NULL;
		*/
	ra_req.data = (void *)this;

	write_thread_alive = false;
	read_thread_alive = false;
	ra_thread_alive = false;

	connection = CLI_DISCONNECTED;

	uv_queue_work(uv_default_loop(), &connect_req,\
			generic_connect_handler, after_connect_handler);
}

ANT_Client::~ANT_Client()
{
	uv_cancel((uv_req_t *)&read_req);
	uv_cancel((uv_req_t *)&write_req);

	if(NULL != serv_sock){
		delete serv_sock;
	}
	if(NULL != bt_ops)
		delete bt_ops;
	//if(NULL != cvs)
		//delete cvs;
	if(NULL != rqs)
		delete rqs;
}




void ANT_Client::generic_connect_handler(uv_work_t *req)
{
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->connection = CLI_CONNECTING;
	BT_Operations *bt_ops = op_client->bt_ops;
	comm_log("Connect thread Up %d", ++op_client->num_threads);
	char *intf_name = op_client->intf_name;
	int err = bt_ops->connectBT(intf_name, op_client->serv_sock->get_sock_fd());

	op_client->connect_err = err;
}
void ANT_Client::after_connect_handler(uv_work_t *req, int status)
{
	ANT_Client *op_client = (ANT_Client *)req->data;
	int err = op_client->connect_err;

	comm_log("Connect thread down %d", --op_client->num_threads);


	if(COMM_S_OK == err){
		op_client->connection = CLI_CONNECTED;
		struct sockaddr_rc rem_addr = {0};
		socklen_t opt = sizeof(rem_addr);
		char buf[BUFF_SIZE];
		getpeername(op_client->serv_sock->get_sock_fd(), \
				(struct sockaddr *)&rem_addr, &opt);
		ba2str(&rem_addr.rc_bdaddr, buf);

		comm_log("Connected! %d:%s", op_client->serv_sock->get_sock_fd(), buf);
		FD_SET(op_client->serv_sock->get_sock_fd(), &op_client->readfds);
		op_client->max_fd = op_client->serv_sock->get_sock_fd();
		op_client->serv_sock->get();
		op_client->connected = TRUE;

		if(!op_client->read_thread_alive)
			uv_queue_work(uv_default_loop(), &op_client->read_req,\
					generic_read_handler, \
					after_read_handler);
		if(!op_client->ra_thread_alive)
			uv_queue_work(uv_default_loop(), &op_client->ra_req,\
					generic_ra_handler,\
					after_ra_handler);
	}
	else{
		comm_log("Connect failed : %d", err);
		op_client->connection = CLI_DISCONNECTED;
	}

	if(NULL != op_client->onConnect){
		op_client->onConnect(NULL, err);
	}
}
void ANT_Client::generic_read_handler(uv_work_t *req)
{
	int err = SOCKET_ERR_NONE;
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->read_thread_alive = true;
	ANT_Socket *serv_sock = op_client->serv_sock;
	uint16_t crc16val = 0;
	op_client->num_threads++;
	comm_log("Read Thread generated %d", op_client->num_threads);
	fd_set readfds = op_client->readfds;

	do{
		if(select(op_client->max_fd+1, &readfds, NULL, NULL, NULL) < 0){
			comm_log("select error");
			err = SOCKET_ERR_FAIL;
			break;				
		}

		if(NULL == serv_sock){
			comm_log("No serv_sock allocated in read handler?");
			err = SOCKET_ERR_FAIL;
			break;
		}

		if(FD_ISSET(serv_sock->get_sock_fd(), &readfds)){
			int rCount;
			uint8_t buff[ANT_HEADER_SIZE];
			ANT_Header *op_header = NULL;
			queue_data_t *queue_data = NULL;
			ANT_MSG *op_msg = NULL;

			comm_log("selected");

			queue_data = new queue_data_t(serv_sock);
			if(NULL == queue_data){
				comm_log("Memory allocation failed");
				err = SOCKET_ERR_FAIL;
				break;
			}
			else
				queue_data->attached++;

			op_msg = queue_data->op_msg;
			op_header = op_msg->get_header();
			rCount = serv_sock->Read(buff, ANT_HEADER_SIZE);

			if(rCount != ANT_HEADER_SIZE){
				if(rCount == 0){
					comm_log("Disconnected");
					queue_data->attached--;
					if(queue_data->attached==0)
						delete queue_data;
					err = SOCKET_ERR_DISCON;
					break;
				}
				else{
					comm_log("Read error %d/%d", rCount, ANT_HEADER_SIZE);

					queue_data->attached--;
					if(queue_data->attached==0)
						delete queue_data;
					comm_log("deleted?");
					err = SOCKET_ERR_FAIL;
					break;
				}
			}
			else
				comm_log("Read done %d/%d", rCount, ANT_HEADER_SIZE);
			comm_log("Init from header...");

			if(COMM_S_OK != op_header->init_from_buff(buff)){
				comm_log("Failed to initialized header");
					
				queue_data->attached--;
				if(queue_data->attached==0) delete queue_data;
				else
					comm_log("Tried to delete, but attached");

				err = SOCKET_ERR_FAIL;
				break;
			}

			comm_log("Succeeded initialization of header....");
			if(op_msg->is_file()){
				FILE *fp_tmp;
				uint8_t *data;
				char fname[256];
				comm_log("It's file");

				if(MAX_DAT_LEN < op_msg->get_data_len()){
					comm_log("Received file data length is greater than MAX_DAT_LEN (%d > %d)",\
							op_msg->get_data_len(), MAX_DAT_LEN);

					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("Tried to delete, but attached");
					err = SOCKET_ERR_FAIL;
					break;
				}
				comm_log("Read data:%d", op_msg->get_data_len());
				if(op_msg->get_data_len() > 0){
					data = (uint8_t *)malloc(op_msg->get_data_len());
					rCount = serv_sock->Read(data, op_msg->get_data_len());


					if(rCount != (int)op_msg->get_data_len()){
						if(rCount == 0){
							comm_log("Disconnected");
							err = SOCKET_ERR_DISCON;
							queue_data->attached--;
							if(queue_data->attached==0) delete queue_data;
							break;
						}
						else{
							comm_log("Read error%d/%d", rCount, op_msg->get_data_len());
							err = SOCKET_ERR_FAIL;
							queue_data->attached--;
							if(queue_data->attached==0) delete queue_data;
							break;
						}
					}
					else{
						comm_log("File read %d/%d", rCount, op_msg->get_data_len());
					}
				}
				else 
					data = NULL;

				if(!op_msg->is_special()){
					int pathlen, cur;
					char *path;

					mkdir("./data", 0755);

					// File path parsing //Should be done at client part
					path = op_msg->get_file_name();
					pathlen = strlen(path);
					cur = pathlen;
					while(path[cur] != '/'){
						cur --;
						if(cur < 0)
							break;
					}
					cur++;

					sprintf(fname, "./data/%s", &path[cur]);

					if(op_msg->get_file_offset() == 0)
						fp_tmp = fopen(fname, "w+");
					else
						fp_tmp = fopen(fname, "r+");
					if(NULL == fp_tmp){
						comm_log("File open error");
						queue_data->attached--;
						if(queue_data->attached==0) delete queue_data;
						else
							comm_log("Tried to delete, but attached");
						err = SOCKET_ERR_FOPEN;
						break;
					}

					if(0 != fseek(fp_tmp, op_msg->get_file_offset(), SEEK_SET)){
						comm_log("FSEEk error : %s", fname);
						fclose(fp_tmp);
						queue_data->attached--;
						if(queue_data->attached==0) delete queue_data;
						else
							comm_log("tried to delete, but attached");

						err = SOCKET_ERR_FOPEN;
						break;
					}

					if(op_msg->get_data_len() != fwrite(data, sizeof(uint8_t), \
								op_msg->get_data_len(), fp_tmp)){
						comm_log("fwrite error :%s", fname);
						fclose(fp_tmp);
					queue_data->attached--;
						if(queue_data->attached==0) delete queue_data;
						else
							comm_log("Tried to delete, but attached");

						err = SOCKET_ERR_FREAD;
						free(data);
						break;
					}
					else{
						comm_log("fwrite succedded");
						fclose(fp_tmp);
						err = SOCKET_ERR_NONE;
						free(data);
					}
				}
				else{
					op_msg->set_data(data, op_msg->get_data_len());
				}
				{//It's last
					int pathlen, cur;
					char *path;
					char fname[256];
					char *tmp_msg = "File:";
					char *msg_data;

					if(NULL != op_msg->get_data())
						comm_log("Special, but msg-piggy-backing not supported");
					else{
						// File path parsing //Should be done at client part
						path = op_msg->get_file_name();
						pathlen = strlen(path);
						cur = pathlen;
						while(path[cur] != '/'){
							cur --;
							if(cur < 0)
								break;
						}
						cur++;

						sprintf(fname, "./data/%s", &path[cur]);
						comm_log("File name : %s(%d)", fname, op_msg->get_file_offset());

						msg_data = (char *)malloc(strlen(fname)+strlen(tmp_msg)+1);
						strcpy(msg_data, tmp_msg);
						strcat(msg_data, fname);

						op_msg->set_data((uint8_t *)msg_data, strlen(msg_data)+1);
						comm_log("%s received", msg_data);
						crc16val = crc16_ccitt((const void *)data, op_msg->get_data_len());
						if(op_msg->get_header()->chksum != crc16val){
							comm_log("Chksum error %d, %d", op_msg->get_header()->chksum, crc16val);
							//exit(1);
						}

					}
				}
			}
			else{
				uint8_t *data;
				if(MAX_MSG_LEN < op_msg->get_data_len()){
					comm_log("Received message length is greater than MAX_MSG_LEN");
					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("Tried to delete, but attached");

					err = SOCKET_ERR_FAIL;
					break;
				}

				if(op_msg->get_data_len() > 0){
					data = (uint8_t *)malloc(op_msg->get_data_len());
					rCount = serv_sock->Read(data, op_msg->get_data_len());

					if(rCount != (int)op_msg->get_data_len()){
						if(rCount == 0){
							comm_log("Disconnected");

							err = SOCKET_ERR_DISCON;
							queue_data->attached--;
							if(queue_data->attached==0) delete queue_data;
							break;
						}
						else{
							comm_log("");

							err = SOCKET_ERR_FAIL;
							queue_data->attached--;
							if(queue_data->attached==0) delete queue_data;
							break;
						}
					}
					else{
						op_msg->set_data(data, op_msg->get_data_len());
						comm_log("%s MSG received", (char *)op_msg->get_data());
					}
				}
			}

			comm_log("isAck()?%d", op_msg->is_ack());
			if(!op_msg->is_ack()){
				if(NULL != op_client->client_handler){
					int ins_err = op_client->read_queue.enqueue(queue_data);
					if(COMM_S_OK != ins_err)
						err = ins_err;
				}
			}
			else {
				/*
				   int req_err = op_client->cvs->sch_to_sig(op_msg->get_req_id(), &op_client->ack_queue, queue_data);
				 */

				op_client->rqs->signal(op_msg->get_req_id(), queue_data);
				comm_log("Ack comes:%s", op_msg->get_data());
				if(op_msg->is_file() && !op_msg->is_special()){
					op_client->rqs->insert(queue_data);
				}

				/*
				   if(req_err < 0){
				   if(queue_data->attached==0) delete queue_data;
				   else
				   comm_log("Tried to delete, but attached");

				//err = SOCKET_ERR_FAIL;

				break;
				}
				 */
			}
			if(NULL == queue_data)
				comm_log("??");
			queue_data->attached--;
			if(queue_data->attached==0) delete queue_data;

		}
		else{
			comm_log("Failed handling this case");
			err = SOCKET_ERR_FAIL;
		}

	}while(0);

	if(SOCKET_ERR_NONE != err){
		FD_CLR(op_client->serv_sock->get_sock_fd(), &op_client->readfds);
		op_client->connected = 0;
		queue_data_t *qdt = new queue_data_t();
		ANT_MSG *op_msg = qdt->op_msg;
		op_msg->set_err(SOCKET_ERR_DISCON);
		struct sockaddr_rc rem_addr = {0};
		socklen_t opt = sizeof(rem_addr);
		char *buf = (char *)malloc(BUFF_SIZE);

		op_msg->set_data((uint8_t *)buf, BUFF_SIZE);
		comm_log("Euqnueing %x", qdt);
		op_client->read_queue.enqueue(qdt);

	}
}

void ANT_Client::after_read_handler(uv_work_t *req, int status)
{
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->read_thread_alive = false;
	ANT_Socket *serv_sock = op_client->serv_sock;
	op_client->num_threads--;
	comm_log("Read thread down:%d", op_client->num_threads);
	if(UV_ECANCELED == status)
		return;

	while(TRUE){
		queue_data_t *queue_data = NULL;
		if(NULL == serv_sock)
			return;
		if(op_client->read_queue.isEmptyQueue() && !op_client->read_thread_alive){
			uv_queue_work(uv_default_loop(), &op_client->read_req, generic_read_handler, after_read_handler);
			break;
		}

		queue_data = op_client->read_queue.dequeue();
		if(NULL == queue_data)
			continue;
		else
			queue_data->attached++;

		if(NULL != op_client->client_handler)
			op_client->client_handler(queue_data->op_msg, queue_data->op_msg->get_err());

		queue_data->attached--;

		if(queue_data->attached==0){
			delete queue_data;
		}
		else
			comm_log("%dTried to delete, but attached", queue_data->attached);
	}

	return;
}

int ANT_Client::msg_write(IN const char *buf, IN int len,\
		IN ANT_MSG *req_msg /*=NULL*/,\
		IN Comm_Handler ack_msg_handler/* = NULL*/)
{
	queue_data_t *queue_data = NULL;
	ANT_Socket *op_socket = NULL;
	ANT_MSG *op_msg = NULL;
	bool empty = FALSE;
	uint8_t *data = NULL;

	if(NULL == buf){
		comm_log("empty buffer");
		return -COMM_E_POINTER;
	}

	if(len <= 0){
		comm_log("len should be > 0");
		return -COMM_E_INVALID_PARAM;
	}

	op_socket = serv_sock;
	if(op_socket == NULL){
		comm_log("Invalid client");
		return -COMM_E_INVALID_PARAM;
	}

	queue_data = new queue_data_t(op_socket);
	op_msg = queue_data->op_msg;

	if(NULL == req_msg){
		if(0 == last_req_id)
			last_req_id++;
		op_msg->set_req_id(last_req_id++);
		op_msg->set_msg(NULL);
	}
	else{
		op_msg->set_req_id(req_msg->get_req_id());
		op_msg->set_msg(NULL);
		op_msg->set_ack();
	}

	data = (uint8_t *)malloc(len);
	if(NULL == data){
		comm_log("Malloc failed");
		delete(queue_data);
		return -COMM_E_POINTER;
	}
	memcpy(data, buf, len);
	op_msg->set_data(data, len);
	op_msg->get_header()->chksum = crc16_ccitt((const void *)data, len);

	/*if(NULL != ack_msg_handler){
		cvs->insert(op_msg->get_req_id(), ack_msg_handler);
		int iter_ra;
		for(iter_ra=0; iter_ra<MAX_REQ_LEN; iter_ra++){
			if(ra_req[iter_ra].data == NULL){
				ra_req[iter_ra].data = this;
				break;
			}
		}
		if(iter_ra == MAX_REQ_LEN)
			comm_log("Plz... I don't wanna see this");
		else
			uv_queue_work(uv_default_loop(), &ra_req[iter_ra], generic_ra_handler, after_ra_handler);
	}
	*/
	if(NULL != ack_msg_handler){
		queue_data_t *ack_data = new queue_data_t(op_socket);
		ANT_MSG *ack_msg = ack_data->op_msg;
		ack_data->handler = ack_msg_handler;
		ack_msg->set_req_id(op_msg->get_req_id());
		ack_msg->complete_header();
		comm_log("Inserted : %d,%d", ack_msg->get_req_id(), op_msg->get_req_id());
		rqs->insert(ack_data);
	}

	queue_data->buff = (uint8_t *)malloc(sizeof(uint8_t) * (ANT_HEADER_SIZE + len));
	op_msg->get_header()->init_to_buff(queue_data->buff);
	memcpy(queue_data->buff+ANT_HEADER_SIZE, buf, len);
	queue_data->buff_len = ANT_HEADER_SIZE + len;
	empty = write_queue.isEmptyQueue();

	write_queue.enqueue(queue_data);
	if(empty && !write_thread_alive){
		comm_log("Write thread gogo");
		uv_queue_work(uv_default_loop(), &write_req, generic_write_handler, after_write_handler);
	}
	return COMM_S_OK;
}

int ANT_Client::file_write(IN const char *filePath, \
		IN ANT_MSG *req_msg/* = NULL*/,\
		IN Comm_Handler ack_file_handler/* = NULL*/)
{
	queue_data_t *queue_data = NULL;
	ANT_Socket *op_socket = NULL;
	ANT_MSG *op_msg = NULL;
	file_info_t finfo;
	FILE *fp_file;
	bool empty = NULL;

	if(NULL == filePath){
		comm_log("filePath NULL");
		return -COMM_E_INVALID_PARAM;
	}

	fp_file = fopen(filePath, "r");
	if(NULL == fp_file){
		comm_log("File Open error %s", filePath);
		return -COMM_E_INVALID_PARAM;
	}

	fseek(fp_file, 0, SEEK_END);
	finfo.fsize = ftell(fp_file);
	rewind(fp_file);
	finfo.offset = 0;
	fclose(fp_file);

	if(strlen(filePath) >= 24){
		comm_log("file Path name is too long");
		return -COMM_E_INVALID_PARAM;
	}
	memcpy(finfo.fname, filePath, strlen(filePath)+1);

	op_socket = serv_sock;
	if(op_socket == NULL){
		comm_log("SErv_sock!!!");
		return -COMM_E_INVALID_PARAM;
	}

	queue_data = new queue_data_t(op_socket);
	op_msg = queue_data->op_msg;

	if(NULL == req_msg){
		if(0 == last_req_id)
			last_req_id++;
		op_msg->set_req_id(last_req_id++);
		op_msg->set_file(&finfo);
	}
	else{
		op_msg->set_req_id(req_msg->get_req_id());
		op_msg->set_file(&finfo);
		op_msg->set_ack();
	}
	op_msg->complete_header();
	comm_log("File info set:%s/%s (size:%d/%d)", op_msg->get_file_name(), filePath,op_msg->get_file_size(), finfo.fsize);

	/*if(NULL != ack_file_handler){
		if(cvs->insert(op_msg->get_req_id(), ack_file_handler) < 0)
			comm_log("cvs insert error");
		int iter_ra;
		for(iter_ra=0; iter_ra<MAX_REQ_LEN; iter_ra++){
			if(ra_req[iter_ra].data == NULL){
				ra_req[iter_ra].data = this;
				break;
			}
		}
		if(iter_ra == MAX_REQ_LEN)
			comm_log("Plz I don't wanna see this");
		else
			uv_queue_work(uv_default_loop(), &ra_req[iter_ra], generic_ra_handler, after_ra_handler);
	}
	*/
	if(NULL != ack_file_handler){
		queue_data_t *ack_data = new queue_data_t(op_socket);
		ANT_MSG *ack_msg = ack_data->op_msg;
		ack_data->handler = ack_file_handler;
		ack_msg->set_req_id(op_msg->get_req_id());
		ack_msg->complete_header();

		comm_log("Inserted : %d,%d", ack_msg->get_req_id(), op_msg->get_req_id());
		rqs->insert(ack_data);
	}

	empty = write_queue.isEmptyQueue();
	write_queue.enqueue(queue_data);

	if(empty && !write_thread_alive)
		uv_queue_work(uv_default_loop(), &write_req, generic_write_handler, after_write_handler);

	return COMM_S_OK;
}

void ANT_Client::generic_write_handler(uv_work_t *req)
{
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->write_thread_alive =true;
	int err = SOCKET_ERR_NONE;
	queue_data_t *queue_data;
	ANT_MSG *op_msg;
	int wval;
	ANT_Socket *op_socket, *serv_sock = op_client->serv_sock;
	//cv_set *cvs = op_client->cvs;
	uint32_t len;
	op_client->num_threads++;
	comm_log("write thread up %d", op_client->num_threads);

	do{
		queue_data = op_client->write_queue.dequeue();
		if(NULL == queue_data){
			comm_log("Dequeue failed");
			err = SOCKET_ERR_FAIL;
			break;
		}
		else{
			queue_data->attached++;
			comm_log("Dequeue succedded");
		}


		op_msg = queue_data->op_msg;
		op_socket = serv_sock;
		if(NULL == serv_sock){
			err = SOCKET_ERR_FAIL;
			queue_data->attached--;
			break;
		}
		if(NULL == op_msg){
			comm_log("MSG null");
			err = SOCKET_ERR_FAIL;
			queue_data->attached--;
			break;
		}

		if(op_msg->is_file()){
			FILE *fp_file = NULL;
			int readCount = 0;

			comm_log("It's file");

			len = op_msg->get_file_size() - op_msg->get_file_offset();
			if(len > MAX_DAT_LEN)
				len = MAX_DAT_LEN;

			if(len != 0){
				fp_file = fopen(op_msg->get_file_name(), "rb");
				if(NULL == fp_file){
					err = SOCKET_ERR_FOPEN;
					queue_data->attached--;
					break;
				}
			}
			else{
				comm_log("Last write gogo");
				op_msg->set_special();
			}

			op_msg->set_data(NULL, len);

			queue_data->buff = (uint8_t *)malloc(ANT_HEADER_SIZE + len);
			queue_data->buff_len = ANT_HEADER_SIZE + len;
			op_msg->get_header()->init_to_buff(queue_data->buff);

			if(len != 0){
				uint8_t dat[MAX_DAT_LEN] = {0,};
				//readCount = fread(queue_data->buff+ANT_HEADER_SIZE, 1, len, fp_file);
				if(0 != fseek(fp_file, op_msg->get_file_offset(), SEEK_SET)){
					comm_log("FSEEk error ");
					fclose(fp_file);
					queue_data->attached--;
					if(queue_data->attached==0) delete queue_data;
					else
						comm_log("tried to delete, but attached");

					err = SOCKET_ERR_FOPEN;
					break;
				}

				readCount = fread(dat, 1, len, fp_file);
				if( readCount < (int)len){
					err = SOCKET_ERR_FREAD;
					comm_log("Fread failed");
					queue_data->attached--;
					break;
				}
				fclose(fp_file);

				op_msg->get_header()->chksum = crc16_ccitt((const void *)dat, len);
				memcpy(queue_data->buff+ANT_HEADER_SIZE, dat, len);
			}
		}

		comm_log("Writing to socket ... %s", op_msg->is_msg()? (char *)op_msg->get_data():"This is File");
		wval = op_socket->Write(queue_data->buff, queue_data->buff_len);
		comm_log("%d / %d written", wval, queue_data->buff_len);
		if(wval <(int)queue_data->buff_len){
			err = SOCKET_ERR_FAIL;
			queue_data->attached--;
			break;
		}

		if(op_msg->is_file()){
			if(op_msg->get_file_offset() < op_msg->get_file_size()){
				queue_data_t *new_queue_data = new queue_data_t(op_socket);
				file_info_t finfo;
				ANT_MSG *new_msg = new_queue_data->op_msg;

				memcpy(finfo.fname, op_msg->get_file_name(), 24);
				finfo.fsize = op_msg->get_file_size();
				finfo.offset = op_msg->get_file_offset() + len;

				new_msg->set_req_id(op_msg->get_req_id());
				new_msg->set_file(&finfo);

				if(op_msg->is_ack()){
					new_msg->set_ack();
				}
				new_msg->complete_header();

				/*

				cvs->sch_to_sig(op_msg->get_req_id(), NULL, new_queue_data);
				cvs->insert(op_msg->get_req_id(), new_queue_data->handler);
				*/
				op_client->rqs->signal(op_msg->get_req_id(), NULL, 1);

				op_client->write_queue.enqueue(new_queue_data);
			}
		}

		queue_data->attached--;
		if(queue_data->attached==0) delete queue_data;
		else
			comm_log("Tried to delet, but attached");

	}while(0);

	if(err != SOCKET_ERR_NONE){
		op_msg->set_err(err);
		//cvs->sch_to_sig(op_msg->get_req_id(), &op_client->ack_queue, queue_data);
		op_client->rqs->signal(op_msg->get_req_id(), queue_data);
	}
}

void ANT_Client::after_write_handler(uv_work_t *req, int status)
{
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->write_thread_alive = false;
	op_client->num_threads--;
	comm_log("write thread down %d", op_client->num_threads);
	if(UV_ECANCELED == status)
		return;
/*
	if(op_client->cvs->getLen() > 0){
		int iter_ra;
		for(iter_ra=0; iter_ra<MAX_REQ_LEN; iter_ra++){
			if(op_client->ra_req[iter_ra].data == NULL){
				op_client->ra_req[iter_ra].data = op_client;
				break;
			}
		}
		if(iter_ra == MAX_REQ_LEN)
			comm_log("plz I don't wanna see this");
		else
			uv_queue_work(uv_default_loop(), &op_client->ra_req[iter_ra], generic_ra_handler, after_ra_handler);
	}
	*/
	if(op_client->write_queue.isEmptyQueue() == FALSE && !op_client->write_thread_alive)
		uv_queue_work(uv_default_loop(), &op_client->write_req, generic_write_handler, after_write_handler);
}

void ANT_Client::generic_ra_handler(uv_work_t *req)
{
	int i, res = -1;
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->ra_thread_alive = true;
	//cv_set *cvs = op_client->cvs;
	op_client->num_threads++;
	comm_log("ra thread up %d", op_client->num_threads);

	while(TRUE){
		res = op_client->rqs->wait(MAX_MSG_TIMEOUT);
		if(res >= 0)
			break;
	}
/*
	for(i=0; i<MAX_REQ_LEN; i++){
		res = cvs->wait(i, MAX_FILE_TIMEOUT);
		if(res >= 0)
			break;
	}

	comm_log("ra wait done:%d", res);

	comm_log("ra gc done");
	*/

}
void ANT_Client::after_ra_handler(uv_work_t *req, int status)
{
	ANT_Client *op_client = (ANT_Client *)req->data;
	op_client->ra_thread_alive = false;
	queue_data_t *queue_data;
	ANT_MSG *op_msg;

	op_client->num_threads--;
	comm_log("ra thread down %d", op_client->num_threads);

	if(UV_ECANCELED == status){
		comm_log("Canceled");
		return;
	}

	while(TRUE){
		queue_data = op_client->ack_queue.dequeue();
		if(NULL == queue_data && !op_client->ra_thread_alive){
			uv_queue_work(uv_default_loop(), &op_client->ra_req, generic_ra_handler, after_ra_handler);
			break;
		}
		else{
			queue_data->attached++;
			op_msg = queue_data->op_msg;
			comm_log("%s Ack Comes", op_msg->get_data());
			if(queue_data->handler)
				queue_data->call_handler();
			else if(NULL != op_client->client_handler)
				op_client->client_handler(op_msg, op_msg->get_err());
			else
				comm_log("ra_ error");

			queue_data->attached--;
			//if(op_msg->is_file() && !op_msg->is_special()){
			//	op_client->rqs->insert(queue_data);
			//}
			if(queue_data->attached==0)
				delete queue_data;
			else
				comm_log("%d attached", queue_data->attached);

		}
	}

	return;
}

void ANT_Client::SetClientHandler(IN Comm_Handler cli_handler)
{
	client_handler = cli_handler;
}

