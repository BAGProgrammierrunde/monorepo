#pragma once

class Scene {
private:

public:
    Scene();
    //virtual void handleEvent(/*ComponentEvent pEvent or so in future*/);
    virtual void update() = 0;
};