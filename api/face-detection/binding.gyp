{
  "targets": [
    {
      "target_name": "face-detection-api",
      "sources": ["src/addon.cc", "src/ANTFaceDetection.cc", "src/ANTFaceDetection.h"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")", "src", "/usr/local/include"
      ],
			"cflags" : ["-Wno-unused-variable"],
			"cflags!" : ["-fno-exceptions"],
			"cflags_cc!" : ["-fno-exceptions"],
			"libraries" : ["<!@(pkg-config dbus-1 --libs)", "<!@(pkg-config glib-2.0 --libs)", "<!@(pkg-config opencv --libs)"],
			"cflags" : ["<!@(pkg-config --cflags dbus-1)", "<!@(pkg-config --cflags opencv)"],
			"cflags_cc" : ["<!@(pkg-config --cflags dbus-1)", "<!@(pkg-config --cflags opencv)"], 
			"cflags_cc!" : ["-fno-rtti"],
			"cflags_cc+" : ["-frtti"]
    }
  ]
}
