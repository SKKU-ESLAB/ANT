#include "ant_stream_native_internal.h"
#include "../../../common/native/ant_common.h"

#include <gio/gio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gst/gst.h>

#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-protocol.h>

#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ANT_STREAMTHREAD_DBUS_BUS "org.ant.streamThread"
#define ANT_STREAMTHREAD_DBUS_PATH "/org/ant/streamThread"
#define ANT_STREAMTHREAD_DBUS_INTERFACE "org.ant.streamthread"
#define ANT_STREAMTHREAD_DBUS_METHOD "callMethod"

gchar *g_gdbus_address = NULL;
gchar *g_gdbus_tmpdir = NULL;

// I refered a gdlib tutorial for inter-thread communication
// https://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/gio/GDBusServer.html

static void handle_method_call_fn(GDBusConnection *, const gchar *,
                                  const gchar *, const gchar *, const gchar *,
                                  GVariant *, GDBusMethodInvocation *,
                                  gpointer);

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
GMainLoop *g_main_loop;

// TODO: element registry: hard-coded size.
// It should be changed to a registory of dynamic size.
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
    g_element_registry[index] = element;
    g_element_registry_index++;
    return index;
  }
}
GstElement *getElement(int index) { return g_element_registry[index]; }

// RPC functions
// TODO: hardcoded args, argv length
#define RPC_MAX_ARGC 10
#define MAX_ARG_LENGTH 100
void rpc_streamapi_quitMainLoop(int argc, char argv[][MAX_ARG_LENGTH],
                                char *responseMessage) {
  // Input arguments
  if (argc != 1) {
    g_printerr("Invalid arguments!\n");
    return;
  }

  // Internal
  g_main_loop_quit(g_main_loop);

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "true");
}

/* This function is called when an error message is posted on the bus */
static void error_cb(GstBus *bus, GstMessage *msg, void *data) {
  GError *err;
  gchar *debug_info;

  /* Print error details on the screen */
  gst_message_parse_error(msg, &err, &debug_info);
  g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src),
             err->message);
  g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
  g_clear_error(&err);
  g_free(debug_info);

  g_main_loop_quit(g_main_loop);
}

void rpc_streamapi_createPipeline(int argc, char argv[][MAX_ARG_LENGTH],
                                  char *responseMessage) {
  // On Stream Thread
  GstElement *pipeline;
  GstBus *bus;
  int element_index;
  const char *pipeline_name;

  // Input arguments
  if (argc != 2) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  pipeline_name = argv[1];

  // Internal
  pipeline = gst_pipeline_new(pipeline_name);
  element_index = registerElement(pipeline);

  bus = gst_element_get_bus(pipeline);
  gst_bus_add_signal_watch(bus);
  g_signal_connect(G_OBJECT(bus), "message::error", (GCallback)error_cb, NULL);
  gst_object_unref(bus);

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "%d", element_index);
}

void rpc_streamapi_createElement(int argc, char argv[][MAX_ARG_LENGTH],
                                 char *responseMessage) {
  // On Stream Thread
  GstElement *element;
  int element_index;
  const char *element_name;

  // Input arguments
  if (argc != 2) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  element_name = argv[1];

  // Internal
  element = gst_element_factory_make(element_name, NULL);
  element_index = registerElement(element);

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "%d", element_index);
}

void rpc_pipeline_binAdd(int argc, char argv[][MAX_ARG_LENGTH],
                         char *responseMessage) {
  // On Stream Thread
  GstElement *pipeline, *element;
  int pipeline_element_index;
  int element_index;
  gboolean result;

  // Input arguments
  if (argc != 3) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  sscanf(argv[1], "%d", &pipeline_element_index);
  sscanf(argv[2], "%d", &element_index);
  pipeline = getElement(pipeline_element_index);
  element = getElement(element_index);

  // Internal
  result = gst_bin_add(GST_BIN(pipeline), element);

  // Response message
  if (result) {
    snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "true");
  } else {
    snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "false");
  }
}

