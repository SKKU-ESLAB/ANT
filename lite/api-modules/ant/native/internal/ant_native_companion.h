#ifndef __ANT_NATIVE_COMPANION_H__
#define __ANT_NATIVE_COMPANION_H__

#include <sys/types.h>

void ant_companion_getMyIPAddress_internal(const char *interfaceName, char *resultIPAddress);

void initANTCompanion(void);

#endif /* !defined(__ANT_NATIVE_COMPANION_H__) */