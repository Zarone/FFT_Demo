#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <chrono>

#include "transform.h"
#include "logger.h"
#include "FFT.h"
#include "DFT.h"

using std::vector;
using std::complex;

vector<vector<int16_t>> transformWAVData(const vector<int16_t>& data, bool fast) {

  // Error case: too small
  if (data.size() < HEADER_OFFSET) {
    std::cout << "WAV data is too small to contain header information." << std::endl;
  }

  // Error case: bad header 
  int bitsPerSample = data[17]; // bytes 35 and 36
  if (bitsPerSample != 16) {
    printf("Expected 16 bits per sample, but got %i.\n", bitsPerSample);
    return (vector<vector<int16_t>>)0;
  }

  // Let transformAmplitudeData decide the number of elements
  int numElements;
  vector<vector<int16_t>> value = transformAmplitudeData(data, numElements, fast);

  // Add header info back
  for (int i = 0; i < numElements; i++) {
    for (size_t j = 0; j < HEADER_OFFSET; j++) {
      value[i][j] = data[j];
    }
  }

  return value;
}

vector<vector<int16_t>> transformAmplitudeData(const vector<int16_t>& data, int& numElements, bool fast) {
  printf("fast = %i\n", fast);

   // Get starting timepoint
  auto start = std::chrono::high_resolution_clock::now();

  numElements = 2;
  const size_t len = data.size();

  vector<vector<int16_t>> value(numElements);

  vector<int16_t> stripped_data = vector<int16_t>(data.begin()+HEADER_OFFSET, data.end());

  vector<complex<double>> frequencyData;
  
  performFourierTransform(value[0], frequencyData, stripped_data, len, fast, false);
  
  performInverseFourierTransform(value[1], frequencyData, len, fast, false);

  auto end = std::chrono::high_resolution_clock::now();

  auto dur_mic = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  auto dur_mil = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto dur_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);

  std::cout << "Transform took: " << dur_mic.count() << " microseconds" << std::endl;
  std::cout << "Transform took: " << dur_mil.count() << " milliseconds" << std::endl;
  std::cout << "Transform took: " << dur_sec.count() << " seconds" << std::endl;

  return value;
}

void performInverseFourierTransform(vector<int16_t>& outputReference, vector<complex<double>>& inputData, size_t len, bool fast, bool windowed) {
  vector<int16_t> recreation;
  if (!fast) {
    recreation = inverseDFT(inputData, windowed);
  } else {
    recreation = IFFTPadding(inputData);
  }

  size_t rec_len = recreation.size() + HEADER_OFFSET;
  outputReference = vector<int16_t>(len);
  if (!fast) {
    empty_file("./logs/InverseDFT.log");
  } else {
    empty_file("./logs/InverseFFTp.log");
  }

  for (size_t i = HEADER_OFFSET; i < std::min(rec_len, len); i++) {
    outputReference[i] = recreation[i-HEADER_OFFSET];
    if (!fast) {
      file_logger("./logs/InverseDFT.log", outputReference[i]); 
    } else {
      file_logger("./logs/InverseFFTp.log", outputReference[i]); 
    }
  }
  for (size_t i = rec_len; i < len; ++i) {
    outputReference[i] = 0;
  }
}

void performFourierTransform(vector<int16_t>& outputReference, vector<complex<double>>& frequencyData, vector<int16_t> inputData, size_t len, bool fast, bool windowed) {
  if (!fast) {
    frequencyData = DFT(inputData, windowed);
  } else {
    frequencyData = FFTPadding(inputData);
  }

  size_t freq_len = frequencyData.size() + HEADER_OFFSET;
  outputReference = vector<int16_t>(std::max(freq_len, len));

  if (!fast) {
    empty_file("./logs/DFT.log");
  } else {
    empty_file("./logs/FFTp.log");
  }

  for (size_t i = HEADER_OFFSET; i < freq_len; i++) {
    outputReference[i] = (int16_t) hypot(frequencyData[i-HEADER_OFFSET].imag(), frequencyData[i-HEADER_OFFSET].real());
    if (!fast) {
      file_logger("./logs/DFT.log", outputReference[i]); 
    } else {
      file_logger("./logs/FFTp.log", outputReference[i]); 
    }
  }
  for (size_t i = freq_len; i < len; i++) {
    outputReference[i] = 0;
  }
}
