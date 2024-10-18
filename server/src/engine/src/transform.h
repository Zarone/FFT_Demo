#pragma once

#include <cstdint>
#include <vector>
#include <complex>

// 44 header bytes  / 2
// We divide by two because we use int16
const size_t HEADER_OFFSET = 22;

/**
  *
  *
  * @brief Applies FFT to data array for WAV File. Keeps header info intact as it was before, but copies it amoung several int16 arrays.
  *
  * @param data The actual array data
  * @param length The number of elements in data
  *
  */
std::vector<std::vector<int16_t>> transformWAVData(const std::vector<int16_t>& data);


/**
*
*
* @brief Applies FFT to only amplitude data. Returns array of components. It's worth noting the output leaves the first 44 bytes free for header information to be inserted. However it does not touch that data, since that's the responsability of transformWAVData.
*
* @param data The actual array data
* @param length The number of elements in data
* @param numElements A pointer to store the number of elements in the returned array
*
*/
std::vector<std::vector<int16_t>> transformAmplitudeData(const std::vector<int16_t>& data, int& numElements);

/**
*
*
* @brief Just a simple DFT
*/
std::vector<std::complex<double>> DFT(const std::vector<int16_t>& data);
