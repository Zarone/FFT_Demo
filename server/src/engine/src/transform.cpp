#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

#include "transform.h"
#include "logger.h"

using std::vector;
using std::complex;

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

   // Get starting timepoint
  auto start = std::chrono::high_resolution_clock::now();

  numElements = 2;
  size_t len = data.size();

  //vector<vector<int16_t>> value(numElements, vector<int16_t>(data.size()));
  vector<vector<int16_t>> value(numElements, vector<int16_t>(HEADER_OFFSET));

  //for (size_t i = 0; i<HEADER_OFFSET; i++) {
    //value[0][i] = 0;
  //}

  vector<int16_t> stripped_data = vector<int16_t>(data.begin()+HEADER_OFFSET, data.end());

  //vector<complex<double>> frequencyData = DFT(stripped_data);
  
  //empty_file("./logs/DFT.log");
  //for (size_t i = HEADER_OFFSET; i < len; i++) {
    //value[0][i] = (int16_t) hypot(frequencyData[i-HEADER_OFFSET].imag(), frequencyData[i-HEADER_OFFSET].real());
    //file_logger("./logs/DFT.log", value[0][i]); 
  //}
  
  vector<complex<double>> frequencyData = FFT_padding(stripped_data);
  len = frequencyData.size() + HEADER_OFFSET;
  value[0] = vector<int16_t>(len);
  
  empty_file("./logs/FFTp.log");
  for (size_t i = HEADER_OFFSET; i < len; i++) {
    value[0][i] = (int16_t) hypot(frequencyData[i-HEADER_OFFSET].imag(), frequencyData[i-HEADER_OFFSET].real());
    file_logger("./logs/FFTp.log", value[0][i]); 
  }


  vector<int16_t> recreation = InverseDFT(frequencyData);
  len = recreation.size() + HEADER_OFFSET;
  value[1] = vector<int16_t>(len);

  empty_file("./logs/InverseDFT.log");
  for (size_t i = HEADER_OFFSET; i < len; i++) {
    value[1][i] = recreation[i-HEADER_OFFSET];
    file_logger("./logs/InverseDFT.log", value[1][i]); 
  }

  //vector<int16_t> recreation = IFFT_padding(frequencyData);
  //len = recreation.size() + HEADER_OFFSET;
  //value[1] = vector<int16_t>(len);

  //empty_file("./logs/InverseFFTp.log");
  //for (size_t i = HEADER_OFFSET; i < len; i++) {
    //value[1][i] = recreation[i-HEADER_OFFSET];
    //file_logger("./logs/InverseFFTp.log", value[1][i]); 
  //}

  auto end = std::chrono::high_resolution_clock::now();

  auto dur_mic = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  auto dur_mil = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto dur_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);

  std::cout << "Transform took: " << dur_mic.count() << " microseconds" << std::endl;
  std::cout << "Transform took: " << dur_mil.count() << " milliseconds" << std::endl;
  std::cout << "Transform took: " << dur_sec.count() << " seconds" << std::endl;

  return value;
}

// uses pointers instead of vectors
void raw_FFT_padding(complex<double>* data, size_t n) {
  const double NEGATIVE_TWO_PI_OVER_LEN = -2*M_PI/n;

  // base case
  if (n == 1) {
    return;
  } else {
    complex<double> odd[n/2];
    complex<double> even[n/2];
    for (size_t i = 0; i < n; i+=2) {
      odd[i/2] = data[i+1];
      even[i/2] = data[i];
    }

    raw_FFT_padding(odd, n/2);
    raw_FFT_padding(even, n/2);

    // minor optimization
    complex<double> shift = exp(
      complex<double>(0.0, NEGATIVE_TWO_PI_OVER_LEN) 
    ); 
    complex<double> cur = complex<double>(1.0, 0.0);

    for (size_t i = 0; i < n/2; i++) {
      //complex<double> odd_multiple = odd[i] * exp(
        //complex<double>(0.0, i*NEGATIVE_TWO_PI_OVER_LEN) 
      //);
      complex<double> odd_multiple = odd[i] * cur;
      cur *= shift;

      data[i] = even[i] + odd_multiple;
      data[i+n/2] = even[i] - odd_multiple;
    }

  }
}

vector<complex<double>> FFT_padding(const vector<int16_t>& data) {
  size_t len = data.size();
  size_t next_radix_2 = 1;
  while (next_radix_2 < len) next_radix_2 <<= 1; // Round up to the next power of 2

  /*   Zero pad   */
  /*
  vector<complex<double>> output(next_radix_2);
  for (size_t i = 0; i < len; ++i) {
    raw_data[i] = data[i];
  }
  for (size_t i = len; i < next_radix_2; ++i) {
    raw_data[i] = 0;
  }
  */

  /*   Remove Elements   */ 
  /**/
  next_radix_2/=2;
  vector<complex<double>> output(next_radix_2);
  complex<double> raw_data[next_radix_2];
  for (size_t i = 0; i < next_radix_2; ++i) {
    raw_data[i] = data[i];
  }
  /**/

  raw_FFT_padding(raw_data, next_radix_2);

  for (size_t i = 0; i < next_radix_2; ++i) {
    output[i] = raw_data[i];
  }

  return output;
}

vector<complex<double>> DFT(const vector<int16_t>& data) {
  size_t len = data.size();
  const double NEGATIVE_TWO_PI_OVER_LEN = -2*M_PI/len;

  vector<complex<double>> output(len);

  for (size_t i = 0; i < len; i++) {
    complex<double> sum = 0;
    for (size_t j = 0; j < len; j++) {
      double dataValue = static_cast<double>(data[j]);
      
      // improves frequency resolution sometimes
      dataValue *= windowingFunction(j, len);

      sum += static_cast<double>(dataValue) * exp(
        complex<double>(0.0, i*j*NEGATIVE_TWO_PI_OVER_LEN) 
      );
    }
    output[i] = sum;
  }

  return output;
}

vector<int16_t> InverseDFT(const vector<complex<double>>& data) {
  size_t len = data.size();
  const double TWO_PI_OVER_LEN = 2*M_PI/len;

  vector<int16_t> output(len);

  for (size_t i = 0; i < len; i++) {
    double sum = 0;
    for (size_t j = 0; j < len; j++) {
      complex<double> scale = exp(
        complex<double>(0.0, i*j*TWO_PI_OVER_LEN) 
      );
      complex<double> x_t = (data[j] * scale);
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

