#pragma once

namespace pa {
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

    /*struct RGBA8 {
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
    };*/

    // Note for ME: typename keyword after C++20 almost never required anymore for dependent typenames (Update!)
    // Another: replace static_cast<Type>(val) oftentimes with just Type(val) Syntax!

    template <std::size_t BitCountR, std::size_t BitCountG, std::size_t BitCountB, std::size_t BitCountA>
    struct ColorBase {
    public:
        static constexpr std::size_t sTotalBitCount = BitCountR + BitCountG + BitCountB + BitCountA;

        using PackedT = bit_uint<sTotalBitCount>;
        using RT = bit_uint<BitCountR>;
        using GT = bit_uint<BitCountG>;
        using BT = bit_uint<BitCountB>;
        using AT = bit_uint<BitCountA>;

    protected:
        constexpr ColorBase() {}
    };

    template <
        std::size_t BitCountR, std::size_t BitCountG, std::size_t BitCountB, std::size_t BitCountA,
        bool StoreChannelsElsePacked = (bit_uint<BitCountR + BitCountG + BitCountB + BitCountA>::sActualBitCount >= bit_uint<BitCountR>::sActualBitCount + bit_uint<BitCountG>::sActualBitCount + bit_uint<BitCountB>::sActualBitCount + bit_uint<BitCountA>::sActualBitCount)
    >
    struct Color : public ColorBase<BitCountR, BitCountG, BitCountB, BitCountA> {};

    template <std::size_t BitCountR, std::size_t BitCountG, std::size_t BitCountB, std::size_t BitCountA>
    struct Color<BitCountR, BitCountG, BitCountB, BitCountA, true> : public ColorBase<BitCountR, BitCountG, BitCountB, BitCountA> {
    private:
        using ColorBaseT = ColorBase<BitCountR, BitCountG, BitCountB, BitCountA>; // Mmmmehh..! Have to be using this base class access evrywhere cuz of this being a templated derived class accessing base's stuff ;(

        [[no_unique_address]] ColorBaseT::RT r;
        [[no_unique_address]] ColorBaseT::GT g;
        [[no_unique_address]] ColorBaseT::BT b;
        [[no_unique_address]] ColorBaseT::AT a;

    public:
        constexpr Color() : Color(ColorBaseT::RT::sMax, 0, ColorBaseT::BT::sMax) {} // Magenta, replace with predefined color later

        constexpr Color(const ColorBaseT::PackedT& pColor) : ColorBaseT(),
            r(ColorBaseT::RT(pColor)), // Uses automatic type conversion bit truncating for these
            g(ColorBaseT::GT(pColor >> BitCountR)),
            b(ColorBaseT::BT(pColor >> BitCountR + BitCountG)),
            a(ColorBaseT::AT(pColor >> BitCountR + BitCountG + BitCountB)) {}
        
        constexpr Color(const ColorBaseT::RT& pR, const ColorBaseT::GT& pG, const ColorBaseT::BT& pB, const ColorBaseT::AT& pA = ColorBaseT::AT::sMax) : ColorBaseT(),
            r(pR), g(pG), b(pB), a(pA) {}
        
        constexpr Color(float pPercentageR, float pPercentageG, float pPercentageB, float pPercentageA = 1) : Color(pPercentageR * ColorBaseT::RT::sMax, pPercentageG * ColorBaseT::GT::sMax, pPercentageB * ColorBaseT::BT::sMax, pPercentageA * ColorBaseT::AT::sMax) {}

        constexpr ColorBaseT::PackedT getColor() const {
            return std::bit_cast<ColorBaseT::PackedT>(*this); // I thInk this is completely legal and not unsafe buut. hm..
        }
        constexpr ColorBaseT::RT getR() const {return r;}
        constexpr ColorBaseT::GT getG() const {return g;}
        constexpr ColorBaseT::BT getB() const {return b;}
        constexpr ColorBaseT::AT getA() const {return a;}

        constexpr float getPercR() const {return (r / ColorBaseT::RT::sMax);}
        constexpr float getPercG() const {return (g / ColorBaseT::GT::sMax);}
        constexpr float getPercB() const {return (b / ColorBaseT::BT::sMax);}
        constexpr float getPercA() const {return (a / ColorBaseT::AT::sMax);}

        constexpr void setR(const ColorBaseT::RT& pR) {r = pR;}
        constexpr void setG(const ColorBaseT::GT& pG) {g = pG;}
        constexpr void setB(const ColorBaseT::BT& pB) {b = pB;}
        constexpr void setA(const ColorBaseT::AT& pA) {a = pA;}

