#include <complex>
#include <vector>

#include "FFT.h"

using std::vector;
using std::complex;

void rawFFTSpaceSaving(complex<double>* data, size_t n, size_t stride) {

  // base case
  if (n == 1) {
    return;
  }

  const double NEGATIVE_TWO_PI_OVER_LEN = -2*M_PI/n;

  rawFFTSpaceSaving(data, n/2, stride*2);
  rawFFTSpaceSaving(data+stride, n/2, stride*2);

  // minor optimization
  complex<double> shift = exp(
    complex<double>(0.0, NEGATIVE_TWO_PI_OVER_LEN) 
  ); 
  complex<double> cur = complex<double>(1.0, 0.0);

  for (size_t i = 0; i < n/2; ++i) {
    complex<double> even_value = data[i*2*stride];
    complex<double> odd_value = data[i*2*stride + stride];

    complex<double> odd_multiple = odd_value * cur;
    cur *= shift;

    data[i*stride] = even_value + odd_multiple;
    data[(i+n/2)*stride] = even_value - odd_multiple;
  }
  
}


// uses pointers instead of vectors
void rawFFT(complex<double>* data, size_t n) {

  // base case
  if (n == 1) {
    return;
  } 

  const double NEGATIVE_TWO_PI_OVER_LEN = -2*M_PI/n;

  complex<double> odd[n/2];
  complex<double> even[n/2];
  for (size_t i = 0; i < n; i+=2) {
    odd[i/2] = data[i+1];
    even[i/2] = data[i];
  }

  rawFFT(odd, n/2);
  rawFFT(even, n/2);

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

vector<complex<double>> FFTPadding(const vector<int16_t>& data) {
  size_t len = data.size();
  size_t next_radix_2 = 1;
  while (next_radix_2 < len) next_radix_2 <<= 1; // Round up to the next power of 2

  /*   Zero pad   */
  /**/
  complex<double> raw_data[next_radix_2];
  vector<complex<double>> output(next_radix_2);
  for (size_t i = 0; i < len; ++i) {
    raw_data[i] = data[i];
  }
  for (size_t i = len; i < next_radix_2; ++i) {
    raw_data[i] = 0;
  }
  /**/

  /*   Remove Elements   */ 
  /*
  next_radix_2/=2;
  vector<complex<double>> output(next_radix_2);
  complex<double> raw_data[next_radix_2];
  for (size_t i = 0; i < next_radix_2; ++i) {
    raw_data[i] = data[i];
  }
  */

  rawFFTSpaceSaving(raw_data, next_radix_2, 1);

  for (size_t i = 0; i < next_radix_2; ++i) {
    output[i] = raw_data[i];
  }

  return output;
}

// uses pointers instead of vectors
void rawIFFT(complex<double>* data, size_t n) {
  const double TWO_PI_OVER_LEN = 2*M_PI/n;

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

    rawIFFT(odd, n/2);
    rawIFFT(even, n/2);

    // minor optimization
    complex<double> shift = exp(
      complex<double>(0.0, TWO_PI_OVER_LEN) 
    ); 
    complex<double> cur = complex<double>(1.0, 0.0);

    for (size_t i = 0; i < n/2; i++) {
      complex<double> odd_multiple = odd[i] * cur;
      cur *= shift;

      data[i] = even[i] + odd_multiple;
      data[i+n/2] = even[i] - odd_multiple;
    }

  }
}

vector<int16_t> IFFTPadding(const vector<complex<double>>& data) {
  size_t len = data.size();
  size_t next_radix_2 = 1;
  while (next_radix_2 < len) next_radix_2 <<= 1; // Round up to the next power of 2

  if (len != next_radix_2) {
    printf("len %zi should be next_radix_2 %zi\n", len, next_radix_2);
    exit(EXIT_FAILURE);
  }

  complex<double> raw_data[len];
  vector<int16_t> output(len);
  for (size_t i = 0; i < len; ++i) {
    raw_data[i] = data[i];
  }

  rawIFFT(raw_data, next_radix_2);

  for (size_t i = 0; i < len; ++i) {
    output[i] = (double)(raw_data[i].real())/len;
  }

  return output;
}

