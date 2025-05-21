#pragma once

#include "Data_Types/macros.hpp"

namespace me {
    // Basically a std::unique_ptr implementation which is cloneable
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
        template <DERIVED_TYPENAME(BaseT)>
        poly_value(DerivedT pDerived) {
            reset();
            set(pDerived);
        }

        // Constructor: Copy
        poly_value(const poly_value<BaseT> &pCopyFrom) {
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
        poly_value<BaseT>& operator= (const poly_value<BaseT> &pCopyAssignFrom) {
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

        template <DERIVED_TYPENAME(BaseT)>
        DerivedT* getDerived() {
            DerivedT* derivedPtr = dynamic_cast<DerivedT*>(basePtr);
            return (derivedPtr ? derivedPtr : nullptr);
        }

        template <DERIVED_TYPENAME(BaseT)>
        void set(DerivedT pDerived) {
            clear();
            basePtr = new DerivedT(pDerived);
            cloneFunc = [](BaseT* pFromBasePtr) -> BaseT* {return new DerivedT(*static_cast<DerivedT*>(pFromBasePtr));};
            //cloneFunc = [basePtr = this->basePtr]() -> BaseT* {return new DerivedT(*static_cast<DerivedT*>(basePtr));}; <-- Caused me 3 days of pure headache (capture)
        }

        void clear() {
            delete basePtr;
            reset();
        }
    };
};
