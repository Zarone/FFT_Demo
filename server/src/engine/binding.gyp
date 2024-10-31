{
  "targets": [
    {
      "cflags_cc": ["-std=c++20"],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          "-std=c++20",
        ],
      },
      "target_name": "engine",
      "sources": [ "./src/main.cpp", "./src/transform.h", "./src/transform.cpp", "./src/logger.cpp", "./src/logger.h" ],
    }
  ]
}