void rpc_pipeline_setState(int argc, char argv[][MAX_ARG_LENGTH],
                           char *responseMessage) {
  // On Stream Thread
  GstElement *pipeline;
  int pipeline_element_index;
  int state;
  GstStateChangeReturn result;

  // Input arguments
  if (argc != 3) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  sscanf(argv[1], "%d", &pipeline_element_index);
  sscanf(argv[2], "%d", &state);
  pipeline = getElement(pipeline_element_index);

  // Internal
  result = gst_element_set_state(pipeline, state);

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "%d", (int)result);
}

void rpc_pipeline_unref(int argc, char argv[][MAX_ARG_LENGTH],
                        char *responseMessage) {
  // On Stream Thread
  GstElement *pipeline;
  int pipeline_element_index;

  // Input arguments
  if (argc != 2) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  sscanf(argv[1], "%d", &pipeline_element_index);
  pipeline = getElement(pipeline_element_index);

  // Internal
  gst_object_unref(pipeline);

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "true");
}

void rpc_element_setProperty(int argc, char argv[][MAX_ARG_LENGTH],
                             char *responseMessage) {
  // On Stream Thread
  GstElement *element;
  int element_index;
  const char *key;
  int type;

  // Input arguments
  if (argc != 5) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  sscanf(argv[1], "%d", &element_index);
  key = argv[2];
  sscanf(argv[3], "%d", &type);
  element = getElement(element_index);

  // Internal
  switch (type) {
  case 0: { // boolean
    gboolean value_boolean;
    if (strncmp(argv[4], "true", MAX_ARG_LENGTH) == 0) {
      value_boolean = TRUE;
    } else {
      value_boolean = FALSE;
    }
    g_object_set(G_OBJECT(element), key, value_boolean, NULL);
    break;
  }
  case 1: { // string
    const char *value_string;
    value_string = argv[4];
    g_object_set(G_OBJECT(element), key, value_string, NULL);
    break;
  }
  case 2: { // integer
    int value_integer;
    sscanf(argv[4], "%d", &value_integer);
    g_object_set(G_OBJECT(element), key, value_integer, NULL);
    break;
  }
  case 3: { // float
    float value_float;
    sscanf(argv[4], "%f", &value_float);
    g_object_set(G_OBJECT(element), key, value_float, NULL);
    break;
  }
  }

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "true");
}

void rpc_element_setCapsProperty(int argc, char argv[][MAX_ARG_LENGTH],
                                 char *responseMessage) {
  // On Stream Thread
  GstCaps *caps;
  GstElement *element;
  int element_index;
  const char *key;
  const char *value;

  // Input arguments
  if (argc != 4) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  sscanf(argv[1], "%d", &element_index);
  key = argv[2];
  value = argv[3];
  element = getElement(element_index);

  // Internal
  caps = gst_caps_from_string(value);
  g_object_set(G_OBJECT(element), key, caps, NULL);
  gst_caps_unref(caps);

  // Response message
  snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "true");
}

void rpc_element_link(int argc, char argv[][MAX_ARG_LENGTH],
                      char *responseMessage) {
  // On Stream Thread
  GstElement *src_element, *dest_element;
  int src_element_index;
  int dest_element_index;
  gboolean result;

  // Input arguments
  if (argc != 3) {
    g_printerr("Invalid arguments!\n");
    return;
  }
  sscanf(argv[1], "%d", &src_element_index);
  sscanf(argv[2], "%d", &dest_element_index);
  src_element = getElement(src_element_index);
  dest_element = getElement(dest_element_index);

  // Internal
  result = gst_element_link(src_element, dest_element);

  // Response message
  if (result) {
    snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "true");
  } else {
    snprintf(responseMessage, MAX_RESULT_MESSAGE_LENGTH, "false");
  }
}

