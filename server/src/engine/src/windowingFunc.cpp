#include <cmath>

double windowingFunction(double index, double length) {
  // Hamming Window
  return 0.54 - 0.46 * std::cos(2 * M_PI * index / (length-1));
}

