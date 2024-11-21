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
      "sources": [ 
        "./src/main.cpp", 
        "./src/transform.h", 
        "./src/transform.cpp", 
        "./src/logger.h", 
        "./src/logger.cpp",
        "./src/DFT.cpp",
        "./src/DFT.h",
        "./src/FFT.cpp",
        "./src/FFT.h"
      ],
    }
  ]
}
