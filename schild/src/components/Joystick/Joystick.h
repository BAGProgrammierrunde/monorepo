#pragma once

class Joystick : public Component<2> {
private:
    float threshold;

    float readAsPercent(unsigned int pPin);

public:
    Button button;
    
    explicit Joystick(unsigned int pPinX, unsigned int pPinY, unsigned int pPinButton, float pThreshold);

    float getX();
    float getY();
};