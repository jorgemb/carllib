//
// Created by jorge on 04/07/2026.
//

#ifndef CARLLIB_VARIANT_HPP
#define CARLLIB_VARIANT_HPP

namespace carllib::util
{
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;
} // namespace carllib::util

#endif  // CARLLIB_VARIANT_HPP
