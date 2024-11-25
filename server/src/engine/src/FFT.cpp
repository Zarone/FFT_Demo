#include <complex>
#include <vector>

#include "FFT.h"

using std::vector;
using std::complex;

int bit_reverse(int x, int bit_len) {
  int result = 0;
  for (int i = 0; i < bit_len; ++i) {
    result = (result << 1) | (x & 1);
    x = x >> 1;
  }
  return result;
}

void bit_reverse_index(complex<double>* arr, int len) {
  for (int i = 0; i < len; ++i){ 
    int j = bit_reverse(i, log2(len));
    if (i < j) {
      // swap i and j
      complex<double> temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }
}

void rawInPlaceFFT(complex<double>* data, size_t n, bool inverse, bool initCall = true) {
  // base case
  if (n == 1) {
    return;
  } 

  const double TWO_PI_OVER_LEN = 2*M_PI/n;
  const double multiple = inverse ? TWO_PI_OVER_LEN : -TWO_PI_OVER_LEN;

  if (initCall) {
    bit_reverse_index(data, n);
  }
  
  rawInPlaceFFT(data, n/2, inverse, false);
  rawInPlaceFFT(data+n/2, n/2, inverse, false);

  // minor optimization
  complex<double> shift = exp(
    complex<double>(0.0, multiple) 
  ); 
  complex<double> cur = complex<double>(1.0, 0.0);

  for (size_t i = 0; i < n/2; ++i) {
    complex<double> even_value = data[i];
    complex<double> odd_value = data[i+n/2];

    complex<double> odd_multiple = odd_value * cur;
    cur *= shift;

    data[i] = even_value + odd_multiple;
    data[i+n/2] = even_value - odd_multiple;
  }
  
}

void rawFFT(complex<double>* data, size_t n, bool inverse) {
  // base case
  if (n == 1) {
    return;
  } 

  const double TWO_PI_OVER_LEN = 2*M_PI/n;
  const double multiple = inverse ? TWO_PI_OVER_LEN : -TWO_PI_OVER_LEN;

  complex<double> odd[n/2];
  complex<double> even[n/2];
  for (size_t i = 0; i < n; i+=2) {
    odd[i/2] = data[i+1];
    even[i/2] = data[i];
  }

  rawFFT(odd, n/2, inverse);
  rawFFT(even, n/2, inverse);

  // minor optimization
  complex<double> shift = exp(
    complex<double>(0.0, multiple) 
  ); 
  complex<double> cur = complex<double>(1.0, 0.0);

  for (size_t i = 0; i < n/2; ++i) {
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

  rawInPlaceFFT(raw_data, next_radix_2, false);

  for (size_t i = 0; i < next_radix_2; ++i) {
    output[i] = raw_data[i];
  }

  return output;
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

  rawInPlaceFFT(raw_data, next_radix_2, true);

  for (size_t i = 0; i < len; ++i) {
    output[i] = (double)(raw_data[i].real())/len;
  }

  return output;
}

