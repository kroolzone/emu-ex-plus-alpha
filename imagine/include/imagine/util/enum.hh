#pragma once

/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#include "wise_enum/wise_enum.h"

namespace IG
{

template <class WiseEnum>
constexpr auto lastEnum = wise_enum::range<WiseEnum>.back().value;

}

#define IG_DEFINE_ENUM_BIT_FLAG_FUNCTIONS(T) \
constexpr T operator~(T a) { return static_cast<T>(~std::to_underlying(a)); } \
constexpr T operator|(T a, T b) { return static_cast<T>(std::to_underlying(a) | std::to_underlying(b)); } \
constexpr T operator&(T a, T b) { return static_cast<T>(std::to_underlying(a) & std::to_underlying(b)); } \
constexpr T operator^(T a, T b) { return static_cast<T>(std::to_underlying(a) ^ std::to_underlying(b)); } \
constexpr T& operator|= (T& a, T b) { a = a | b; return a; } \
constexpr T& operator&= (T& a, T b) { a = a & b; return a; } \
constexpr T& operator^= (T& a, T b) { a = a ^ b; return a; }