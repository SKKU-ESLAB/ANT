#include "ant_native_stream.h"

#include <gio/gio.h>
#include <glib.h>
#include <gst/gst.h>

#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-protocol.h>

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ANT_STREAMTHREAD_DBUS_BUS "org.ant.streamThread"
#define ANT_STREAMTHREAD_DBUS_PATH "/org/ant/streamThread"
#define ANT_STREAMTHREAD_DBUS_INTERFACE "org.ant.streamthread"
#define ANT_STREAMTHREAD_DBUS_SIGNAL "sendMessage"
#define ANT_STREAMTHREAD_DBUS_RULE                                             \
  "type='signal', interface='" ANT_STREAMTHREAD_DBUS_INTERFACE "'"

static const gchar g_introspection_xml[] =
    "<node>"
    "   <interface name='" ANT_STREAMTHREAD_DBUS_INTERFACE "'>"
    "     <signal name='" ANT_STREAMTHREAD_DBUS_SIGNAL "'>"
    "       <arg type='s' name='message' />"
    "     </signal>"
    "   </interface>"
    "</node>";

static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
  GMainLoop *loop = (GMainLoop *)data;

  switch (GST_MESSAGE_TYPE(msg)) {
  case GST_MESSAGE_EOS:
    g_print("End of stream\n");
    g_main_loop_quit(loop);
    break;

  case GST_MESSAGE_ERROR: {
    gchar *debug;
    GError *error;

    gst_message_parse_error(msg, &error, &debug);
    g_free(debug);

    g_printerr("Error: %s\n", error->message);
    g_error_free(error);

    g_main_loop_quit(loop);
    break;
  }
  default:
    break;
  }

  return TRUE;
}

char g_ip_address[100];
pthread_t g_test_pipeline_thread;

DBusHandlerResult dbus_message_filter_fn(DBusConnection *connection,
                                         DBusMessage *msg, void *data) {
  DBusError dbus_error;
  dbus_error_init(&dbus_error);

  if (dbus_message_is_signal(msg, ANT_STREAMTHREAD_DBUS_INTERFACE,
                             ANT_STREAMTHREAD_DBUS_SIGNAL)) {
    const char *message_string;
    dbus_message_get_args(msg, &dbus_error, DBUS_TYPE_STRING, &(message_string),
                          DBUS_TYPE_INVALID);
    g_print("Message catched: %s\n", message_string);
    return DBUS_HANDLER_RESULT_HANDLED;
  } else {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }
}

DBusConnection *g_dbus_connection;
GDBusNodeInfo *g_dbus_introspection_data;
void on_dbus_bus_acquired_fn(GDBusConnection *connection, const gchar *name,
                             gpointer user_data) {
  DBusError dbus_error;
  void *data = NULL;

  g_print("Acquired a bus connection\n");

  dbus_error_init(&dbus_error);
  g_dbus_connection = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error);
  if (dbus_error_is_set(&dbus_error)) {
    g_printerr("D-bus bus system registration failed: %s / %s", dbus_error.name,
               dbus_error.message);
    dbus_error_free(&dbus_error);
    return;
  }

  dbus_bus_add_match(g_dbus_connection, ANT_STREAMTHREAD_DBUS_RULE,
                     &dbus_error);
  if (dbus_error_is_set(&dbus_error)) {
    g_printerr("D-bus add a match rule failed");
    dbus_error_free(&dbus_error);
    return;
  }

  if (!dbus_connection_add_filter(g_dbus_connection, (*dbus_message_filter_fn),
                                  data, NULL)) {
    g_printerr("D-Bus add filter failed");
    return;
  }

  dbus_connection_setup_with_g_main(g_dbus_connection, NULL);
}

