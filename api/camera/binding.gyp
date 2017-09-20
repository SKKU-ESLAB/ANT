{
  "targets": [
    {
      "target_name": "camera-api",
      "sources": ["src/ANTRecordingAPI.cc", "src/ANTRecordingAPI.h"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")", "src"
      ],
			"cflags" : ["-Wno-unused-variable"],
			"cflags!" : ["-fno-exceptions"],
			"cflags_cc!" : ["-fno-exceptions"],
			"libraries" : ["<!@(pkg-config dbus-1 --libs)", "<!@(pkg-config glib-2.0 --libs)"],
			"cflags" : ["<!@(pkg-config --cflags dbus-1)"],
			"cflags_cc" : ["<!@(pkg-config --cflags dbus-1)"], 
			"cflags_cc!" : ["-fno-rtti"],
			"cflags_cc+" : ["-frtti"]
    }
  ]
}
