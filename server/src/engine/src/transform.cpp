#include <cstdlib>
#include <cstdio>
#include <vector>

#include "transform.h"

std::vector<std::vector<int16_t>> transformWAVData(const std::vector<int16_t>& data) {
  if (data.size() < 22) {
    printf("WAV data is too small to contain header information.\n");
  }

  int bitsPerSample = data[17]; // bytes 35 and 36
  int bytesPerSample = bitsPerSample/8;
  if (bitsPerSample != 16) {
    printf("Expected 16 bits per sample, but got %i.\n", bitsPerSample);
  }

  int numElements;
  std::vector<std::vector<int16_t>> value = transformAmplitudeData(data, numElements);

  // Add header info back
  for (int i = 0; i < numElements; i++) {
    for (size_t j = 0; j < HEADER_OFFSET; j++) {
      value[i][j] = data[j];
    }
  }

  return value;
}


std::vector<std::vector<int16_t>> transformAmplitudeData(const std::vector<int16_t>& data, int& numElements) {
  // TODO: Actually write this method
  // It's just returns some random sin wave data right now

  numElements = 1;
  size_t len = data.size();

  std::vector<std::vector<int16_t>> value(numElements, std::vector<int16_t>(data.size()));

  for (size_t i = 0; i<HEADER_OFFSET; i++) {
    value[0][i] = 0;
  }

  std::vector<std::complex<double>> frequencyData = DFT(data);
  
  for (size_t i = HEADER_OFFSET; i < len; i++) {
    value[0][i] = (int16_t) hypot(frequencyData[i-HEADER_OFFSET].imag(), frequencyData[i-HEADER_OFFSET].real());
  }
  
  return value;
}

std::vector<std::complex<double>> DFT(const std::vector<int16_t>& data) {
  size_t len = data.size() - HEADER_OFFSET;
  std::vector<std::complex<double>> output(len);
  for (size_t i = 0; i < len; i++) {
    std::complex<double> sum = 0;
    for (size_t j = 0; j < len; j++) {
      sum += (double)data[j+HEADER_OFFSET] * exp(
        std::complex<double>(0.0, -2*M_PI*i*j / len) 
      );
    }
    output[i] = sum;
  }

  return output;
}
