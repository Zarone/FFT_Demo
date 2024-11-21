#pragma once

#include <complex>
#include <vector>

/**
 *
 *
 * @brief A function used to preprocess the input data to better find frequencies
 *
 * @param index
 * @param length
 *
 */
double windowingFunction(double index, double length);

/**
*
* @param data The input data
* @param windowed Whether or not to apply a windowing function. Increases frequency data clarity but breaks if windowing function is not reversed at the end.
*
* @brief Just a simple DFT
*/
std::vector<std::complex<double>> DFT(const std::vector<int16_t>& data, bool windowed);

/**
*
*
*
*
* @brief Just a simple inverse DFT
*/
std::vector<int16_t> InverseDFT(const std::vector<std::complex<double>>& data, bool windowed);
