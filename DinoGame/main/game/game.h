#pragma once

class Game {
  private:
    int shift = 0;
    int nextStep = 0;

  public:
    void init();
    void loop();
};
