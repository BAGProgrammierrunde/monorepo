#pragma once

class Scene {
private:

public:
    Scene();
    virtual Scene* update(float deltaTime, bool buttonPressed) = 0;
};
