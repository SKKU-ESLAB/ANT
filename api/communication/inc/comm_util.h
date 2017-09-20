#ifndef __COMM_UTIL_H__
#define __COMM_UTIL_H__

#include <stdio.h>
#include <stdarg.h>

#define comm_log(format, args...) do { _comm_log(format, __FILE__, __func__, __LINE__, ##args); } while(0)

#ifndef IN
#define IN
#define OUT
#endif

#define NO_LOG


#ifndef NO_LOG
inline void _comm_log(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	va_list ap;

	printf("COMM(%s):%s(%d)\t> ", funcName, fileName,lineNo);
	va_start(ap, lineNo);
	vprintf(format, ap);
	va_end(ap);
	printf("\n");
}
#else
inline void _comm_log(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	va_list ap;
}
#endif

void name2uuid(IN char inputString[], OUT unsigned int outUuid[])
{
	int x;

	for(x=0; x<4; x++){
		outUuid[x] = (unsigned int)inputString[x*4] | (unsigned int)inputString[x*4+1] << 8\
					 | (unsigned int)inputString[x*4+2] << 16 | (unsigned int)inputString[x*4+3] << 24;
	}

}

int find_first_zero_bit(unsigned int x)
{
	int invert, twos;
	int res = -1;

	invert = ~x;
	twos = -invert;

	x = invert & twos;

	while(x != 0){
		x = x >> 1;
		res++;
	}

	return res;
}
enum COMMRESULT
{
	COMM_S_OK
		, COMM_S_FALSE
		, COMM_E_POINTER
		, COMM_E_OUTOFMEMORY
		, COMM_E_FAIL
		, COMM_E_NOINTERFACE
		, COMM_E_NOTIMPL
		, COMM_E_TYPO
		, COMM_E_INVALID_PARAM
};
#endif
