#pragma once

class Scene {
  private:
  public:
    virtual ~Scene() {
    }
    virtual void start() {}
    virtual void update(float deltaTime, bool buttonPressed) = 0;
};
