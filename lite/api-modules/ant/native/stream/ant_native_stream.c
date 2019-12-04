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
#define ANT_STREAMTHREAD_DBUS_METHOD "callMethod"
#define ANT_STREAMTHREAD_DBUS_ADDRESS "tcp:host=0.0.0.0,port=40001"

// I refered a gdlib tutorial for inter-thread communication
// https://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gio/GDBusServer.html
static const gchar g_introspection_xml[] =
    "<node name='" ANT_STREAMTHREAD_DBUS_PATH "'>"
    "   <interface name='" ANT_STREAMTHREAD_DBUS_INTERFACE "'>"
    "     <method name='" ANT_STREAMTHREAD_DBUS_METHOD "'>"
    "       <arg type='s' direction='in' />"
    "       <arg type='s' direction='out' />"
    "     </method>"
    "   </interface>"
    "</node>";

static const GDBusInterfaceVTable g_interface_vtable = {
    .method_call = handle_method_call_fn,
    .get_property = NULL,
    .set_property = NULL};
GDBusNodeInfo *g_gdbus_introspection;
pthread_t g_stream_thread;

#define ELEMENT_REGISTRY_SIZE 100
int g_element_registry_index = 0;
GstElement *g_element_registry[ELEMENT_REGISTRY_SIZE] = {
    0,
};
int registerElement(GstElement *element) {
  if (g_element_registry_index >= (ELEMENT_REGISTRY_SIZE - 1)) {
    g_printerr("Element registry is full! (max size: %d)\n",
               ELEMENT_REGISTRY_SIZE);
    return -1;
  } else {
    int index = g_element_registry_index;
    g_element_registry[index];
    g_element_registry_index++;
    return index;
  }
}
inline GstElement *getElement(int index) { return g_element_registry[index]; }

static void
handle_method_call_fn(GDBusConnection *connection, const gchar *sender,
                      const gchar *object_path, const gchar *interface_name,
                      const gchar *method_name, GVariant *parameters,
                      GDBusMethodInvocation *invocation, gpointer user_data) {
  if (g_strcmp0(method_name, ANT_STREAMTHREAD_DBUS_METHOD) == 0) {
    const gchar *inputMessage;
    gchar *responseMessage;
    g_variant_get(parameters, "(&s)", &inputMessage);

    // TODO: put handlers here
    g_print("Incoming method call message: %s\n", inputMessage);

    responseMessage = g_strdup_printf("Response for '%s'", inputMessage);
    g_dbus_method_invocation_return_value(
        invocation, g_variant_new("(s)", responseMessage));
    g_free(responseMessage);
  }
}

static gboolean on_new_connection_fn(GDBusServer *gdbus_server,
                                     GDBusConnection *gdbus_connection,
                                     gpointer user_data) {
  guint registration_id;

  g_object_ref(gdbus_connection);
  registration_id = g_dbus_connection_register_object(
      gdbus_connection, ANT_STREAMTHREAD_DBUS_PATH,
      g_gdbus_introspection->interfaces[0], &g_interface_vtable,
      NULL,  /* user_data */
      NULL,  /* user_data_free_func */
      NULL); /* GError** */
  g_assert(registration_id > 0);

  return TRUE;
}

int rpc_streamapi_createPipeline(char *pipeline_name) {
  // On Stream Thread
  GstElement *pipeline;
  int element_index;
  pipeline = gst_pipeline_new(pipeline_name);
  element_index = registerElement(pipeline);
  return element_index;
}

// TODO: implement extra rpc functions

void *stream_thread_fn(void *arg) {
  // On Stream Thread
  GMainLoop *loop;

  /* Initialize GStreamer */
  gst_init(NULL, NULL);
  loop = g_main_loop_new(NULL, FALSE);

  /* Initialize gdbus filter */
  g_gdbus_introspection =
      g_dbus_node_info_new_for_xml(g_introspection_xml, NULL);
  g_assert(g_gdbus_introspection != NULL);
  {
    GDBusServer *gdbus_server;
    gchar *gdbus_guid;
    GDBusServerFlags gdbus_server_flags;
    GError *gerror;

    gdbus_guid = g_dbus_generate_guid();
    gdbus_server_flags = G_DBUS_SERVER_FLAGS_NONE |
                         G_DBUS_SERVER_FLAGS_AUTHENTICATION_ALLOW_ANONYMOUS;
    gerror = NULL;

    gdbus_server = g_dbus_server_new_sync(ANT_STREAMTHREAD_DBUS_ADDRESS,
                                          gdbus_server_flags, gdbus_guid, NULL,
                                          NULL, &gerror);
    g_dbus_server_start(gdbus_server);
    g_free(gdbus_guid);

    if (gdbus_server == NULL) {
      g_printerr("Error creating server at address %s: %s\n",
                 ANT_STREAMTHREAD_DBUS_ADDRESS, gerror->message);
      g_error_free(gerror);
      return NULL;
    }
    g_signal_connect(gdbus_server, "new-connection",
                     G_CALLBACK(on_new_connection_fn), NULL);
  }

  /* gstreamer main loop: wait until error or EOS */
  g_print("Main Loop for Gstreamer Running...\n");
  g_main_loop_run(loop);

  /* Free resources */
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  g_main_loop_unref(loop);
  g_dbus_node_info_unref(g_gdbus_introspection);

  pthread_exit(NULL);

  // TODO: notify IoT.js to set "ant.stream._mIsInitialized = false"
}

