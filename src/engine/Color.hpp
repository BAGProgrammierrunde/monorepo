#pragma once

struct Color {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color(uint8_t pR = 255, uint8_t pG = 0, uint8_t pB = 255, uint8_t pA = 255) {
        r = pR;
        g = pG;
        b = pB;
        a = pA;
    }

    Color(uint16_t pColor565) {
        setRGB565(pColor565);
    }

    uint16_t getRGB565() {
        return (static_cast<uint16_t>(r & 31) | (static_cast<uint16_t>(g & 63) << 5) | (static_cast<uint16_t>(b & 31) << 11));
    }

    void setRGB565(uint16_t pColor565) {
        b = pColor565 & 31;
        pColor565 >>= 5;
        g = pColor565 & 63;
        pColor565 >>= 6;
        r = pColor565 & 31;
    }
};