void handle_method_call_internal(int argc, char argv[][MAX_ARG_LENGTH],
                                 char *responseMessage) {
  if (argc == 0) {
    g_printerr("Empty arguments!\n");
    return;
  }
  if (strncmp(argv[0], "streamapi_quitMainLoop", MAX_ARG_LENGTH) == 0) {
    rpc_streamapi_quitMainLoop(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "streamapi_createPipeline", MAX_ARG_LENGTH) ==
             0) {
    rpc_streamapi_createPipeline(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "streamapi_createElement", MAX_ARG_LENGTH) == 0) {
    rpc_streamapi_createElement(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "pipeline_binAdd", MAX_ARG_LENGTH) == 0) {
    rpc_pipeline_binAdd(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "pipeline_setState", MAX_ARG_LENGTH) == 0) {
    rpc_pipeline_setState(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "pipeline_unref", MAX_ARG_LENGTH) == 0) {
    rpc_pipeline_unref(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "element_setProperty", MAX_ARG_LENGTH) == 0) {
    rpc_element_setProperty(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "element_setCapsProperty", MAX_ARG_LENGTH) == 0) {
    rpc_element_setCapsProperty(argc, argv, responseMessage);
  } else if (strncmp(argv[0], "element_link", MAX_ARG_LENGTH) == 0) {
    rpc_element_link(argc, argv, responseMessage);
  } else {
    g_printerr("Invalid method call!: %s\n", argv[0]);
  }
}

