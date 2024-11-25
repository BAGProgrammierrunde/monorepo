#pragma once

namespace me {
    #define CONCAT(First, Second) First##Second

    #define DERIVED_TYPENAME(BaseT) typename DerivedT, typename = std::enable_if_t<std::is_base_of<BaseT, DerivedT>::value>

    // FOR VERSION >= C++20 USE THE "require" KEYWORD (and not this shitty SFINAE stuff)
    #define HAS_MEMBER(MemberName, CallCode) \
        template <typename T> \
        struct CONCAT(Has, MemberName) { \
        private: \
            template <typename TT, typename = void> \
            struct CONCAT(HasMember, MemberName) : std::false_type {}; \
            template <typename TT> \
            struct CONCAT(HasMember, MemberName)<TT, std::void_t<decltype(std::declval<TT>().CallCode)>> : std::true_type {}; \
            CONCAT(Has, MemberName)() {} \
        public: \
            static constexpr bool has() { \
                return CONCAT(HasMember, MemberName)<T>::value; \
            } \
        };

    template<int N, typename ...Ts> using getNthPackType = typename std::tuple_element_t<N, std::tuple<Ts...>>; // FOR VERSION >= C++26 USE THE "[N]" LOOKUP SYNTAX

    template<int N, typename ...Ts>
    auto getNthPackValue(Ts... pTypes) { // FOR VERSION >= C++26 USE THE "[N]" LOOKUP SYNTAX
        return std::get<N>(std::tuple<Ts...>(pTypes...));
    }

    template <typename Signature>
    struct FunctionPointerInfo {};
    template <typename ReturnT, typename ...Params>
    struct FunctionPointerInfo<ReturnT(*)(Params...)> {
    public:
        using ReturnType = ReturnT;
        using ParameterTypes = std::tuple<Params...>;
    private:
        FunctionPointerInfo() {}
    };
    template <typename ReturnT, typename ...Params>
    struct FunctionPointerInfo<ReturnT(*)(Params..., ...)> : FunctionPointerInfo<ReturnT(*)(Params...)> {}; // Variadic version

    template <typename Signature>
    struct MemberFunctionPointerInfo {};
    template <typename ClassT, typename ReturnT, typename ...Params>
    struct MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params...)> {
    public:
        using ClassType = ClassT;
        using ReturnType = ReturnT;
        using ParameterTypes = std::tuple<Params...>;
    private:
        MemberFunctionPointerInfo() {}
    };
    template <typename ClassT, typename ReturnT, typename ...Params>
    struct MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params...) const> : MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params...)> {}; // Const version
    template <typename ClassT, typename ReturnT, typename ...Params>
    struct MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params..., ...)> : MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params...)> {}; // Variadic version
    template <typename ClassT, typename ReturnT, typename ...Params>
    struct MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params..., ...) const> : MemberFunctionPointerInfo<ReturnT(ClassT::*)(Params...)> {}; // Const-variadic version
};
