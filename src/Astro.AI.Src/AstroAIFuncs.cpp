#include "main.h"

// Motors - All are declared in other .cpp files, if they aren't then declare them here.

// Sensors - most are declared in other .cpp files, only declare the ones that haven't been declared yet.
/*
I use "#define PORT_NAME 0" in my main.h file to define all of my ports, so replace, for example,
"DIST" in the distance sensor declaration with the port number that it is
plugged in to if your ports are not declared the same fashion that mine are
*/

/*ADIAnalogIn towerSens (TOP_BALL);
ADIAnalogIn midBallSens (MID_BALL);
ADIAnalogIn frontBallSens (FRONT_BALL);

Optical opticalSort (OPTICAL);
Distance towerDist (DIST);*/
Imu driveInert (INERT);
Vision driveVis (VIS);

// Ball color signatures
#define RED_SIG 1
#define BLUE_SIG 2

namespace Astro_Ai {
  void DriveAi (string allyColor) {
    vision_object_s_t redBall = driveVis.get_by_sig(0, RED_SIG);
    vision_object_s_t blueBall = driveVis.get_by_sig(0, BLUE_SIG);

    if ((redBall.signature == RED_SIG) && (allyColor == "red")) {
      // Adjust so robot is aligned with the ball
      if (redBall.x_middle_coord > 100) { // If ball is too far right
        driveLeft(25);
        driveRight(-25);

      }
      if (redBall.x_middle_coord < -100) { // If ball is too far left
        driveLeft(-25);
        driveRight(25);

      }
      if ((redBall.x_middle_coord < -100) && (redBall.x_middle_coord > 100)) { // If ball is in middle
        if (redBall.y_middle_coord > -150) { // If ball is too far forward
          driveLeft(25);
          driveRight(-25);
        }
        if (redBall.y_middle_coord < 150) { // If ball is too far back
          driveLeft(-25);
          driveRight(25);
        }
        if ((redBall.y_middle_coord > -150) && (redBall.y_middle_coord < 150)) { // If ball is in middle
          // Nothing, ball is aligned
        }
      }
    }
  }

  void IntakeAi () {

  }
}
