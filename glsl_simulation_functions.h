#ifndef GLSL_SIMULATION_FUNCTIONS_H
#define GLSL_SIMULATION_FUNCTIONS_H

#include <cstdint>
#include <cmath>

// A simple implementation of an ivec2 and ivec 3 to allow the C++ code
// to resemble GLSL code more.
class ivec2 {
public:
  ivec2() {
    x = 0;
    y = 0;
  }
  ivec2(int32_t a, int32_t b) {
    x = a;
    y = b;
  }
  void setUint64(uint64_t a) {
    x = a & 0xFFFFFFFF;
    y = a >> 32;
  }
  void setInt64(int64_t a) {
    x = a & 0xFFFFFFFF;
    y = a >> 32;
  }
  uint64_t getUint64() const {
    uint64_t temp = (static_cast<uint64_t>(static_cast<uint32_t>(y))) << 32;
    uint64_t result = temp | static_cast<uint64_t>(static_cast<uint32_t>(x));
    return result;
  }
  int64_t getInt64() const {
    return static_cast<int64_t>(getUint64());
  }
  long double getLongDouble() const {
    long double shift32 = static_cast<long double>(0x100000000);
    long double result = y;
    result *= shift32;
    long double x2 = x;
    result += x2;
    return result;
  }
  void flipBit(uint32_t index) {
    if (index >= 32) {
      index -= 32;
      y ^= 1 << index;
      return;
    }
    x ^= 1 << index;
    return;
  }

  union {
    struct {
      int32_t x;
      int32_t y;
    };
    uint64_t val;
  };
};

class ivec3 {
public:
  ivec3() {
    x = 0;
    y = 0;
    z = 0;
  }
  ivec3(int32_t a, int32_t b, int32_t c) {
    x = a;
    y = b;
    z = c;
  }

  uint64_t getLowUint64() const {
    uint64_t temp = static_cast<uint64_t>(static_cast<uint32_t>(y)) << 32;
    temp |= static_cast<uint32_t>(x);
    return temp;
  }
  void setLowUint64(uint64_t val) {
    x = val & 0xFFFFFFFF;
    y = val >> 32;
  }
  uint32_t getUpper32() const { return z; }
  long double getLongDouble() const {
    long double shift32 = static_cast<long double>(0x100000000);
    long double result = z;
    result *= shift32;
    long double y2 = y;
    result += y2;
    result *= shift32;
    long double x2 = x;
    result += x2;
    return result;
  };

  void flipBit(uint32_t index) {
    if (index >= 64) {
      index -= 64;
      z ^= 1 << index;
      return;
    }
    if (index >= 32) {
      index -= 32;
      y ^= 1 << index;
      return;
    }
    x ^= 1 << index;
    return;
  }

  union {
    struct {
      int32_t z;
      int32_t y;
      int32_t x;
    };
    struct {
      uint32_t upper_32;
      uint64_t lower_64;
    };
  };
};

// Utility functions to emulate 64-bit and 96-bit arithmetic in GLSL. Since
// GLSL is so horrible to debug, make sure the code for these functions stays
// both valid C++ and valid GLSL, so the unit tests for this class can help
// debug the shader issues.
ivec2 Add64(ivec2 a, ivec2 b);
ivec2 Sub64(ivec2 a, ivec2 b);
float Multiply64BitWithFloat(ivec2 a, float b);
float Multiply64BitWithFloat(ivec2 a, float b);
ivec3 Add96(ivec3 a, ivec3 b);
ivec3 Sub96(ivec3 a, ivec3 b);
float Multiply96BitWithFloat(ivec3 a, float b);
int TopNibble(int value);
ivec3 LongDoubleTo96Bits(long double value);
ivec3 Load64BitLeftShiftedBy4Into96Bit(int low, int high);
ivec2 Load32BitLeftShiftedBy4Into64Bit(int low);

// Functions that are not necessarily needed in GLSL.
ivec2 LongDoubleTo64Bits(long double value);
ivec3 LongDoubleTo96Bits(long double value);

#endif // GLSL_SIMULATION_FUNCTIONS_H