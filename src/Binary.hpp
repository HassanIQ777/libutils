/* Part of https://github.com/HassanIQ777/libutils
Made on:     2026-Jan-18
Last update: 2026-Jan-19 */

#ifndef BINARY_HPP
#define BINARY_HPP

#include <stdint.h>

class Binary {
public:
  // sets bit at bit_index to 1
  static uint32_t set(uint32_t num, uint32_t bit_index) {
    if (bit_index >= 32) {
      return num;
    }
    return num | (1u << bit_index);
  }

  // sets bit at bit_index to 0
  static uint32_t unset(uint32_t num, uint32_t bit_index) {
    if (bit_index >= 32) {
      return num;
    }
    return num & ~(1u << bit_index);
  }

  // toggles bit at bit_index
  static uint32_t toggle(uint32_t num, uint32_t bit_index) {
    if (bit_index >= 32) {
      return num;
    }
    return num ^ (1u << bit_index);
  }

  // return bit at bit_index as a boolean
  static bool get(uint32_t num, uint32_t bit_index) {
    if (bit_index >= 32) {
      return false;
    }
    return (num & (1u << bit_index)) != 0;
  }

  // used to clear all bits
  static uint32_t clear() { return 0; }

  // changes all 0s to 1s, and 1s to 0s
  static uint32_t flip(uint32_t num) { return ~num; }
}; // end of class

#endif // BINARY_HPP
