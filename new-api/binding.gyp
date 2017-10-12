{
  'targets': [
    {
      'target_name': 'ant',
      'sources': [
        'src/AppBase.cpp',
        'src/API.cpp',
        'src/AppAPI.cpp',
        'src/AppAPIInternal.cpp',
        'src/MLAPI.cpp',
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
