#include "proto.h"

char * script_transform_multiXX (struct script_value value, int parameter, struct script_value * result, unsigned char width) {
  result -> data = NULL;
  result -> value = parameter;
  if (parameter < 0)
    return duplicate_string("negative item count");
  else if (value.type)
    return duplicate_string("type mismatch");
  else if (value.value % width)
    return duplicate_string("data length is not aligned");
  else if (value.value < (width * parameter))
    return duplicate_string("data is too short");
  const unsigned char * p = value.data;
  unsigned n;
  switch (width) {
    case 1: {
      signed char * data8 = malloc(parameter);
      memcpy(data8, value.data, parameter);
      result -> data = data8;
      result -> type = 5;
    } break;
    case 2: {
      short * data16 = malloc(sizeof(short) * parameter);
      for (n = 0; n < parameter; n ++) {
        data16[n] = convert_buffer_to_number(p, 2);
        p += 2;
      }
      result -> data = data16;
      result -> type = 6;
    } break;
    case 4: {
      int * data32 = malloc(sizeof(int) * parameter);
      for (n = 0; n < parameter; n ++) {
        data32[n] = convert_buffer_to_number(p, 4);
        p += 4;
      }
      result -> data = data32;
      result -> type = 7;
    } break;
    default:
      return duplicate_string("internal error");
  }
  return NULL;
}

char * script_transform_multi8 (struct script_value value, int parameter, struct script_value * result) {
  if (value.type && (value.type < 4)) {
    result -> type = 5;
    result -> value = parameter;
    result -> data = NULL;
    if (parameter < 0) return duplicate_string("negative item count");
    signed char * data8 = malloc(parameter);
    unsigned p;
    for (p = 0; p < parameter; p ++) data8[p] = value.value;
    result -> data = data8;
    return NULL;
  }
  return script_transform_multiXX(value, parameter, result, 1);
}

char * script_transform_multi16 (struct script_value value, int parameter, struct script_value * result) {
  if (value.type && (value.type < 4)) {
    result -> type = 6;
    result -> value = parameter;
    result -> data = NULL;
    if (parameter < 0) return duplicate_string("negative item count");
    short * data16 = malloc(parameter * sizeof(short));
    unsigned p;
    for (p = 0; p < parameter; p ++) data16[p] = value.value;
    result -> data = data16;
    return NULL;
  }
  return script_transform_multiXX(value, parameter, result, 2);
}

char * script_transform_multi32 (struct script_value value, int parameter, struct script_value * result) {
  if (value.type && (value.type < 4)) {
    result -> type = 7;
    result -> value = parameter;
    result -> data = NULL;
    if (parameter < 0) return duplicate_string("negative item count");
    int * data32 = malloc(parameter * sizeof(int));
    unsigned p;
    for (p = 0; p < parameter; p ++) data32[p] = value.value;
    result -> data = data32;
    return NULL;
  }
  return script_transform_multiXX(value, parameter, result, 4);
}

char * script_transform_multi (struct script_value value, int parameter, struct script_value * result) {
  result -> data = NULL;
  if (value.type)
    return duplicate_string("type mismatch");
  else if ((parameter != 1) && (parameter != 2) && (parameter != 4))
    return duplicate_string("parameter must be 1, 2 or 4");
  else if (value.value & (parameter - 1))
    return duplicate_string("data length is not aligned");
  return script_transform_multiXX(value, value.value / parameter, result, parameter);
}
