#ifndef __COMM_BT_H__
#define __COMM_BT_H__
#ifdef __cplusplus
extern "C"{
#endif
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#define MAX_BT_SCAN_LEN 5
#define MAX_BT_SCAN_RSP 255

enum BTRESULT
{
	BT_S_OK
		, BT_S_FALSE
		, BT_E_POINTER
		, BT_E_OUTOFMEMORY
		, BT_E_FAIL
		, BT_E_NOINTERFACE
		, BT_E_NOTIMPL
		, BT_E_TYPO
		, BT_E_INVALID_PARAM
};
int bt_dynamic_bind_rc(int sock);
sdp_session_t *bt_register_service(uint8_t *uint8_arr, int port);
int bt_connect(uint8_t *uuid, int sock);
#ifdef __cplusplus
}
#endif

#endif
