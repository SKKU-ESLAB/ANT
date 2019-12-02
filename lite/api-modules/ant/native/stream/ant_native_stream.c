#include "ant_native_stream.h"

#include <glib.h>
#include <gst/gst.h>
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

void *test_pipeline_thread_fn(void *arg) {
  GstElement *pipeline, *source, *converter, *omxh264enc, *h264parse,
      *rtph264pay, *gdppay, *sink;
  guint bus_watch_id;
  GMainLoop *loop;
  char *ipAddress;
  ipAddress = g_ip_address;
  printf("IP Address: %s\n", ipAddress);

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

  /* Wait until error or EOS */
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
}

bool ant_stream_testPipeline_internal(const char *ipAddress) {
  printf("IP Address': %s\n", ipAddress);
  snprintf(g_ip_address, 100, ipAddress);
  pthread_create(&g_test_pipeline_thread, NULL, &test_pipeline_thread_fn, NULL);
  return true;
}