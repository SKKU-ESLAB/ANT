{
  'targets': [
    {
      'target_name': 'sensor-api',
      'sources': [
        'src/nil.cc',
        'src/nil_dbus_connection.cc',
		    'src/nil_dbus.cc',
		    'src/nil_request.cc',
		    'src/nil.h',
        'src/cJSON.h',
        'src/cJSON.c',
        'src/sensfw_log.h'
       ],
      'libraries': [
        '<!@(pkg-config glib-2.0 --libs)',
        '<!@(pkg-config dbus-1 --libs)'
      ],
      'include_dirs': [
        '<!@(pkg-config glib-2.0 --cflags-only-I | sed s/-I//g)',
        '<!@(pkg-config dbus-1 --cflags-only-I | sed s/-I//g)',
        'inc'
      ]
    }
  ]
}


