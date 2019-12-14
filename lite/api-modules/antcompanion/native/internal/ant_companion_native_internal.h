#ifndef __ANT_COMPANION_NATIVE_INTERNAL_H__
#define __ANT_COMPANION_NATIVE_INTERNAL_H__

#include <sys/types.h>

// TODO: hardcoded result message length
#define MAX_RESULT_MESSAGE_LENGTH 100

void ant_companion_getMyIPAddress_internal(const char *interfaceName, char *resultIPAddress);

void initANTCompanion(void);

#endif /* !defined(__ANT_COMPANION_NATIVE_INTERNAL_H__) */