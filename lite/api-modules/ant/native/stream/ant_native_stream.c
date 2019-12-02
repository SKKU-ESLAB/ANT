#include "ant_native_stream.h"

#include <glib.h>
#include <gst/gst.h>

bool ant_stream_test_pipeline(const char *ipAddress) {
  GstElement *pipeline, *source, *converter, *omxh264enc, *h264parse,
      *rtph264pay, *gdppay, *sink;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init(NULL, NULL);

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

    return false;
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
  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                   GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Free resources */
  if (msg != NULL)
    gst_message_unref(msg);
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return true;
}