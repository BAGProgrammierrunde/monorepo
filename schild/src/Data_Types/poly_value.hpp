#pragma once

#include "Data_Types/macros.hpp"

namespace me {
    // Basically a smart pointer (std::unique_ptr replacement which is not unique) implementation which is cloneable (kinda proud of it :D)
    template <typename BaseT>
    class poly_value {
    private:
        BaseT* basePtr;
        BaseT*(*cloneFunc)(BaseT*);
        
        void reset() {
            basePtr = nullptr;
            cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* {return nullptr;};
        }
        
    public:
        // Constructor: Default
        poly_value() {
            reset();
        }
        
        // Constructor: Normal
        template <typename DerivedT>
        poly_value(const DerivedT& pDerived) {
            reset();
            set(pDerived);
        }
        
        // Constructor: Copy
        poly_value(const poly_value<BaseT>& pCopyFrom) {
            basePtr = pCopyFrom.cloneFunc(pCopyFrom.basePtr);
            cloneFunc = pCopyFrom.cloneFunc;
        }
        
        // Constructor: Move
        poly_value(poly_value<BaseT>&& pMoveFrom) noexcept {
            basePtr = pMoveFrom.basePtr;
            cloneFunc = pMoveFrom.cloneFunc;
            pMoveFrom.reset();
        }
        
        // Destructor
        ~poly_value() {
            clear();
        }
        
        // Operator: Copy assignment
        poly_value<BaseT>& operator= (const poly_value<BaseT>& pCopyAssignFrom) {
            clear();
            basePtr = pCopyAssignFrom.cloneFunc(pCopyAssignFrom.basePtr);
            cloneFunc = pCopyAssignFrom.cloneFunc;
            return *this;
        }
        
        // Operator: Move assignment
        poly_value<BaseT>& operator= (poly_value<BaseT>&& pMoveAssignFrom) noexcept {
            clear();
            basePtr = pMoveAssignFrom.basePtr;
            cloneFunc = pMoveAssignFrom.cloneFunc;
            pMoveAssignFrom.reset();
            return *this;
        }
        
        // METHODS
        
        bool has() {
            return (basePtr != nullptr);
        }
        
        BaseT* get() {
            return basePtr;
        }
        
        template <typename DerivedT, REQUIRE_DERIVEDT(BaseT, DerivedT)>
        DerivedT* getDerived() {
            return static_cast<DerivedT*>(basePtr);
        }
        
        template <typename DerivedT, REQUIRE_DERIVEDT(BaseT, DerivedT)>
        void set(const DerivedT& pDerived) {
            clear();
            basePtr = new DerivedT(pDerived);
            cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* {return new DerivedT(*static_cast<DerivedT*>(pFromBasePtr));}; // Carries clone derived type info
            // The following line caused me 3 days of pure headache, but I've learned a valuable C++ life lesson about lambda reference captures that day :). (Rookie mistake though I must admit)
            //cloneFunc = [basePtr = this->basePtr]() -> BaseT* {return new DerivedT(*static_cast<DerivedT*>(basePtr));};
        }
        
        template <typename DerivedT, REQUIRE_DERIVEDT(BaseT, DerivedT)>
        void setInPlace(DerivedT* pDerivedRef) { // Option to set without needing temporary object and construct in-place, so (for example) DerivedTs refs to own members don't break cuz of moving/copying as normal set does
            clear();
            basePtr = pDerivedRef;
            cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* {return new DerivedT(*static_cast<DerivedT*>(pFromBasePtr));};
        }
        
        void clear() {
            delete basePtr;
            reset();
        }
    };
};
