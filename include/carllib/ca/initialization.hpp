//
// Created by jorge on 04/07/2026.
//

#ifndef CARLLIB_INITIALIZATION_HPP
#define CARLLIB_INITIALIZATION_HPP

#include <cstddef>
#include <variant>

namespace carllib::initialization
{

// Types for representing initial conditions

/**
 * Represents the CA standard start depending on the type
 */
struct standard
{
};

/**
 * Represents a randomized start with a seed.
 */
struct randomized
{
  std::size_t seed;
};

using initial_condition = std::variant<standard, randomized>;

}  // namespace carllib::initialization

#endif  // CARLLIB_INITIALIZATION_HPP
