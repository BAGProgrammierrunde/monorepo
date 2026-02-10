#pragma once

#include "Device.h"

class Scene {
  private:
  public:
    virtual ~Scene() = default;
    virtual void start(Device& device) {}
    virtual void update(float deltaTime) = 0;
};
