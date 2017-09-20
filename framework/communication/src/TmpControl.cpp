/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Eunsoo Park<esevan.park@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "TmpControl.h"

static void _manipulate_res_name(char *res_name, OUT char *res_full_name)
{
	strcpy( res_full_name, "/tmp/" );
	if( res_name[0] == '/' ){
		strcat( res_full_name, res_name + 1 );
	}
	else {
		strcat( res_full_name, res_name );
	}
}
static int _rm_rf( char *res_name )
{
	char cmd[256];
	snprintf( cmd, 255, "rm -rf %s", res_name );
	return system( cmd );
}
static int _mkdir_p(char *res_name)
{
	char cmd[256];
	snprintf( cmd, 255, "mkdir -p %s", res_name );
	return system( cmd );
}
int tmpc_post(char *res_name, char *res_val, size_t count)
{
	int res = 0;
	int fd_res = 0;
	struct stat st;
	char res_name_buff[RES_NAME_LEN];
	
	__TMP_ENTER__;

	if( !res_name ){
		res = -1; //INVALID PARAMS
		__TMP_EXIT__;
		return res;
	}
	if( res_name[0] == '\0' ){
		res = -1;
		__TMP_EXIT__;
		return res;
	}

	_manipulate_res_name( res_name, res_name_buff );

	fd_res = stat( res_name_buff, &st );
	if( fd_res < 0 && errno == ENOENT ) {
		_mkdir_p( res_name_buff );
	}

	strcat( res_name_buff, "/self");
	fd_res = stat( res_name_buff, &st );
	if( fd_res < 0 && errno == ENOENT ) {
		fd_res = open( res_name_buff, O_RDWR | O_TRUNC | O_CREAT, 0666 );
		tmpc_log("%s created", res_name_buff );
	}
	else {
		fd_res = open( res_name_buff, O_RDWR );
		if( fd_res < 0 ) {
			res = -2; //FAILED TO OPEN
			__TMP_EXIT__;
			return res;
		}
	}

	if( res_val ){
		int w_res = write( fd_res, res_val, count );
		if( w_res < 0 ){
			res = -3; //WRITE FAILED
			close( fd_res );
			__TMP_EXIT__;
			return res;
		}
	}
	else{
		int w_res = write( fd_res, "", sizeof(""));
		if( w_res < 0 ){
			res = -3; //WRITE FAILED
			close( fd_res );
			__TMP_EXIT__;
			return res;
		}
	}

	close( fd_res );

	__TMP_EXIT__;
	return res;
}
int tmpc_get(char *res_name, OUT char *val, size_t count)
{
	int fd_res, res = 0;
	char res_name_buff[RES_NAME_LEN];
	__TMP_ENTER__;

	if( !res_name ) {
		res = -1;
		__TMP_EXIT__;
		return res;
	}
	if( res_name[0] == '\0' ){
		res = -1;
		__TMP_EXIT__;
		return res;
	}
	_manipulate_res_name( res_name, res_name_buff );

	strcat( res_name_buff, "/self" );

	fd_res = open( res_name_buff, O_RDONLY );
	if( fd_res < 0 ){
		res = -2; //FAILED TO OPEN
		__TMP_EXIT__;
		return res;
	}

	int r_res = read( fd_res, val, count );
	if( r_res < 0 ){
		res = -3; //READ FAILED
		close( fd_res );
		__TMP_EXIT__;
		return res;
	}

	close( fd_res );

	res = r_res;
	
	__TMP_EXIT__;
	return res;
}
int tmpc_put(char *res_name, char *res_val, size_t count)
{
	int fd_res, res = 0;
	char res_name_buff[RES_NAME_LEN];
	
	__TMP_ENTER__;

	if( !res_name ) {
		res = -1;
		__TMP_EXIT__;
		return res;
	}
	if( res_name[0] == '\0' ){
		res = -1;
		__TMP_EXIT__;
		return res;
	}
	
	_manipulate_res_name( res_name, res_name_buff );
	strcat( res_name_buff, "/self" );

	fd_res = open( res_name_buff, O_RDWR );
	if( fd_res < 0 ){
		res = -2; //FAILED TO OPEN
		__TMP_EXIT__;
		return res;
	}

	if( res_val ){
		int w_res = write( fd_res, res_val, count );
		if( w_res < 0 ){
			res = -3; //WRITE FAILED
			close( fd_res );
			__TMP_EXIT__;
			return res;
		}
		res = w_res;
	}

	close( fd_res );
	__TMP_EXIT__;
	return res;
}
int tmpc_del(char *res_name)
{
	int fd_res, res = 0;
	char res_name_buff[RES_NAME_LEN];
	
	__TMP_ENTER__;

	if( !res_name ) {
		res = -1;
		__TMP_EXIT__;
		return res;
	}
	if( res_name[0] == '\0' ){
		res = -1;
		__TMP_EXIT__;
		return res;
	}

	_manipulate_res_name( res_name, res_name_buff );
	fd_res = _rm_rf( res_name_buff );
	if( fd_res ){
		res = -2;
		__TMP_EXIT__;
		return res;
	}
	
	__TMP_EXIT__;
	return res;
}
int tmpc_subscribe(char *res_name, void (*callback)(void *))
{
	int res = 0;
	__TMP_ENTER__;

	// TODO: subscribe API

	__TMP_EXIT__;
	return res;
}

