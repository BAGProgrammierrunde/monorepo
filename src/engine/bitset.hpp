#pragma once

namespace page {
    template <std::size_t BitCount>
    struct bitset {
    private:
        static constexpr std::size_t byteCount = std::ceil(BitCount / 8.f);
        using bit_type = std::tuple_element_t<
            (byteCount > 1) + (byteCount > 2) + (byteCount > 4) + (byteCount > 8),
            std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, std::uint8_t[byteCount]>
        >;
        static constexpr bool isArray = (BitCount > 64);

    public:
        bit_type data;

        bitset() {}

        constexpr std::size_t getBitCount() {
            return BitCount;
        }
        constexpr bit_type getMaxValue() {
            //~bit_type(0);
            return std::memset(&bit_type{}, 0xff, byteCount);
        }

        //getBit
        //getBit
        //getBitRange
        //setBitRange
        //setPercentage

        // ... normal number interactions (+-*/ and bit operations)
    };
}