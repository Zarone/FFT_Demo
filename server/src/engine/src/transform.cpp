#include <cstdlib>
#include <cstdio>
#include <math.h>

#include "transform.h"

int16_t** transformWAVData(int16_t* data, size_t length) {
  int bitsPerSample = data[17]; // bytes 35 and 36
  int bytesPerSample = bitsPerSample/8;
  if (bitsPerSample != 16) {
    printf("Expected 16 bits per sample, but got %i\n", bitsPerSample);
  }

  int startOffset = 22; // 44 bytes at start of wav file are header data

  int numElements;
  int16_t** value = transformAmplitudeData(data+startOffset, length, &numElements);

  // Add header info back
  for (int i = 0; i < numElements; i++) {
    for (int j = 0; j < startOffset; j++) {
      value[i][j] = data[j];
    }
  }



  return value;
}


int16_t** transformAmplitudeData(int16_t* data, size_t length, int* numElements) {
  // TODO: Actually write this method
  // It's just returns some random sin wave data right now
  // This is the FFT method, so implement it

  int16_t** value = (int16_t**) calloc(1, sizeof(int16_t*));
  *numElements = 1;
  //value[0] = data;
  value[0] = (int16_t*) calloc(length, sizeof(int16_t));
  for (int i = 0; i<22; i++) {
    value[0][i] = 0;
  }
  for (size_t i = 22; i < length; i++) {
    int sound = pow(2,15)/3*(sin((float)(i)/5000 * 261.63) + sin((float)(i)/5000 * 329.63) + sin((float)(i)/5000 * 392));
    value[0][i] = sound;
  }
  
  return value;
}
