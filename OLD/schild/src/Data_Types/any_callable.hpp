#pragma once

#include "Data_Types/macros.hpp"
#include "Data_Types/poly_value.hpp"

namespace me {
    // Basically a std::function implementation with no small object optimization and thus a size of just 16 instead of 32
    // Huge credits to this SO question and its accepted answer!!: https://stackoverflow.com/questions/14936539/how-stdfunction-works
    template <typename Signature>
    class any_callable;
    
    template <typename ReturnT, typename ...Params>
    class any_callable<ReturnT(Params...)> {
    private:
        struct CallableBase {
        public:
            virtual ReturnT operator()(Params... pArgs) = 0;
            virtual ~CallableBase() {}
        };
        
        template <typename T>
        struct Callable : public CallableBase {
        public:
            T callable;
            
            Callable(T pCallable) : callable(pCallable) {}
            
            ReturnT operator()(Params... pArgs) {
                if constexpr (std::is_member_function_pointer<T>()) throw std::bad_function_call();
                else return callable(pArgs...);
            }
            
            template <typename ClassT>
            ReturnT operator()(ClassT* pObjRef, Params... pArgs) {
                if constexpr (!std::is_member_function_pointer<T>() || !std::is_same<ClassT, typename MemberFunctionPointerInfo<T>::ClassType>()) throw std::bad_function_call(); // Dependent typename shit
                else return (pObjRef->*callable)(pArgs...);
            }
        };
        
        template <typename T>
        Callable<T>* getCallableRef() {
            return anyCallable.template getDerived<Callable<T>>(); // Dependent template shit
        }
        
        poly_value<CallableBase> anyCallable;
        
    public:
        any_callable() {}
        
        template <typename T>
        any_callable(const T& pCallable) {
            set(pCallable);
        }
        
        ReturnT operator()(Params... pArgs) { // It doesn't seem to like perfect forwarding here ;c
            return (has() ? (*anyCallable.get())(pArgs...) : ReturnT());
        }
        
        template <typename ClassT>
        ReturnT operator()(ClassT* pObjectRef, Params... pArgs) {
            if (!has()) return ReturnT();
            Callable<ReturnT(ClassT::*)(Params...)>* memberFuncPtrCallable = getCallableRef<ReturnT(ClassT::*)(Params...)>();
            if (memberFuncPtrCallable == nullptr) throw std::bad_function_call();
            return (*memberFuncPtrCallable).template operator()<ClassT>(pObjectRef, pArgs...); // Dependent template shit
        }
        
        bool has() {
            return anyCallable.has();
        }
        
        template <typename ClassT>
        bool isMemberFunctionPointerOf() {
            return (getCallableRef<ReturnT(ClassT::*)(Params...)>() != nullptr);
        }
        
        template <typename T>
        T* getRawCallableRef() {
            Callable<T>* callableRef = getCallableRef<T>();
            return (callableRef != nullptr ? &callableRef->callable : nullptr);
        }
        
        template <typename T>
        void set(const T& pCallable) {
            if constexpr (std::is_member_function_pointer<T>()) // Doesn't produce errors automatically for incompatible memFuncPtrs when setting
            {
                // Dependent typename shit twice :(
                static_assert(std::is_same<ReturnT, typename MemberFunctionPointerInfo<T>::ReturnType>(), "Tried assigning callable with member function pointer of incomatible return type");
                static_assert(std::is_same<std::tuple<Params...>, typename MemberFunctionPointerInfo<T>::ParameterTypes>(), "Tried assigning callable with member function pointer of incomatible parameter types");
            }
            anyCallable.set(Callable<T>(pCallable));
        }
        
        void clear() {
            anyCallable.clear();
        }
    };
};
