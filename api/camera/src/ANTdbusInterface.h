#ifndef ANT_DBUS_INTERFACE
#define ANT_DBUS_INTERFACE
#include <string>

static const char *dbus_path = "/org/ant/camera/daemon";
static const char *dbus_interface = "org.ant.camera.daemon";

static const char *rec_start_request = "recStart";
static const char *rec_stop_request = "recStop";

static const char *snap_start_request = "jpegStart";

static const char *opencv_start_request = "openCVStart";
static const char *opencv_stop_request = "openCVStop";

static const char *streaming_start_request = "streamingStart";
static const char *streaming_stop_request = "streamingStop";

static const char *sensor_overlay_start_request = "sensorOverlayStart";
static const char *sensor_overlay_stop_request = "sensorOverlayStop";

static const char *pre_rec_init_request = "preRecordingInit";
static const char *pre_rec_start_request = "preRecordingStart";
static const char *pre_rec_stop_request = "preRecordingStop";


typedef struct _dbusRequest {
  unsigned pid;
  unsigned camera_id;
  unsigned play_seconds;
  std::string file_path;
  unsigned width;
  unsigned height;
  unsigned fps;
} dbusRequest;

typedef struct _dbusStreamingRequest {
  unsigned pid;
  unsigned camera_id;
  std::string ip_address;
  unsigned port;
} dbusStreamingRequest;

typedef struct _dbusSensorOverlayRequest {
  unsigned pid;
  unsigned camera_id;
  std::string sensor_name;
} dbusSensorOverlayRequest;

#endif /* ANT_DBUS_INTERFACE */
