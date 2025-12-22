#pragma once

#include "macros.h"

// Basically a smart pointer (std::unique_ptr replacement which is not unique) implementation which is cloneable
template <typename BaseT> class PolyValue {
  private:
    BaseT* basePtr;
    BaseT* (*cloneFunc)(BaseT*);

    void reset() {
        basePtr   = nullptr;
        cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* { return nullptr; };
    }

  public:
    // Constructor: Default
    PolyValue() {
        reset();
    }

    // Constructor: Normal
    template <typename DerivedT> PolyValue(const DerivedT& pDerived) {
        reset();
        set(pDerived);
    }

    // Constructor: Copy
    PolyValue(const PolyValue<BaseT>& pCopyFrom) {
        basePtr   = pCopyFrom.cloneFunc(pCopyFrom.basePtr);
        cloneFunc = pCopyFrom.cloneFunc;
    }

    // Constructor: Move
    PolyValue(PolyValue<BaseT>&& pMoveFrom) noexcept {
        basePtr   = pMoveFrom.basePtr;
        cloneFunc = pMoveFrom.cloneFunc;
        pMoveFrom.reset();
    }

    // Destructor
    ~PolyValue() {
        clear();
    }

    // Operator: Copy assignment
    PolyValue<BaseT>& operator=(const PolyValue<BaseT>& pCopyAssignFrom) {
        clear();
        basePtr   = pCopyAssignFrom.cloneFunc(pCopyAssignFrom.basePtr);
        cloneFunc = pCopyAssignFrom.cloneFunc;
        return *this;
    }

    // Operator: Move assignment
    PolyValue<BaseT>& operator=(PolyValue<BaseT>&& pMoveAssignFrom) noexcept {
        clear();
        basePtr   = pMoveAssignFrom.basePtr;
        cloneFunc = pMoveAssignFrom.cloneFunc;
        pMoveAssignFrom.reset();
        return *this;
    }

    // Operator: Arrow (pointer access)
    BaseT* operator->() {
        return basePtr;
    }

    // METHODS

    bool has() {
        return (basePtr != nullptr);
    }

    BaseT* get() {
        return basePtr;
    }

    template <typename DerivedT, REQUIRE_DERIVEDT(BaseT, DerivedT)> DerivedT* getDerived() {
        return static_cast<DerivedT*>(basePtr);
    }

    template <typename DerivedT, REQUIRE_DERIVEDT(BaseT, DerivedT)> void set(const DerivedT& pDerived) {
        clear();
        basePtr   = new DerivedT(pDerived);
        cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* { return new DerivedT(*static_cast<DerivedT*>(pFromBasePtr)); };
    }

    template <typename DerivedT, REQUIRE_DERIVEDT(BaseT, DerivedT)>
    void setInPlace(DerivedT* pDerivedRef) { // Option to set without needing temporary object and construct in-place, so (for example)
                                             // DerivedTs refs to own members don't break cuz of moving/copying as normal set does
        clear();
        basePtr   = pDerivedRef;
        cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* { return new DerivedT(*static_cast<DerivedT*>(pFromBasePtr)); };
    }

    void clear() {
        delete basePtr;
        reset();
    }
};
