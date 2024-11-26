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
  * @param fast Whether or not to use Fast Fourier Transform instead of DFT
  *
  */
std::vector<std::vector<int16_t>> transformWAVData(const std::vector<int16_t>& data, bool fast);


/**
*
*
* @brief Applies FFT to only amplitude data. Returns array of components. It's worth noting the output leaves the first 44 bytes free for header information to be inserted. However it does not touch that data, since that's the responsability of transformWAVData.
*
* @param data The actual array data
* @param length The number of elements in data
* @param numElements A pointer to store the number of elements in the returned array
* @param fast Whether or not to use Fast Fourier Transform instead of DFT
*
*/
std::vector<std::vector<int16_t>> transformAmplitudeData(const std::vector<int16_t>& data, int& numElements, bool fast);

/**
 *
 * @brief Performs the fourier transform
 *
 * @param outputReference a reference to put the actual frequency data into
 * @param inputData data on which to run the fourier transform
 * @param frequencyData A reference where we can put our frequency data
 * @param len the length of the array we're writing to
 * @param fast whether or not the perform FFT
 * @param windowed whether or not to window the function
 *
 */
void performFourierTransform(std::vector<int16_t>& outputReference, std::vector<std::complex<double>>& frequencyData, std::vector<int16_t> inputData, size_t len, bool fast, bool windowed);

/**
 *
 * @brief Performs the fourier transform
 *
 * @param outputReference a reference to put the actual frequency data into
 * @param inputData data on which to run the fourier transform
 * @param len the length of the array we're writing to
 * @param fast whether or not the perform FFT
 * @param windowed whether or not to window the function
 *
 */
void performInverseFourierTransform(std::vector<int16_t>& outputReference, std::vector<std::complex<double>>& inputData, size_t len, bool fast, bool windowed);
