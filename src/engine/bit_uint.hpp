#pragma once

//[[no_unique_address]]
//#pragma pack(1)
//#pragma pack(pop)

namespace pa {
    struct empty_t {
    public:
        constexpr empty_t() {}
        template <typename T>
        constexpr empty_t(const T& pEmpty) {}

        // Conversions

        template <typename T>
        constexpr operator T() const {return T();}

        // Arithmetic operations

        constexpr empty_t operator+() const {return empty_t();}
        constexpr empty_t operator-() const {return empty_t();}
        template <typename T>
        constexpr empty_t operator+(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator-(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator*(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator/(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator%(const T& pOther) const {return empty_t();}

        template <typename T>
        constexpr empty_t& operator+=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator-=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator*=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator/=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator%=(const T& pOther) {return *this;}

        constexpr empty_t& operator++() {return *this;}
        constexpr empty_t& operator--() {return *this;}

        // Bit operations

        constexpr empty_t operator~() const {return empty_t();}
        template <typename T>
        constexpr empty_t operator&(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator|(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator^(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator<<(const T& pOther) const {return empty_t();}
        template <typename T>
        constexpr empty_t operator>>(const T& pOther) const {return empty_t();}
        
        template <typename T>
        constexpr empty_t& operator&=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator|=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator^=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator<<=(const T& pOther) {return *this;}
        template <typename T>
        constexpr empty_t& operator>>=(const T& pOther) {return *this;}

        // Comparisons

        template <typename T>
        constexpr bool operator==(const T& pOther) const {return false;}
        template <typename T>
        constexpr bool operator!=(const T& pOther) const {return false;}

        template <typename T>
        constexpr bool operator<(const T& pOther) const {return false;}
        template <typename T>
        constexpr bool operator>(const T& pOther) const {return false;}

        template <typename T>
        constexpr bool operator<=(const T& pOther) const {return false;}
        template <typename T>
        constexpr bool operator>=(const T& pOther) const {return false;}
    };

    template <std::size_t BitCount, typename = std::enable_if_t<BitCount <= 64>> // Until now only supports all normal CPUs handled types (so up to uint64 bit as of now)
    struct bit_uint {
    public:
        using type = std::tuple_element_t<
            (BitCount > 0) + (BitCount > 8) + (BitCount > 16) + (BitCount > 32),
            std::tuple<empty_t, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>
        >;
        static constexpr std::size_t sActualBitCount = (BitCount > 0 ? sizeof(type) : 0);

        template <std::size_t MaskOffset, std::size_t MaskRange>
        static constexpr type bit_mask_r = type(~type(0)) >> (sActualBitCount*8 - MaskRange) << MaskOffset;
        template <std::size_t MaskOffset, std::size_t MaskRange>
        static constexpr type bit_mask_l = type(~type(0)) << (sActualBitCount*8 - MaskRange) >> MaskOffset;

        static constexpr type sMax = bit_mask_r<0, BitCount>;

    private:
        [[no_unique_address]] type value;

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

        //explicit constexpr operator type() const {return value;} // Has to be explicit IF used, so that all my own defined operator overloads don't suddenly collide with the default provided ones for type (gives ambiguity errors)
        //template <std::size_t OtherBitCount>
        //constexpr operator bit_uint<OtherBitCount>() const {return bit_uint<OtherBitCount>(value);} // ? enough?
        template <typename T>
        constexpr operator T() const {return T(value);} // ? Too much? Or good?

        // Arithmetic operations

        constexpr bit_uint<BitCount> operator+() const {return bit_uint<BitCount>(+value);}
        constexpr bit_uint<BitCount> operator-() const {return bit_uint<BitCount>(-value);}
        constexpr bit_uint<BitCount> operator+(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value + pOther.value);}
        constexpr bit_uint<BitCount> operator-(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value - pOther.value);}
        constexpr bit_uint<BitCount> operator*(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value * pOther.value);}
        constexpr bit_uint<BitCount> operator/(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value / pOther.value);}
        constexpr bit_uint<BitCount> operator%(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value % pOther.value);}

        constexpr bit_uint<BitCount>& operator++() {assign(value + 1); return *this;}
        constexpr bit_uint<BitCount>& operator--() {assign(value - 1); return *this;}
        constexpr bit_uint<BitCount>& operator+=(const bit_uint<BitCount>& pOther) {assign(value + pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator-=(const bit_uint<BitCount>& pOther) {assign(value - pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator*=(const bit_uint<BitCount>& pOther) {assign(value * pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator/=(const bit_uint<BitCount>& pOther) {assign(value / pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator%=(const bit_uint<BitCount>& pOther) {assign(value % pOther.value); return *this;}

        // Bit operations

        constexpr bit_uint<BitCount> operator~() const {return bit_uint<BitCount>(~value);}
        constexpr bit_uint<BitCount> operator&(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value & pOther.value);}
        constexpr bit_uint<BitCount> operator|(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value | pOther.value);}
        constexpr bit_uint<BitCount> operator^(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value ^ pOther.value);}
        constexpr bit_uint<BitCount> operator<<(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value << pOther.value);}
        constexpr bit_uint<BitCount> operator>>(const bit_uint<BitCount>& pOther) const {return bit_uint<BitCount>(value >> pOther.value);}
        
        constexpr bit_uint<BitCount>& operator&=(const bit_uint<BitCount>& pOther) {assign(value & pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator|=(const bit_uint<BitCount>& pOther) {assign(value | pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator^=(const bit_uint<BitCount>& pOther) {assign(value ^ pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator<<=(const bit_uint<BitCount>& pOther) {assign(value << pOther.value); return *this;}
        constexpr bit_uint<BitCount>& operator>>=(const bit_uint<BitCount>& pOther) {assign(value >> pOther.value); return *this;}

        // Comparisons

        template <typename T>
        constexpr bool operator==(const T& pOther) const {return (value == pOther);}
        template <typename T>
        constexpr bool operator!=(const T& pOther) const {return (value != pOther);}
        template <typename T>
        constexpr bool operator<(const T& pOther) const {return (value < pOther);}
        template <typename T>
        constexpr bool operator>(const T& pOther) const {return (value > pOther);}
        template <typename T>
        constexpr bool operator<=(const T& pOther) const {return (value <= pOther);}
        template <typename T>
        constexpr bool operator>=(const T& pOther) const {return (value >= pOther);}

        // Are these needed? Or the above? Good to have them, right? Or rather useless?
        constexpr bool operator==(const bit_uint<BitCount>& pOther) const {return (value == pOther.value);}
        constexpr bool operator!=(const bit_uint<BitCount>& pOther) const {return (value != pOther.value);}
        constexpr bool operator<(const bit_uint<BitCount>& pOther) const {return (value < pOther.value);}
        constexpr bool operator>(const bit_uint<BitCount>& pOther) const {return (value > pOther.value);}
        constexpr bool operator<=(const bit_uint<BitCount>& pOther) const {return (value <= pOther.value);}
        constexpr bool operator>=(const bit_uint<BitCount>& pOther) const {return (value >= pOther.value);}
    };
}