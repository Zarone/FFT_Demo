#include <cstdlib>

#include "transform.h"

int16_t** transformWAVData(int16_t* data) {
  int16_t** value = (int16_t**) calloc(1, sizeof(int16_t*));
  value[0] = data;
  return value;
}