void ant_stream_initializeStream_internal() {
  pthread_create(&g_stream_thread, NULL, &stream_thread_fn, NULL);
}

void ant_stream_callDbusMethod_internal(const char *inputMessage,
                                        char *resultMessage) {
  GDBusConnection *connection;
  GVariant *value;
  gchar *inputMessageBuffer;
  const gchar *resultMessageBuffer;
  GError *gerror;

  gerror = NULL;
  connection = g_dbus_connection_new_for_address_sync(
      ANT_STREAMTHREAD_DBUS_ADDRESS,
      G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT,
      NULL, /* GDBusAuthObserver */
      NULL, /* GCancellable */
      &gerror);
  if (connection == NULL) {
    g_printerr("Error connecting to D-Bus address %s: %s\n",
               ANT_STREAMTHREAD_DBUS_ADDRESS, gerror->message);
    g_error_free(gerror);
    return;
  }

  inputMessageBuffer = g_strdup_printf(inputMessage);
  value = g_dbus_connection_call_sync(
      connection, NULL, /* bus_name */
      ANT_STREAMTHREAD_DBUS_PATH, ANT_STREAMTHREAD_DBUS_INTERFACE,
      ANT_STREAMTHREAD_DBUS_METHOD, g_variant_new("(s)", inputMessageBuffer),
      G_VARIANT_TYPE("(s)"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerror);
  if (value == NULL) {
    g_printerr("Error invoking HelloWorld(): %s\n", gerror->message);
    g_error_free(gerror);
    return;
  }
  g_variant_get(value, "(&s)", &resultMessageBuffer);
  snprintf(resultMessage, 100, resultMessageBuffer);
  g_variant_unref(value);

  g_object_unref(connection);
}

void initANTStream(void) {
  // Environment setting for opening of d-bus session bus
  setenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/run/dbus/system_bus_socket",
         1);
}

// void *test_pipeline_thread_fn(void *arg) {
//   GstElement *pipeline, *source, *converter, *omxh264enc, *h264parse,
//       *rtph264pay, *gdppay, *sink;
//   char *ipAddress;
//   ipAddress = g_ip_address;

//   /* Build the pipeline */
//   pipeline = gst_pipeline_new("test");
//   source = gst_element_factory_make("videotestsrc", NULL);
//   converter = gst_element_factory_make("videoconvert", NULL);
//   omxh264enc = gst_element_factory_make("omxh264enc", NULL);
//   h264parse = gst_element_factory_make("h264parse", NULL);
//   rtph264pay = gst_element_factory_make("rtph264pay", NULL);
//   gdppay = gst_element_factory_make("gdppay", NULL);
//   sink = gst_element_factory_make("tcpserversink", NULL);

//   if (!pipeline || !source || !converter || !omxh264enc || !h264parse ||
//       !rtph264pay || !gdppay || !sink) {
//     g_printerr("One element could not be created.\n");
//     return NULL;
//   }

//   g_object_set(G_OBJECT(source), "pattern", 1, NULL);
//   g_object_set(G_OBJECT(rtph264pay), "pt", 96, "config-interval", 1, NULL);
//   g_object_set(G_OBJECT(sink), "sync", FALSE, "host", ipAddress, "port",
//   5000,
//                NULL);

//   gst_bin_add_many(GST_BIN(pipeline), source, converter, omxh264enc,
//   h264parse,
//                    rtph264pay, gdppay, sink, NULL);
//   gst_element_link_many(source, converter, omxh264enc, h264parse, rtph264pay,
//                         gdppay, sink, NULL);

//   /* Start playing */
//   gst_element_set_state(pipeline, GST_STATE_PLAYING);
// }
