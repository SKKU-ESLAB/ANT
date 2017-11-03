/*
	Libuv based d-bus connection
	From other source
*/

/*
 * Copyright (c) 2011, Motorola Mobility, Inc
 * All Rights Reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  - Neither the name of Motorola Mobility nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * */


#include <stdlib.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <uv.h>
#include <stdbool.h>
#include "nil.h"


static void
handle_asyncw_freed(void *data) {
	uv_async_t *asyncw = (uv_async_t *)data;
	if (asyncw == NULL)
		return;
	asyncw->data = NULL;
	uv_close((uv_handle_t *)asyncw, (uv_close_cb)g_free);
}

static void
handle_iow_freed(void *data) {
	uv_poll_t *iow = (uv_poll_t*)data;
	if (iow == NULL)
		return;
	iow->data = NULL;
	uv_ref((uv_handle_t *)iow);
	uv_poll_stop(iow);
	uv_close((uv_handle_t *)iow, (uv_close_cb)g_free);
}

static void
iow_cb(uv_poll_t *w, gint status, gint events) {
	DBusWatch *watch = (DBusWatch*)w->data;
	guint dbus_condition = 0;

	if (events & UV_READABLE)
		dbus_condition |= DBUS_WATCH_READABLE;
	if (events & UV_WRITABLE)
		dbus_condition |= DBUS_WATCH_WRITABLE;
#if 0
	if (events & EV_ERROR) {
		dbus_condition |= DBUS_WATCH_ERROR;
		dbus_condition |= DBUS_WATCH_HANGUP;
	}
#endif

	dbus_watch_handle(watch, dbus_condition);
}

static dbus_bool_t
add_watch(DBusWatch *watch, void *data) {
	if (!dbus_watch_get_enabled(watch)
		|| dbus_watch_get_data(watch) != NULL)
		return true;

	gint fd = dbus_watch_get_unix_fd(watch);
	guint flags = dbus_watch_get_flags(watch);
	gint events = 0;

	if (flags & DBUS_WATCH_READABLE)
		events |= UV_READABLE;
	if (flags & DBUS_WATCH_WRITABLE)
		events |= UV_WRITABLE;

	uv_poll_t *iow = g_new0(uv_poll_t, 1);
	iow->data = (void *)watch;
	uv_poll_init(uv_default_loop(), iow, fd);
	uv_poll_start(iow, events, iow_cb);
	uv_unref((uv_handle_t *)iow);

	dbus_watch_set_data(watch, (void *)iow, handle_iow_freed);
	return true;
}

static void
remove_watch(DBusWatch *watch, void *data) {
	uv_poll_t* iow = (uv_poll_t*)dbus_watch_get_data(watch);

	if (iow == NULL)
		return;

	dbus_watch_set_data(watch, NULL, NULL);
}

static void
watch_toggled(DBusWatch *watch, void *data) {
	if (dbus_watch_get_enabled(watch))
		add_watch(watch, data);
	else
		remove_watch(watch, data);
}

static void
timeout_cb(uv_timer_t *w/*, int status*/) {
	DBusTimeout *timeout = (DBusTimeout*)w->data;
	dbus_timeout_handle(timeout);
}

static void
handle_timeout_freed(void *data) {
	uv_timer_t *timer = (uv_timer_t*)data;
	if (timer == NULL)
		return;
	timer->data = NULL;
	uv_timer_stop(timer);
	uv_unref((uv_handle_t *)timer);
	uv_close((uv_handle_t *)timer, (uv_close_cb)g_free);
}

static dbus_bool_t
add_timeout(DBusTimeout *timeout, void *data) {
	if (!dbus_timeout_get_enabled(timeout)
		|| dbus_timeout_get_data(timeout) != NULL)
		return true;

	uv_timer_t* timer = g_new0(uv_timer_t, 1);
	timer->data = timeout;
	uv_timer_init(uv_default_loop(), timer);
	uv_timer_start(timer, timeout_cb, dbus_timeout_get_interval(timeout), 0);
	//int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat)

	dbus_timeout_set_data(timeout, (void *)timer, handle_timeout_freed);
	return true;
}

static void
remove_timeout(DBusTimeout *timeout, void *data) {
	uv_timer_t *timer =
		(uv_timer_t*)dbus_timeout_get_data(timeout);

	if (timer == NULL)
		return;

	dbus_timeout_set_data(timeout, NULL, NULL);
}

static void
timeout_toggled(DBusTimeout *timeout, void *data) {
	if (dbus_timeout_get_enabled(timeout))
		add_timeout(timeout, data);
	else
		remove_timeout(timeout, data);
}

static void
wakeup_ev(void *data) {
	uv_async_t* asyncw = (uv_async_t *)data;
	uv_async_send(asyncw);
}

static void
asyncw_cb(uv_async_t *w/*, int revents*/) {
	DBusConnection *bus_cnxn = (DBusConnection *)w->data;
	dbus_connection_read_write(bus_cnxn, 0);
	while (dbus_connection_dispatch(bus_cnxn) ==
		DBUS_DISPATCH_DATA_REMAINS);
}

gboolean
NIL_DbusConnection(DBusConnection *bus_cnxn) {
	if (!dbus_connection_set_watch_functions(bus_cnxn,
		add_watch,
		remove_watch,
		watch_toggled,
		NULL, NULL))
		return false;

	if (!dbus_connection_set_timeout_functions(bus_cnxn,
		add_timeout,
		remove_timeout,
		timeout_toggled,
		NULL, NULL))
		return false;

	uv_async_t *asyncw = g_new0(uv_async_t, 1);
	asyncw->data = (void *)bus_cnxn;
	uv_async_init(uv_default_loop(), asyncw, asyncw_cb);
	uv_unref((uv_handle_t *)asyncw);
	dbus_connection_set_wakeup_main_function(bus_cnxn,
		wakeup_ev,
		(void *)asyncw, handle_asyncw_freed);

	return true;
}

