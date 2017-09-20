#ifndef __COMM_CORE_H__
#define __COMM_CORE_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

#include <uv.h>

#include <comm_queue.h>
#include <comm_bt.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_NAME_LEN 16
#define MAX_MSG_LEN 880
#define MAX_DAT_LEN 880
#define MAX_REQ_LEN 256
#define MAX_CLIENTS 30		//bit operation check
#define REQ_TIMEOUT 10
#define ANT_HEADER_SIZE 128
#define BUFF_SIZE 1024
#define MAX_FILE_TIMEOUT 10
#define MAX_MSG_TIMEOUT 1
#define SECFROMNANO 1000000000

enum {
	QUEUE_TYPE_WRITE = 0,\
	QUEUE_TYPE_READ,\
	QUEUE_TYPE_REQ,\
	QUEUE_TYPE_ACK \
};

enum {
	PACKET_TYPE_MSG = 1,\
	PACKET_TYPE_FILE = 2,\
	PACKET_TYPE_ACK = 4, \
	PACKET_TYPE_SPE = 8
};
enum {
	CONNECTION_TYPE_BT = 1,\
	CONNECTION_TYPE_WD = 2 \
};
enum {
	CV_STAT_INIT = 0,\
	CV_STAT_READY = 1,\
	CV_STAT_WAIT = 2, \
	CV_STAT_REM = 3 
};

enum{
	SOCKET_ERR_NONE = 0,
	SOCKET_ERR_DISCON,
	SOCKET_ERR_FAIL,
	SOCKET_ERR_FOPEN,	
	SOCKET_ERR_FREAD,
	SOCKET_ERR_FWRIT,
	SOCKET_ERR_TOUT
};

enum{
	CLI_DISCONNECTED = 0,
	CLI_CONNECTING,
	CLI_CONNECTED
};

typedef char ant_bd_name[248];

typedef struct _req_t {
	int reqId;
} req_t;

typedef struct _file_info_t {
	char fname[24];
	uint32_t fsize;
	uint32_t offset;
} file_info_t;

typedef struct _msg_info_t {
	char dest_intf[16];
	char src_intf[16];
} msg_info_t;


class ANT_Header
{
	public:
		uint32_t req_id;
		uint32_t data_len;
		union {
			file_info_t finfo;
			msg_info_t minfo;
		} data_info;
		uint16_t type;
		uint16_t err;
		uint16_t chksum;
		char destFname[24];
		bool initialized;

		ANT_Header();
		int init_from_buff(uint8_t *buff);
		int init_to_buff(uint8_t *buff);
		bool isInitialized() {	return initialized;	}
};

class ANT_Socket
{
	private:
		// CONNECTION_TYPE_BT, CONNECTION_TYPE_WD
		int sock;
		
		uint8_t connection_type;
		uint8_t ref_cnt;

	public:
		ANT_Socket(int sock_fd, uint8_t type);
		~ANT_Socket();
		
		int get_sock_fd();
		void set_sock_fd(int sock_fd);

		bool get();
		bool put();

		uint8_t get_ref_cnt();
		int Read(OUT uint8_t *buff, IN int size);
		int Write(IN uint8_t *buff, IN int size);
};

bool dynamic_sock_get(ANT_Socket **sock);
bool dynamic_sock_put(ANT_Socket **sock);


class ANT_MSG
{
	private:
		ANT_Header *op_header;
		ANT_Socket *op_sock;
		uint8_t *data;
	public:
		ANT_MSG();
		ANT_MSG(ANT_Socket *op_sock);
		~ANT_MSG();

		ANT_Header *get_header();
		uint32_t get_req_id();
		uint32_t get_data_len();
		bool is_file();
		bool is_msg();
		bool is_ack();
		bool is_special();

		char *get_file_name();
		char *get_dfile_name();
		uint32_t get_file_offset();
		uint32_t get_file_size();
		uint16_t get_err();
		uint8_t *get_data();
		ANT_Socket *get_op_sock();

		void set_req_id(uint32_t req);
		void set_file(file_info_t *finfo);
		void set_msg(msg_info_t *minfo);
		void set_ack();
		void set_special();
		void set_err(uint16_t err);
		void set_data(uint8_t *data_p, uint32_t data_len);
		void set_dfile_name(char *src);
		void complete_header();

		void *priv;
};

typedef void (*Comm_Handler)(ANT_MSG *, int );

//Thread safe
class queue_data_t
{
	public:
		uint8_t type;
		uint8_t *buff; //for write
		uint32_t buff_len;
		ANT_MSG *op_msg;

		comm_queue_t queue;
		int attached;

		int sock_pos;
		int cv_num; //for Req queue

		Comm_Handler handler;

		queue_data_t();
		queue_data_t(ANT_Socket *op_sock);
		~queue_data_t();
		
		void call_handler(void);
};

class ANT_Comm_Queue
{
	private:
		uv_mutex_t queue_mutex;
		comm_queue_t queue;
		uint32_t len;

	public:
		ANT_Comm_Queue();
		uint32_t get_len();
		int enqueue(IN queue_data_t *data);
		queue_data_t* dequeue();
		bool isEmptyQueue();
};

class BT_Operations
{
	private:
		int port;
		sdp_session_t *session;
		int dynamic_bind_rc(int sock);
		sdp_session_t *register_service(
				char *intf_name,
				int port);
	public:
		BT_Operations();
		~BT_Operations();
		int openBT(int sock, char *intf_name);
		int connectBT(char *intf_name, int sock);

	
};

class socket_set
{
	private:
		uv_mutex_t socket_set_mutex;
		ANT_Socket* sockets[MAX_CLIENTS];
		uint32_t socket_set_bitmap;
		uint8_t socket_set_len;

