#include <complex>
#include <vector>

#include "FFT.h"

using std::vector;
using std::complex;

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

  raw_FFT_padding(raw_data, next_radix_2);

  for (size_t i = 0; i < next_radix_2; ++i) {
    output[i] = raw_data[i];
  }

  return output;
}

// uses pointers instead of vectors
void raw_IFFT_padding(complex<double>* data, size_t n) {
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

    raw_IFFT_padding(odd, n/2);
    raw_IFFT_padding(even, n/2);

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

vector<int16_t> IFFT_padding(const vector<complex<double>>& data) {
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

  raw_IFFT_padding(raw_data, next_radix_2);

  for (size_t i = 0; i < len; ++i) {
    output[i] = (double)(raw_data[i].real())/len;
  }

  return output;
}

