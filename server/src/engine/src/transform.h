#pragma once

#include <cstdint>

/**
  *
  *
  * @brief Applies FFT to data array for WAV File. Keeps header info intact as it was before, but copies it amoung several int16 arrays.
  *
  * @param data The actual array data
  * @param length The number of elements in data
  *
  */
int16_t** transformWAVData(int16_t* data, size_t length);


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
int16_t** transformAmplitudeData(int16_t* data, size_t length, int* numElements);
