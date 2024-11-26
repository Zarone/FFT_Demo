#include <vector>
#include <complex>

#include "windowingFunc.h"
#include "DFT.h"

using std::vector;
using std::complex;

vector<complex<double>> DFT(const vector<int16_t>& data, bool windowed) {
  size_t len = data.size();
  const double NEGATIVE_TWO_PI_OVER_LEN = -2*M_PI/len;

  vector<complex<double>> output(len);

  for (size_t i = 0; i < len; i++) {
    complex<double> sum = 0;
    for (size_t j = 0; j < len; j++) {
      double dataValue = static_cast<double>(data[j]);
      
      // improves frequency resolution sometimes
      if (windowed) dataValue *= windowingFunction(j, len);

      sum += static_cast<double>(dataValue) * exp(
        complex<double>(0.0, i*j*NEGATIVE_TWO_PI_OVER_LEN) 
      );
    }
    output[i] = sum;
  }

  return output;
}

vector<int16_t> inverseDFT(const vector<complex<double>>& data, bool windowed) {
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
    output[i] = sum/((double)len);
    if (windowed) output[i] /= windowingFunction(i, len);
  }

  return output;
}

