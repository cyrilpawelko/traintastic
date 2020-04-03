/**
 * server/src/core/to.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019 Reinder Feenstra
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TRAINTASTIC_SERVER_CORE_TO_HPP
#define TRAINTASTIC_SERVER_CORE_TO_HPP

#include <type_traits>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <nlohmann/json.hpp>

class not_writable_error : public std::runtime_error
{
  public:
    not_writable_error() :
      std::runtime_error("not writable error")
    {
    }
};

class invalid_value_error : public std::runtime_error
{
  public:
    invalid_value_error() :
      std::runtime_error("invalid value error")
    {
    }
};

class conversion_error : public std::runtime_error
{
  public:
    conversion_error() :
      std::runtime_error("conversion error")
    {
    }
};

class out_of_range_error : public std::runtime_error
{
  public:
    out_of_range_error() :
      std::runtime_error("out of range error")
    {
    }
};

template<typename To, typename From>
To to(const From& value)
{
  if constexpr(std::is_same_v<To, From>)
    return value;
  else if constexpr(std::is_integral_v<To> && std::is_enum_v<From>)
    return static_cast<To>(value);
  else if constexpr(std::is_enum_v<To> && std::is_integral_v<From>)
  {
    // TODO: test if enum value is valid !!
    return static_cast<To>(value);
  }
  else if constexpr(!std::is_same_v<To, bool> && std::is_integral_v<To> && !std::is_same_v<From, bool> && std::is_integral_v<From>)
  {
    if constexpr(std::numeric_limits<To>::min() <= std::numeric_limits<From>::min() && std::numeric_limits<To>::max() >= std::numeric_limits<From>::max())
      return value;
    else if(value >= std::numeric_limits<To>::min() && value <= std::numeric_limits<To>::max())
      return value;
    else
      throw out_of_range_error();
  }
  else if constexpr(std::is_floating_point_v<To> && (std::is_integral_v<From> || std::is_floating_point_v<From>))
    return value;
  else if constexpr(std::is_integral_v<To> && std::is_floating_point_v<From>)
  {
    if(value >= std::numeric_limits<To>::min() && value <= std::numeric_limits<To>::max())
      return static_cast<To>(std::round(value));
    else
      throw out_of_range_error();
  }
  else if constexpr(std::is_same_v<To, std::string> && std::is_integral_v<From> && !std::is_same_v<From, bool>)
  {
    return std::to_string(value);
  }
  else if constexpr(std::is_same_v<From, nlohmann::json>)
  {
  //  if constexpr(std::is_same_v<To, bool>)
      return value;//.get<To>();
  }


  throw conversion_error();
}

#endif
