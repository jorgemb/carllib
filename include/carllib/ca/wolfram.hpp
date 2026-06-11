//
// Created by jorge on 11/06/2026.
//

#ifndef CARLLIB_WOLFRAM_CA_HPP
#define CARLLIB_WOLFRAM_CA_HPP

#include <cstdint>

#include "carllib/ca/base_1d.hpp"

namespace carllib::ca
{

/**
 * Represents the Wolfram rule number
 */
struct wolfram_number
{
  std::uint8_t value;
};

class wolfram : public base_1d<bool> {
public:
  /**
   * Default constructor, receives the rule number and the width.
   * @param rule
   * @param width
   */
  wolfram(wolfram_number rule, std::size_t width);

  /**
   * Calculate the next generation given the inital rule.
   * @return
   */
  auto calculate_next_generation() -> std::size_t override;
private:


  wolfram_number m_rule_number;
};

}  // namespace carllib::ca

#endif  // CARLLIB_WOLFRAM_CA_HPP