	public:
		socket_set();
		~socket_set();

		int insert(IN ANT_Socket* op_sock);
		int insert(IN int sock_fd, uint8_t type);
		void remove(int sock_no);
		ANT_Socket *get(int pos);
		int length();


};
/*
class cv_set
{
	private:
		uint8_t status[MAX_REQ_LEN];
		uv_mutex_t cv_bitmap_mutex;
		uint32_t cv_bitmap;
		uv_mutex_t cv_mutex[MAX_REQ_LEN];
		uv_cond_t cv[MAX_REQ_LEN];
		uv_cond_t notWaiting[MAX_REQ_LEN];
		uint32_t req[MAX_REQ_LEN];
		Comm_Handler handlers[MAX_REQ_LEN];
		int cv_len;
	public:
		cv_set();
		~cv_set();

		int insert(uint32_t reqid, Comm_Handler handler);
		int remove(uint32_t reqid);
		int gc(void);
		int search(uint32_t reqid, uint8_t *stat);
		int signal(uint32_t reqid);
		int sch_to_sig(uint32_t reqid, ANT_Comm_Queue *queue, queue_data_t *queue_data);
		
		//-1:didnt wait, 0:normal wait, 1:full wait(timed out)
		int wait(int i, uint32_t timeout);
		int getHandler(uint32_t reqid, Comm_Handler *handler_p);
		int getLen();
};
*/

class req_set {
	private:
		int tout[MAX_REQ_LEN];
		queue_data_t *qdts[MAX_REQ_LEN];
		ANT_Comm_Queue *ackQueue;
		uv_mutex_t lock;
		uv_cond_t inserted;
	public:
		req_set(ANT_Comm_Queue *ackQueue);
		~req_set();
		int wait(uint32_t timeout);
		int insert(queue_data_t *queue_data);
		void signal(int reqId, queue_data_t *qdt=NULL, int refresh=0);
};

class ANT_Server
{
	private:
		char intf_name[MAX_NAME_LEN];
		ANT_Socket *server_sock;
		BT_Operations *bt_ops;
			
		socket_set *clients;

		fd_set readfds;
		int max_fd;

		int num_threads;
		
		//Thread safe
		ANT_Comm_Queue read_queue;
		ANT_Comm_Queue write_queue;
		ANT_Comm_Queue ack_queue;

		int closing;	//What is this for?
		bool write_thread_alive;
		bool read_thread_alive;
		bool ra_thread_alive;
		uv_work_t write_req;
		uv_work_t read_req;
		//uv_work_t ra_req[MAX_REQ_LEN];
		uv_work_t ra_req;

		req_set *rqs;

		//cv_set *cvs;

		// JS safe
		Comm_Handler server_handler;

	
		static void generic_read_handler(IN uv_work_t *req);
		static void after_read_handler(IN uv_work_t *req, int status);
		static void generic_write_handler(IN uv_work_t *req);
		static void after_write_handler(uv_work_t *req, int status);
		static void generic_ra_handler(uv_work_t *req);
		static void after_ra_handler(uv_work_t *req, int status);
	
	public:
		ANT_Server(const char *intf_name);
		ANT_Server(const char *intf_name, Comm_Handler serv_handler);
		~ANT_Server();
		
		void SetServerHandler(IN Comm_Handler serv_handler);
		int file_write(IN const char *filePath, IN ANT_MSG *req_msg = NULL, \
				IN Comm_Handler ack_file_handler = NULL, IN int cli_no=0, void *priv = NULL);
		int msg_write(IN const char *buf, IN int len, IN ANT_MSG *req_msg = NULL, \
				IN Comm_Handler ack_msg_handler = NULL,	IN int cli_no = 0, void *priv = NULL);

};
class ANT_Client
{
	private:
		char intf_name[MAX_NAME_LEN];
		ANT_Socket *serv_sock;
		BT_Operations *bt_ops;
		bool connected;
		int connect_err;
			

		fd_set readfds;
		int max_fd;

		int num_threads;
		
		//Thread safe
		ANT_Comm_Queue read_queue;
		ANT_Comm_Queue write_queue;
		ANT_Comm_Queue ack_queue;

		int closing;	//What is this for?
		int connection;
		bool write_thread_alive;
		bool read_thread_alive;
		bool ra_thread_alive;

		uv_work_t connect_req;
		uv_work_t write_req;
		uv_work_t read_req;
		//uv_work_t ra_req[MAX_REQ_LEN];
		uv_work_t ra_req;

		//cv_set *cvs;
		req_set *rqs;

		// JS safe
		Comm_Handler client_handler;
		Comm_Handler onConnect;

		static void generic_connect_handler(IN uv_work_t *req);
		static void after_connect_handler(IN uv_work_t *req, int status);
		static void generic_read_handler(IN uv_work_t *req);
		static void after_read_handler(IN uv_work_t *req, int status);
		static void generic_write_handler(IN uv_work_t *req);
		static void after_write_handler(uv_work_t *req, int status);
		static void generic_ra_handler(uv_work_t *req);
		static void after_ra_handler(uv_work_t *req, int status);

	public:
		ANT_Client(const char *intf_name, Comm_Handler onConnect);
		ANT_Client(const char *intf_name, Comm_Handler client_handler, Comm_Handler onConnect);
		~ANT_Client();

		void SetClientHandler(IN Comm_Handler cli_handler);
		int file_write(IN const char *filePath, IN ANT_MSG *req_msg = NULL, IN Comm_Handler ack_file_handler = NULL);
		int msg_write(IN const char *buf, IN int len, IN ANT_MSG *req_msg = NULL, IN Comm_Handler ack_msg_handler = NULL);


};

#endif
