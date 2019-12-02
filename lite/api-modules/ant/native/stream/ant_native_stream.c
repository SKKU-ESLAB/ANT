#include "ant_native_stream.h"

#include <glib.h>
#include <gst/gst.h>

#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-protocol.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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

DBusHandlerResult msg_dbus_filter(DBusConnection *connection, DBusMessage *msg,
                                  void *data) {
  if (dbus_message_is_signal(msg, "org.ant.streamthread", "sendMessage")) {
    const char *message_string;
    dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING, &(message_string),
                          DBUS_TYPE_INVALID);
    g_print("Message catched: %s", message_string);
    return DBUS_HANDLER_RESULT_HANDLED;
  } else {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }
}

void *test_pipeline_thread_fn(void *arg) {
  GstElement *pipeline, *source, *converter, *omxh264enc, *h264parse,
      *rtph264pay, *gdppay, *sink;
  guint bus_watch_id;
  GMainLoop *loop;
  DBusConnection *dbus_connection;
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
  {
    DBusError dbus_error;
    void *data = NULL;

    dbus_error_init(&dbus_error);
    dbus_connection = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error);
    if (dbus_error_is_set(&this->mDBusError)) {
      g_printerr("D-bus bus system registration failed");
      dbus_error_free(&dbus_error);
      return NULL;
    }

    dbus_bus_add_match(dbus_connection,
                       "type='signal', interface='org.ant.streamthread'",
                       &dbus_error);
    if (dbus_error_is_set(&this->mDBusError)) {
      g_printerr("D-bus add a match rule failed");
      dbus_error_free(&dbus_error);
      return NULL;
    }

    if (!dbus_connection_add_filter(dbus_connection, (*filter), data, NULL)) {
      ANT_LOG_ERR(CAM, "D-Bus add filter failed");
      return false;
    }

    dbus_connection_setup_with_g_main(dbus_connection, NULL);
  }

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

  pthread_exit(NULL);

  // TODO: notify IoT.js to set "ant.stream._mIsInitialized = false"
}

bool ant_stream_testPipeline_internal(const char *ipAddress) {
  snprintf(g_ip_address, 100, ipAddress);
  pthread_create(&g_test_pipeline_thread, NULL, &test_pipeline_thread_fn, NULL);
  return true;
}

bool ant_stream_testMessage_internal(const char *message) { return true; }