void *test_pipeline_thread_fn(void *arg) {
  GstElement *pipeline, *source, *converter, *omxh264enc, *h264parse,
      *rtph264pay, *gdppay, *sink;
  guint bus_watch_id;
  GMainLoop *loop;
  char *ipAddress;
  ipAddress = g_ip_address;

  /* Initialize GStreamer */
  gst_init(NULL, NULL);
  loop = g_main_loop_new(NULL, FALSE);

  /* Build the pipeline */
  /* videotestsrc pattern=snow ! videoconvert ! omxh264enc ! h264parse !
   * rtph264pay pt=96 config-interval=1 ! gdppay ! tcpserversink sync=f
   * host=192.168.0.33 port=5000 */
  pipeline = gst_pipeline_new("test");
  source = gst_element_factory_make("videotestsrc", NULL);
  converter = gst_element_factory_make("videoconvert", NULL);
  omxh264enc = gst_element_factory_make("omxh264enc", NULL);
  h264parse = gst_element_factory_make("h264parse", NULL);
  rtph264pay = gst_element_factory_make("rtph264pay", NULL);
  gdppay = gst_element_factory_make("gdppay", NULL);
  sink = gst_element_factory_make("tcpserversink", NULL);

  if (!pipeline || !source || !converter || !omxh264enc || !h264parse ||
      !rtph264pay || !gdppay || !sink) {
    g_printerr("One element could not be created. Existing.\n");
    return NULL;
  }

  // videotestsrc
  g_object_set(G_OBJECT(source), "pattern", 1, NULL);

  // videoconvert
  // omxh264enc
  // h264parse
  // rtph264pay
  g_object_set(G_OBJECT(rtph264pay), "pt", 96, "config-interval", 1, NULL);
  // gdppay
  // tcpserversink
  g_object_set(G_OBJECT(sink), "sync", FALSE, "host", ipAddress, "port", 5000,
               NULL);

  gst_bin_add_many(GST_BIN(pipeline), source, converter, omxh264enc, h264parse,
                   rtph264pay, gdppay, sink, NULL);
  gst_element_link_many(source, converter, omxh264enc, h264parse, rtph264pay,
                        gdppay, sink, NULL);

  /* Start playing */
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  /* Initialize gdbus filter */
  g_dbus_introspection_data =
      g_dbus_node_info_new_for_xml(g_introspection_xml, NULL);
  g_assert(g_dbus_introspection_data != NULL);
  g_bus_own_name(G_BUS_TYPE_SESSION, ANT_STREAMTHREAD_DBUS_BUS,
                 (GBusNameOwnerFlags)(G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                                      G_BUS_NAME_OWNER_FLAGS_REPLACE),
                 on_dbus_bus_acquired_fn, NULL, NULL, NULL, NULL);

  /* gstreamer main loop: wait until error or EOS */
  {
    GstBus *bus;
    bus = gst_element_get_bus(pipeline);
    bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);
  }
  g_print("Main Loop for Gstreamer Running...\n");
  g_main_loop_run(loop);

  /* Free resources */
  g_source_remove(bus_watch_id);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  g_main_loop_unref(loop);
  g_dbus_node_info_unref(g_dbus_introspection_data);

  pthread_exit(NULL);

  // TODO: notify IoT.js to set "ant.stream._mIsInitialized = false"
}

bool ant_stream_testPipeline_internal(const char *ipAddress) {
  snprintf(g_ip_address, 100, ipAddress);
  pthread_create(&g_test_pipeline_thread, NULL, &test_pipeline_thread_fn, NULL);
  return true;
}

bool ant_stream_sendDbusSignal_internal(const char *message) {
  DBusError dbus_error;
  DBusConnection *dbus_connection;
  DBusMessage *dbus_message;

  if (message == NULL) {
    g_printerr("Invalid d-bus message contents!");
    return false;
  }

  dbus_error_init(&dbus_error);
  dbus_connection = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error);
  if (dbus_error_is_set(&dbus_error) || dbus_connection == NULL) {
    g_printerr("Invalid dbus connection!: %s / %s", dbus_error.name,
               dbus_error.message);
    dbus_error_free(&dbus_error);
    return false;
  }

  dbus_message = dbus_message_new_signal(ANT_STREAMTHREAD_DBUS_PATH,
                                         ANT_STREAMTHREAD_DBUS_INTERFACE,
                                         ANT_STREAMTHREAD_DBUS_SIGNAL);

  dbus_message_append_args(dbus_message, DBUS_TYPE_STRING, &(message),
                           DBUS_TYPE_INVALID);
  dbus_connection_send(dbus_connection, dbus_message, NULL);

  return true;
}

void initANTStream(void) {
  setenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/run/dbus/system_bus_socket",
         1);
}