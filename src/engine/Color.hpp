#pragma once

namespace page {
    /*template <typename ...ColorTs>
    struct Color {
    private:
        std::tuple<ColorTs...> colorFormats;

    public:
        Color() {// Use lambdas + fold expressions to loop through color types here aaand set/etc.. them
            // Weeeell actually think about di agaain.. maybe also first finish displays interaction before this one here
        }
    };

    // Add this type and test in driver/display
    // Still implement swap 16 somewhere (or first test with display)
    // --> Other RGB representations? HSV? etc...
    struct RGB565 {
    public:
        std::uint16_t color; // Potentially instead of swapping bytes heere allocate display buffer of uint8_t but double length and do it manually theere hmmm what'd be faster?? or both actually equal in terms of cycles & operations? mmhmm

        constexpr RGB565() : RGB565(31, 0, 31) {}
        constexpr RGB565(std::uint16_t pColor)
            : color(pColor) {}
        constexpr RGB565(std::uint8_t pR, std::uint8_t pG, std::uint8_t pB)
            : color((((static_cast<std::uint16_t>(pR) << 6) | (pG & 0b111111)) << 5) | (pB & 0b11111)) {}

        constexpr std::uint8_t getR() {return (color >> 11);}
        constexpr std::uint8_t getG() {return (color >> 5);}
        constexpr std::uint8_t getB() {return color;}

        constexpr void setR(std::uint8_t pR) {
            color = (color & 0b11111111111) | (static_cast<std::uint16_t>(pR) << 11);
        }
        constexpr void setG(std::uint8_t pG) {
            color = (color & 0b1111100000011111) | (static_cast<std::uint16_t>(pG & 0b111111) << 5);
        }
        constexpr void setB(std::uint8_t pB) {
            color = (color & 0b1111111111100000) | (pB & 0b11111);
        }
    };*/

    struct RGBA8 {
    public:
        using CombColorT = std::uint32_t;
        using ColorChannelT = std::uint8_t;

        union {
            CombColorT color;
            struct {
                ColorChannelT r;
                ColorChannelT g;
                ColorChannelT b;
                ColorChannelT a;
            };
        };

        constexpr RGBA8(CombColorT pColor)
            : color(pColor) {}
        
        constexpr RGBA8(ColorChannelT pR, ColorChannelT pG, ColorChannelT pB, ColorChannelT pA)
            : r(pR), g(pG), b(pB), a(pA) {}
    };

    template <std::size_t BitCountR, std::size_t BitCountG, std::size_t BitCountB, std::size_t BitCountA>
    struct Color {
    public:
        static constexpr std:size_t TotalBitCount = BitCountR + BitCountG + BitCountB + BitCountA;
        bitset<TotalBitCount> color;

        constexpr Color(const bitset<TotalBitCount>& pColor) : color(pColor) {}

        constexpr bitset<BitCountR> getR() {
            return color.getBitRange(0, BitCountR - 1);
        }
        constexpr bitset<BitCountG> getG() {
            return color.getBitRange(BitCountR, BitCountR + BitCountG - 1);
        }
        constexpr bitset<BitCountB> getB() {
            return color.getBitRange(BitCountR + BitCountG, BitCountR + BitCountG + BitCountB - 1);
        }
        constexpr bitset<BitCountA> getA() {
            return color.getBitRange(BitCountR + BitCountG + BitCountB, TotalBitCount - 1);
        }

        constexpr void setR(const bitset<BitCountR>& pR) {
            color.setBitRange(0, BitCountR - 1, pR);
        }
        constexpr void getG(const bitset<BitCountG>& pG) {
            color.setBitRange(BitCountR, BitCountR + BitCountG - 1, pG);
        }
        constexpr void setB(const bitset<BitCountB>& pB) {
            color.setBitRange(BitCountR + BitCountG, BitCountR + BitCountG + BitCountB - 1, pB);
        }
        constexpr void setA(const bitset<BitCountA>& pA) {
            color.setBitRange(BitCountR + BitCountG + BitCountB, TotalBitCount - 1, pA);
        }
    };


    /*static const Color White;
        static const Color Black;
        
        static const Color Red;
        
        static const Color Orange;
        static const Color Yellow;
        static const Color Lime;
        
        static const Color Green;
        
        static const Color Mint;
        static const Color Cyan;
        static const Color LightBlue;
        
        static const Color Blue;

        static const Color Raspberry; // Red-isch Magenta
        static const Color Magenta;
        static const Color Purple;*/

    // External initialization because of otherwise incomplete type, but still staying under same namespace :)
    /*constexpr Color Color::White = Color(255, 255, 255);
    constexpr Color Color::Black = Color(0, 0, 0);

    constexpr Color Color::Red = Color(255, 0, 0);

    constexpr Color Color::Orange = Color(255, 150, 0);
    constexpr Color Color::Yellow = Color(255, 255, 0);
    constexpr Color Color::Lime = Color(150, 255, 0);

    constexpr Color Color::Green = Color(0, 255, 0);

    constexpr Color Color::Mint = Color(0, 255, 150);
    constexpr Color Color::Cyan = Color(0, 255, 255);
    constexpr Color Color::LightBlue = Color(0, 150, 255);

    constexpr Color Color::Blue = Color(0, 0, 255);

    constexpr Color Color::Raspberry = Color(255, 0, 150); // Red-isch Magenta
    constexpr Color Color::Magenta = Color(255, 0, 255);
    constexpr Color Color::Purple = Color(150, 0, 255);*/
}

/*
constexpr std::uint32_t getRGBA8() const {
    return ((((((static_cast<std::uint32_t>(r) << 8) | g) << 8) | b) << 8) | a);
}
constexpr std::uint16_t getRGB565() const {
    return ((((static_cast<std::uint16_t>(r & 31) << 6) | (g & 63)) << 5) | (b & 31));
}

constexpr void setRGBA8(std::uint32_t pColor) {
    a = pColor & 255;
    pColor >>= 8;
    b = pColor & 255;
    pColor >>= 8;
    g = pColor & 255;
    pColor >>= 8;
    r = pColor & 255;
}
constexpr void setRGB565(std::uint16_t pColor565) {
    b = pColor565 & 31;
    pColor565 >>= 5;
    g = pColor565 & 63;
    pColor565 >>= 6;
    r = pColor565 & 31;
}
*/