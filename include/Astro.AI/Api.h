#ifndef _API_H_
#define _API_H_

#include <string>
using namespace std;

namespace Astro_Ai {
  void DriveAi(string allyColor);
  void IntakeAi();
}

namespace Astro_Ai_Field {
  enum Ball { red, blue, none };

  class Tower {
  public:
    const static int capacity = 3; // Const -> the variable should never change
    Ball scoredBalls[capacity]; // Each tower can hold 3 balls (Array sees these as slot 0, 1, 2)
    /*
    Slot 0 is bottom, slot 1 is middle, slot 2 is top
    */
  };

  class FieldManager {
    Tower towerCoords[3][3]; // Coordinates for towers, first dimension is x axis, second dimension is y axis
    void initSkillsField();
  };
}

#endif
