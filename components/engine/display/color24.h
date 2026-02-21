#pragma once
#include <stdint.h>

struct Color24 {
    uint8_t m_R;
    uint8_t m_G;
    uint8_t m_B;

    constexpr bool operator==(const Color24& other) const = default;
    constexpr bool operator!=(const Color24& other) const = default;
};

constexpr size_t ColorSize = 3;

constexpr Color24 BLACK = {0, 0, 0};
constexpr Color24 WHITE = {255, 255, 255};
constexpr Color24 RED = {255, 0, 0};
constexpr Color24 GREEN = {0, 255, 0};
constexpr Color24 BLUE = {0, 0, 255};
