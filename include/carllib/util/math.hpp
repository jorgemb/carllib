//
// Created by jorge on 09/06/2026.
//

#ifndef CARLLIB_MATH_HPP
#define CARLLIB_MATH_HPP
#include <concepts>

namespace carllib::util
{

template<class T>
concept signed_integral = std::integral<T> && std::is_signed_v<T>;

// Source - https://stackoverflow.com/a/707426
// Posted by Lara Bailey, modified by community. See post 'Timeline' for
// change history Retrieved 2026-06-09, License - CC BY-SA 2.5
template<class T>
  requires signed_integral<T>
auto wrap(T value, T const lower, T const upper) -> T {
  T range_size = upper - lower + 1;

  if (value < lower) {
    value += range_size * (((lower - value) / range_size) + 1);
  }

  return lower + ((value - lower) % range_size);
}

}  // namespace carllib::util

#endif  // CARLLIB_MATH_HPP
