#pragma once


namespace pa {
    template <std::size_t BitCount>
    using bit_uint_t = std::tuple_element_t<
        (BitCount > 8) + (BitCount > 16) + (BitCount > 32),
        std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>
    >;

    template <std::size_t MaskBitOffset, std::size_t MaskBitRange, typename T = bit_uint_t<MaskBitOffset + MaskBitRange>>
    static constexpr T bit_mask_r = T(~T(0)) >> (sizeof(T)*8 - MaskBitRange) << MaskBitOffset;
    template <std::size_t MaskBitOffset, std::size_t MaskBitRange, typename T = bit_uint_t<MaskBitOffset + MaskBitRange>>
    static constexpr T bit_mask_l = T(~T(0)) << (sizeof(T)*8 - MaskBitRange) >> MaskBitOffset;
}