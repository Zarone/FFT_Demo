#pragma once

#include <cstdint>

/**
  *
  *
  * @brief Applies FFT to data array for WAV File. Keeps header info intact as it was before, but copies it amoung several int16 arrays.
  *
  */
int16_t** transformWAVData(int16_t* data);

/**
*
*
* @brief Applies FFT to only amplitude data. Returns array of components.
*
*
*/
int16_t** transformAmplitudeData(int16_t* data);
