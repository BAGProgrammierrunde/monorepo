#pragma once

namespace pa {
    template <std::size_t BitCount, typename = std::enable_if_t<BitCount <= 64>> // Until now only supports all normal CPUs handled types (so up to uint64 bit as of now)
    using bit_uint_t = std::tuple_element_t<
        (BitCount > 8) + (BitCount > 16) + (BitCount > 32),
        std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>
    >;

    template <std::size_t BitCount, std::size_t MaskOffset, std::size_t MaskRange>
    static constexpr bit_uint_t<BitCount> bit_mask_r = static_cast<bit_uint_t<BitCount>>(~bit_uint_t<BitCount>(0)) >> (sizeof(bit_uint_t<BitCount>)*8 - MaskRange) << MaskOffset;
    template <std::size_t BitCount, std::size_t MaskOffset, std::size_t MaskRange>
    static constexpr bit_uint_t<BitCount> bit_mask_l = static_cast<bit_uint_t<BitCount>>(~bit_uint_t<BitCount>(0)) << (sizeof(bit_uint_t<BitCount>)*8 - MaskRange) >> MaskOffset;
    
    template <std::size_t BitCount>
    struct bit_uint {
    public:
        using type = bit_uint_t<BitCount>;

        static constexpr type sMax = bit_mask_r<BitCount, 0, BitCount>;

    private:
        type value;

        template <typename T>
        constexpr void assign(const T& pOther) {
            value = pOther;
            value &= sMax; // Purge out-of-bit-scope bits
        }

    public:
        constexpr bit_uint(const type& pValue = 0) { // Takes care of assignment too
            assign(pValue);
        }

        // Conversions

        constexpr operator type() const {return value;}

        // Arithmetic operations

        constexpr bit_uint<BitCount> operator+() const {return bit_uint<BitCount>(+value);}
        constexpr bit_uint<BitCount> operator-() const {return bit_uint<BitCount>(-value);}
        template <typename T>
        constexpr bit_uint<BitCount> operator+(const T& pOther) const {return bit_uint<BitCount>(value + pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator-(const T& pOther) const {return bit_uint<BitCount>(value - pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator*(const T& pOther) const {return bit_uint<BitCount>(value * pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator/(const T& pOther) const {return bit_uint<BitCount>(value / pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator%(const T& pOther) const {return bit_uint<BitCount>(value % pOther);}

        template <typename T>
        constexpr bit_uint<BitCount>& operator+=(const T& pOther) {assign(value + pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator-=(const T& pOther) {assign(value - pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator*=(const T& pOther) {assign(value * pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator/=(const T& pOther) {assign(value / pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator%=(const T& pOther) {assign(value % pOther); return *this;}

        template <typename T>
        constexpr bit_uint<BitCount>& operator++(const T& pOther) {assign(value + 1); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator--(const T& pOther) {assign(value - 1); return *this;}

        // Bit operations

        constexpr bit_uint<BitCount> operator~() const {return bit_uint<BitCount>(~value);}
        template <typename T>
        constexpr bit_uint<BitCount> operator&(const T& pOther) const {return bit_uint<BitCount>(value & pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator|(const T& pOther) const {return bit_uint<BitCount>(value | pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator^(const T& pOther) const {return bit_uint<BitCount>(value ^ pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator<<(const T& pOther) const {return bit_uint<BitCount>(value << pOther);}
        template <typename T>
        constexpr bit_uint<BitCount> operator>>(const T& pOther) const {return bit_uint<BitCount>(value >> pOther);}
        
        template <typename T>
        constexpr bit_uint<BitCount>& operator&=(const T& pOther) {assign(value & pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator|=(const T& pOther) {assign(value | pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator^=(const T& pOther) {assign(value ^ pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator<<=(const T& pOther) {assign(value << pOther); return *this;}
        template <typename T>
        constexpr bit_uint<BitCount>& operator>>=(const T& pOther) {assign(value >> pOther); return *this;}

        // Comparisons

        template <typename T>
        constexpr bool operator<(const T& pOther) const {return (value < pOther);}
        template <typename T>
        constexpr bool operator>(const T& pOther) const {return (value > pOther);}

        template <typename T>
        constexpr bool operator<=(const T& pOther) const {return (value <= pOther);}
        template <typename T>
        constexpr bool operator>=(const T& pOther) const {return (value >= pOther);}
    };
}