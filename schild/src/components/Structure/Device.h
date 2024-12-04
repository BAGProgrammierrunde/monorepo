#pragma once

// Unser "Gerät" als Ganzes, mit all seinen Komponenten und Funktionen
class Device {
private:
    Button button1;
    Button button2;
    Joystick joystick;

public:
    Device();
    void init();
};