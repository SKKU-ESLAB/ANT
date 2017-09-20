#include <comm_util.h>
#include <comm_queue.h>


class ANT_Communicator
{
	private:
		int BD_Stat, WD_Stat;

		/* Bluetooth Configuration Properties */
		bdaddr_t target;
		inquiry_info *bt_scan_list;
		int num_rsp;
		void SetTarget(char *bdaddr_str);

		/* Wifi Direct Configuration Properties */


	public:
		ANT_Communicator();
		~ANT_Communicator();

		// Advertise this device as a ant device

		int Scan(OUT ant_bd_name *name_array, IN bool refresh = 0 );
		int ScanWiFiDirect();
		int ScanBD();

		/* This address is MAC-like 48 bit address with char array representation */
		int Connect(IN const char *addr);

		/*
		   Open RFCOMM Channel and advertise it as a service into the pico net
		   intf_name (IN) - This should be allocated before method call.
		   ant_server (OUT) - This method returns ANT_Server class as a pointer by allocating the new instance of it.
		 */
		int OpenChannel(IN const char *intf_name, OUT ANT_Server **ant_server, IN void (*server_handler)(IN ANT_Socket *client, IN ANT_MSG *msg) = NULL);

		/*
		   Access the channel which is opened by the other piconet node
		   intf_name (IN) - This should be allocated before method call.
		   ant_client (OUT) - This method returns ANT_client class as a pointer by allocating the new instance of it.
		 */
		int AccessChannel(IN const char *intf_name, OUT ANT_Client **ant_client, IN void (*client_handler)(IN ANT_MSG *msg) = NULL);

};
