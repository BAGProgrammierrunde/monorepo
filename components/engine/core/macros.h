#pragma once

#include <tuple>

#define CONCAT(First, Second) First##Second

#define REQUIRE_DERIVEDT(BaseT, DerivedT) typename = std::enable_if_t<std::is_base_of_v<BaseT, DerivedT>>

// FOR VERSION >= C++20 USE THE "require" KEYWORD (and not this shitty SFINAE stuff)
#define HAS_MEMBER(MemberName, CallCode)                                                                                                   \
    template <typename T> struct CONCAT(Has, MemberName) {                                                                                 \
      private:                                                                                                                             \
        template <typename TT, typename = void> struct CONCAT(HasMember, MemberName) : std::false_type {};                                 \
        template <typename TT>                                                                                                             \
        struct CONCAT(HasMember, MemberName)<TT, std::void_t<decltype(std::declval<TT>().CallCode)>> : std::true_type {};                  \
        CONCAT(Has, MemberName)() {                                                                                                        \
        }                                                                                                                                  \
                                                                                                                                           \
      public:                                                                                                                              \
        static constexpr bool has() {                                                                                                      \
            return CONCAT(HasMember, MemberName)<T>::value;                                                                                \
        }                                                                                                                                  \
    };

template <int N, typename... Ts>
using get_nth_pack_type = typename std::tuple_element_t<N, std::tuple<Ts...>>; // FOR VERSION >= C++26 USE THE "[N]" LOOKUP SYNTAX
template <int N, typename... Ts>
constexpr get_nth_pack_type<N, Ts...> get_nth_pack_value(Ts... pTypes) { // FOR VERSION >= C++26 USE THE "[N]" LOOKUP SYNTAX
    return std::get<N>(std::tuple<Ts...>(pTypes...));
}

template <typename ItType, ItType Start, ItType End, ItType Increment, typename CallableT>
constexpr void static_for(const CallableT& pCallable) {
    if constexpr (Start < End) {
        pCallable.template operator()<Start>(); // Dependent template shit; pCallable(std::integral_constant<ItType, Start>{});
        static_for<ItType, Start + Increment, End, Increment, CallableT>(pCallable);
    }
}
template <int Start, int End, typename CallableT> constexpr void static_for(const CallableT& pCallable) {
    static_for<int, Start, End, 1, CallableT>(pCallable);
}

/*template <typename CallableT, typename ReturnT, typename ...Params>
ReturnT callWithTuple(CallableT pCallable, std::tuple<???> pTuple, Params... pArgs) {
    return std::apply(pCallable, std::tuple_cat(std::make_tuple(pArgs...), pTuple));
}*/

template <typename T> struct TemplateClassInfo;
template <template <typename...> typename ClassT, typename... Ts> // WTF syntax
struct TemplateClassInfo<ClassT<Ts...>> {
    using TemplateTypes = std::tuple<Ts...>;
};

template <typename Signature> struct FunctionPointerInfo;
template <typename ReturnT, typename... Params> struct FunctionPointerInfo<ReturnT (*)(Params...)> {
  public:
    using ReturnType      = ReturnT;
    using ParameterTypes  = std::tuple<Params...>;
    FunctionPointerInfo() = delete;
};
template <typename ReturnT, typename... Params>
struct FunctionPointerInfo<ReturnT (*)(Params..., ...)> : FunctionPointerInfo<ReturnT (*)(Params...)> {}; // Variadic version

template <typename Signature> struct MemberFunctionPointerInfo;
template <typename ClassT, typename ReturnT, typename... Params> struct MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params...)> {
  public:
    using ClassType             = ClassT;
    using ReturnType            = ReturnT;
    using ParameterTypes        = std::tuple<Params...>;
    MemberFunctionPointerInfo() = delete;
};
template <typename ClassT, typename ReturnT, typename... Params>
struct MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params...) const> : MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params...)> {
}; // Const version
template <typename ClassT, typename ReturnT, typename... Params>
struct MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params..., ...)> : MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params...)> {
}; // Variadic version
template <typename ClassT, typename ReturnT, typename... Params>
struct MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params..., ...) const> : MemberFunctionPointerInfo<ReturnT (ClassT::*)(Params...)> {
}; // Const-variadic version
