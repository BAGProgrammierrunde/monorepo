#pragma once

#include <stddef.h>
#include <stdint.h>

#define SWAP16(x) ((uint16_t)((((uint16_t)(x)) << 8) | (((uint16_t)(x)) >> 8)))

using Color16 = uint16_t;

constexpr size_t ColorSize = 2;

constexpr Color16 BLACK = SWAP16(0x0000u);
constexpr Color16 WHITE = SWAP16(0xFFFFu);
constexpr Color16 RED = SWAP16(0xF800u);
constexpr Color16 GREEN = SWAP16(0x07E0);
constexpr Color16 BLUE = SWAP16(0x001F);
