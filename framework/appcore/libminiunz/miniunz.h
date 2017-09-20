
#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__))
        #ifndef __USE_FILE_OFFSET64
                #define __USE_FILE_OFFSET64
        #endif
        #ifndef __USE_LARGEFILE64
                #define __USE_LARGEFILE64
        #endif
        #ifndef _LARGEFILE64_SOURCE
                #define _LARGEFILE64_SOURCE
        #endif
        #ifndef _FILE_OFFSET_BIT
                #define _FILE_OFFSET_BIT 64
        #endif
#endif

#ifdef __APPLE__
// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#define FTELLO_FUNC(stream) ftello(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
# include <direct.h>
# include <io.h>
#else
# include <unistd.h>
# include <utime.h>
#endif

#include "unzip.h"

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#ifdef _WIN32
#define USEWIN32IOAPI
#include "iowin32.h"
#endif


void change_file_date(
    const char *filename,
    uLong dosdate,
    tm_unz tmu_date);

int mymkdir(const char* dirname);
int makedir (char *newdir);
void do_banner();
void do_help();
void Display64BitsSize(ZPOS64_T n, int size_char);

int do_list(unzFile uf);
int do_extract_currentfile(unzFile uf,const int* popt_extract_without_path,int* popt_overwrite,
    const char* password);

int do_extract(
    unzFile uf,
    int opt_extract_without_path,
    int opt_overwrite,
    const char* password);

int do_extract_onefile(
    unzFile uf,
    const char* filename,
    int opt_extract_without_path,
    int opt_overwrite,
    const char* password);

int do_unzip(int argc, char *argv[]);