        constexpr void setPercR(float pPercentageR) {r = pPercentageR * ColorBaseT::RT::sMax;}
        constexpr void setPercG(float pPercentageG) {g = pPercentageG * ColorBaseT::GT::sMax;}
        constexpr void setPercB(float pPercentageB) {b = pPercentageB * ColorBaseT::BT::sMax;}
        constexpr void setPercA(float pPercentageA) {a = pPercentageA * ColorBaseT::AT::sMax;}
    };

    template <std::size_t BitCountR, std::size_t BitCountG, std::size_t BitCountB, std::size_t BitCountA>
    struct Color<BitCountR, BitCountG, BitCountB, BitCountA, false> : public ColorBase<BitCountR, BitCountG, BitCountB, BitCountA> {
    private:
        using ColorBaseT = ColorBase<BitCountR, BitCountG, BitCountB, BitCountA>; // Same as above

        ColorBaseT::PackedT color;

    public:
        constexpr Color() : Color(ColorBaseT::RT::sMax, 0, ColorBaseT::BT::sMax) {}

        constexpr Color(const ColorBaseT::PackedT& pColor) : ColorBaseT(),
            color(pColor) {}

        constexpr Color(const ColorBaseT::RT& pR, const ColorBaseT::GT& pG, const ColorBaseT::BT& pB, const ColorBaseT::AT& pA = ColorBaseT::AT::sMax) : ColorBaseT(),
            color(((((((typename ColorBaseT::PackedT(pA) << BitCountB) | pB)) << BitCountG) | pG) << BitCountR) | pR) {}

        constexpr Color(float pPercentageR, float pPercentageG, float pPercentageB, float pPercentageA = 1) : Color(pPercentageR * ColorBaseT::RT::sMax, pPercentageG * ColorBaseT::GT::sMax, pPercentageB * ColorBaseT::BT::sMax, pPercentageA * ColorBaseT::AT::sMax) {}
        
        constexpr ColorBaseT::PackedT getColor() const {return color;}
        constexpr ColorBaseT::RT getR() const {return typename ColorBaseT::RT(color);}
        constexpr ColorBaseT::GT getG() const {return typename ColorBaseT::GT(color >> BitCountR);}
        constexpr ColorBaseT::BT getB() const {return typename ColorBaseT::BT(color >> BitCountR + BitCountG);}
        constexpr ColorBaseT::AT getA() const {return typename ColorBaseT::AT(color >> BitCountR + BitCountG + BitCountB);}

        constexpr float getPercR() const {return (getR() / ColorBaseT::RT::sMax);}
        constexpr float getPercG() const {return (getG() / ColorBaseT::GT::sMax);}
        constexpr float getPercB() const {return (getB() / ColorBaseT::BT::sMax);}
        constexpr float getPercA() const {return (getA() / ColorBaseT::AT::sMax);}

        constexpr void setR(const ColorBaseT::RT& pR) {
            color = color & ~ColorBaseT::PackedT::template bit_mask_r<0, BitCountR> | pR;
        }
        constexpr void setG(const ColorBaseT::GT& pG) {
            color = color & ~ColorBaseT::PackedT::template bit_mask_r<BitCountR, BitCountG> | (typename ColorBaseT::PackedT(pG) << BitCountR);
        }
        constexpr void setB(const ColorBaseT::BT& pB) {
            color = color & ~ColorBaseT::PackedT::template bit_mask_r<BitCountR + BitCountG, BitCountB> | (typename ColorBaseT::PackedT(pB) << (BitCountR + BitCountG));
        }
        constexpr void setA(const ColorBaseT::AT& pA) {
            color = color & ~ColorBaseT::PackedT::template bit_mask_r<BitCountR + BitCountG + BitCountB, BitCountA> | (typename ColorBaseT::PackedT(pA) << (BitCountR + BitCountG + BitCountB));
        }

        constexpr void setPercR(float pPercentageR) {setR(pPercentageR * ColorBaseT::RT::sMax);}
        constexpr void setPercG(float pPercentageG) {setG(pPercentageG * ColorBaseT::GT::sMax);}
        constexpr void setPercB(float pPercentageB) {setB(pPercentageB * ColorBaseT::BT::sMax);}
        constexpr void setPercA(float pPercentageA) {setA(pPercentageA * ColorBaseT::AT::sMax);}
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