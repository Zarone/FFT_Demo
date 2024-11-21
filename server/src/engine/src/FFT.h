#pragma once

#include <vector>
#include <complex>

/**
*
*
* @brief The actual Inverse FFT algorithm (using zero padding)
*/
std::vector<int16_t> IFFT_padding(const std::vector<std::complex<double>>& data);

/**
*
*
* @brief The actual FFT algorithm (using zero padding)
*/
std::vector<std::complex<double>> FFT_padding(const std::vector<int16_t>& data);
