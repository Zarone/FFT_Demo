#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

#include "transform.h"
#include "logger.h"

using std::vector;

vector<vector<int16_t>> transformWAVData(const vector<int16_t>& data) {

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
  vector<vector<int16_t>> value = transformAmplitudeData(data, numElements);

  // Add header info back
  for (int i = 0; i < numElements; i++) {
    for (size_t j = 0; j < HEADER_OFFSET; j++) {
      value[i][j] = data[j];
    }
  }

  return value;
}


vector<vector<int16_t>> transformAmplitudeData(const vector<int16_t>& data, int& numElements) {
  numElements = 2;
  size_t len = data.size();

  vector<vector<int16_t>> value(numElements, vector<int16_t>(data.size()));

  for (size_t i = 0; i<HEADER_OFFSET; i++) {
    value[0][i] = 0;
  }

  vector<std::complex<double>> frequencyData = DFT(data);
  
  empty_file("./logs/DFT.log");
  for (size_t i = HEADER_OFFSET; i < len; i++) {
    value[0][i] = (int16_t) hypot(frequencyData[i-HEADER_OFFSET].imag(), frequencyData[i-HEADER_OFFSET].real());
    file_logger("./logs/DFT.log", value[0][i]); 
  }

  empty_file("./logs/InverseDFT.log");
  vector<int16_t> recreation = InverseDFT(frequencyData);
  for (size_t i = HEADER_OFFSET; i < len; i++) {
    value[1][i] = recreation[i-HEADER_OFFSET];
    file_logger("./logs/InverseDFT.log", value[1][i]); 
  }
  
  return value;
}

vector<std::complex<double>> DFT(const vector<int16_t>& data) {
  size_t len = data.size() - HEADER_OFFSET;
  const double NEGATIVE_TWO_PI_OVER_LEN = -2*M_PI/len;

  vector<std::complex<double>> output(len);

  for (size_t i = 0; i < len; i++) {
    std::complex<double> sum = 0;
    for (size_t j = 0; j < len; j++) {
      double dataValue = static_cast<double>(data[j+HEADER_OFFSET]);
      
      // improves frequency resolution sometimes
      dataValue *= windowingFunction(j, len);

      sum += static_cast<double>(dataValue) * exp(
        std::complex<double>(0.0, i*j*NEGATIVE_TWO_PI_OVER_LEN) 
      );
    }
    output[i] = sum;
  }

  return output;
}

vector<int16_t> InverseDFT(const vector<std::complex<double>>& data) {
  size_t len = data.size();
  const double TWO_PI_OVER_LEN = 2*M_PI/len;

  vector<int16_t> output(len);

  for (size_t i = 0; i < len; i++) {
    double sum = 0;
    for (size_t j = 0; j < len; j++) {
      std::complex<double> scale = exp(
        std::complex<double>(0.0, i*j*TWO_PI_OVER_LEN) 
      );
      std::complex<double> x_t = (data[j] * scale);
      sum += (double)(x_t.real());
    }
    output[i] = sum/((double)len)/windowingFunction(i, len);
  }

  return output;
}

double windowingFunction(double index, double length) {
  // Hamming Window
  return 0.54 - 0.46 * std::cos(2 * M_PI * index / (length-1));
}

