#pragma once
#include <array>

class ComponentBase {
public:
    ComponentBase() {}
};

template <unsigned int PinCount>
class Component : public ComponentBase {
protected:
    std::array<unsigned int, PinCount> pins;

public:
    explicit Component(std::array<unsigned int, PinCount> pPins);
    
    unsigned int getPinCount() const;
    bool usesPin(unsigned int pPin) const;
    int getPin(unsigned int pIndex) const;
};

// DAS HIER IST KACKE, REDUNDANT UND UNFLEXIBEL, WIR HABEN ABER KEINEN ANDEREN WEG HIERFUER GEFUNDEN :(
// (FEHLER DURCH AUFTEILUNG IN .hpp UND .cpp UND DARAUS ENTSTEHENDE UNDEFINED REFERENCES)
template class Component<1>;
template class Component<2>;
template class Component<6>;
