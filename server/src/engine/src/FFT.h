#pragma once

#include <vector>
#include <complex>

/**
*
*
* @brief The actual Inverse FFT algorithm (using zero padding)
* @param windowed Whether or not to apply a windowing function. Increases frequency data clarity but breaks if windowing function is not reversed at the end.
*/
std::vector<int16_t> IFFTPadding(const std::vector<std::complex<double>>& data, bool windowed);
std::vector<int16_t> BluesteinIFFT(const std::vector<std::complex<double>>& data, bool windowed);

/**
*
*
* @brief The actual FFT algorithm (using zero padding)
* @param windowed Whether or not to apply a windowing function. Increases frequency data clarity but breaks if windowing function is not reversed at the end.
*/
std::vector<std::complex<double>> FFTPadding(const std::vector<int16_t>& data, bool windowed);
std::vector<std::complex<double>> BluesteinFFT(const std::vector<int16_t>& data, bool windowed);
