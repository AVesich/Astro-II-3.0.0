#include "main.h"

namespace Astro_Ai_Field {
    void FieldManager::initSkillsField () {
      Tower cornerTower;
      cornerTower.scoredBalls[0] = blue;
      cornerTower.scoredBalls[1] = blue;
      cornerTower.scoredBalls[2] = none;

      Tower edgeTower;
      edgeTower.scoredBalls[0] = blue;
      edgeTower.scoredBalls[1] = none;
      edgeTower.scoredBalls[2] = none;

      Tower middleTower;
      middleTower.scoredBalls[0] = blue;
      middleTower.scoredBalls[1] = blue;
      middleTower.scoredBalls[2] = blue;

      // Corner towers
      towerCoords[0][0] = cornerTower; // Back left
      towerCoords[0][2] = cornerTower; // Far left
      towerCoords[2][0] = cornerTower; // Back right
      towerCoords[2][2] = cornerTower; // Far right

      // Edge towers
      towerCoords[1][0] = edgeTower; // Back mid
      towerCoords[0][1] = edgeTower; // Mid left
      towerCoords[1][2] = edgeTower; // Far mid
      towerCoords[2][1] = edgeTower; // Mid right

      // The almighty middle tower
      towerCoords[1][1] = middleTower;
    }
}
