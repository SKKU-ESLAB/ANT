{
  'targets': [
    {
      'target_name': 'antnative',
      'sources': [
        'src/AppBase.cpp',
        'src/ANTNativeAPI.cpp',
        'src/AppNativeAPI.cpp',
        'src/MLNativeAPI.cpp',
        'src/CommNativeAPI.cpp',
        'src/ResourceNativeAPI.cpp'
       ],
      'link_settings': {
        'libraries': [
          '<!@(pkg-config glib-2.0 --libs)',
          '<!@(pkg-config dbus-1 --libs)',
          "-L<(PRODUCT_DIR)/../../../out/libs -lant-cmfw",
          "-L<(PRODUCT_DIR)/../../../out/libs -lant-message",
        ],
        'ldflags': [
          '-Wl,-rpath'
        ]
      },
      'include_dirs': [
        './inc',
        '<!@(pkg-config glib-2.0 --cflags-only-I | sed s/-I//g)',
        '<!@(pkg-config dbus-1 --cflags-only-I | sed s/-I//g)',
        '../framework/message/inc',
        '../framework/communication/inc'
      ]
    }
  ]
}