static void
handle_method_call_fn(GDBusConnection *connection, const gchar *sender,
                      const gchar *object_path, const gchar *interface_name,
                      const gchar *method_name, GVariant *parameters,
                      GDBusMethodInvocation *invocation, gpointer user_data) {
  if (g_strcmp0(method_name, ANT_STREAMTHREAD_DBUS_METHOD) == 0) {
    const gchar *inputMessage;
    int argc = 0;
    char argv[RPC_MAX_ARGC][MAX_ARG_LENGTH];
    char responseMessage[MAX_RESULT_MESSAGE_LENGTH] = "";
    g_variant_get(parameters, "(&s)", &inputMessage);

    // Parsing arguments
    {
      char *token;
      token = strtok((char *)inputMessage, "\n");
      while (token != NULL) {
        if (argc < RPC_MAX_ARGC) {
          snprintf(argv[argc], MAX_ARG_LENGTH, "%s", token);
        }
        argc++;
        token = strtok(NULL, "\n");
      }
    }
    // g_print("Incoming method call message:%s\n", argv[0]);
    handle_method_call_internal(argc, argv, responseMessage);

    g_dbus_method_invocation_return_value(
        invocation, g_variant_new("(s)", responseMessage));
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

void *stream_thread_fn(void *arg) {
  // On Stream Thread
  GDBusServer *gdbus_server;

  /* Initialize GStreamer */
  gst_init(NULL, NULL);
  g_main_loop = g_main_loop_new(NULL, FALSE);

  /* Initialize gdbus filter */
  g_gdbus_introspection =
      g_dbus_node_info_new_for_xml(g_introspection_xml, NULL);
  g_assert(g_gdbus_introspection != NULL);
  {
    gchar *gdbus_guid;
    GError *gerror;

    gdbus_guid = g_dbus_generate_guid();
    gerror = NULL;

    g_gdbus_tmpdir = g_dir_make_tmp("gdbus-ant-XXXXXX", NULL);
    g_gdbus_address =
        g_strdup_printf("unix:path=%s/gdbus-peer-socket", g_gdbus_tmpdir);
    gdbus_server = g_dbus_server_new_sync(
        g_gdbus_address, G_DBUS_SERVER_FLAGS_AUTHENTICATION_ALLOW_ANONYMOUS,
        gdbus_guid, NULL, NULL, &gerror);
    g_gdbus_address = g_strdup(g_dbus_server_get_client_address(gdbus_server));
    g_dbus_server_start(gdbus_server);
    g_free(gdbus_guid);

    if (gdbus_server == NULL) {
      g_printerr("Error creating server at address %s: %s\n", g_gdbus_address,
                 gerror->message);
      g_error_free(gerror);
      return NULL;
    }
    g_signal_connect(gdbus_server, "new-connection",
                     G_CALLBACK(on_new_connection_fn), NULL);
  }

  /* gstreamer main loop: wait until error or EOS */
  g_print("Stream thread launched!\n");
  g_main_loop_run(g_main_loop);
  g_print("Stream thread terminated!\n");

  /* Free resources */
  // TODO: free gstremaer pipeline, elements
  g_main_loop_unref(g_main_loop);
  g_dbus_node_info_unref(g_gdbus_introspection);

  g_dbus_server_stop(gdbus_server);

  g_free(g_gdbus_address);
  if (g_gdbus_tmpdir != NULL) {
    g_rmdir(g_gdbus_tmpdir);
    g_free(g_gdbus_tmpdir);
  }

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

  if (g_gdbus_address == NULL) {
    g_printerr("GDBus server is not set!");
    return;
  }

  gerror = NULL;
  connection = g_dbus_connection_new_for_address_sync(
      g_gdbus_address, G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT,
      NULL, /* GDBusAuthObserver */
      NULL, /* GCancellable */
      &gerror);
  if (connection == NULL) {
    g_printerr("Error connecting to D-Bus address %s: %s\n", g_gdbus_address,
               gerror->message);
    g_error_free(gerror);
    return;
  }

  inputMessageBuffer = g_strdup_printf("%s", inputMessage);
  value = g_dbus_connection_call_sync(
      connection, NULL, /* bus_name */
      ANT_STREAMTHREAD_DBUS_PATH, ANT_STREAMTHREAD_DBUS_INTERFACE,
      ANT_STREAMTHREAD_DBUS_METHOD, g_variant_new("(s)", inputMessageBuffer),
      G_VARIANT_TYPE("(s)"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerror);
  if (value == NULL) {
    g_printerr("Error invoking method: %s\n", gerror->message);
    g_error_free(gerror);
    return;
  }
  g_variant_get(value, "(&s)", &resultMessageBuffer);
  snprintf(resultMessage, MAX_RESULT_MESSAGE_LENGTH, "%s", resultMessageBuffer);
  g_variant_unref(value);

  g_object_unref(connection);
}

/* The appsink has received a buffer */
// TODO: Hardcoding: unique handler
ant_async_handler g_ant_async_handler;
static GstFlowReturn gst_signal_handler_ant_async(GstElement *element,
                                                  char *element_name) {
  // gst signal handler -> call ant async handler
  GstSample *sample;

  /* Retrieve the buffer */
  g_signal_emit_by_name(element, "pull-sample", &sample);
  if (sample) {
    GstBuffer *buffer;
    GstMapInfo info;
    gboolean mapping_result;
    /* The only thing we do in this example is print a * to indicate a received
     * buffer */
    buffer = gst_sample_get_buffer(sample);
    mapping_result = gst_buffer_map(buffer, &info, GST_MAP_READ);
    if (mapping_result) {
      // call ant async handler with data
      g_ant_async_handler(element_name, info.data, info.size);
      gst_buffer_unmap(buffer, &info);
    } else {
      g_printerr("gst_buffer_map not successful...\n");
    }
    gst_sample_unref(sample);
    return GST_FLOW_OK;
  } else {
    g_printerr("GST_FLOW_ERROR occurs\n");
    return GST_FLOW_ERROR;
  }
}

bool ant_stream_elementConnectSignal_internal(
    int element_index, const char *detailed_signal,
    ant_async_handler _ant_async_handler) {
  GstElement *element;
  gchar *element_name;
  gulong result;

  element = getElement(element_index);
  element_name = g_strdup_printf("%s", gst_element_get_name(element));

  g_ant_async_handler = _ant_async_handler;

  result =
      g_signal_connect(element, detailed_signal,
                       G_CALLBACK(gst_signal_handler_ant_async), element_name);
  if (result > 0) {
    return true;
  } else {
    return false;
  }
}

void initANTStream(void) {
  // Environment setting for opening of d-bus session bus
  setenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/run/dbus/system_bus_socket",
         1);
}