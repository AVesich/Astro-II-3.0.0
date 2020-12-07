#include "main.h"

void right() {
  resetDrive();
  setBrakeMode(2);
  intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);
  driveStraight(.95 TL, 26);
  delay(750);
  driveTurn(175, 35, 'r'); // Deg, Speed, Dir
  delay(500);
  intakeBottomAsync(in);
  driveStraight(1.2 TL, 35);
  intakeBottomAsync(off);
  delay(1000);
  intakeAuto(450, 100);
  delay(500);
  intakeBottomAsync(in);
  driveStraight(1 TL, -35);
  intakeBottomAsync(off);
}

void right_and_middle() {
  // Corner ==========================
  resetDrive();
  setBrakeMode(2);
  intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);
  driveStraight(.7 TL, 45);
  driveStraight(.25 TL, 20);
  delay(250);
  driveTurn(210, 35, 'r'); // Deg, Speed, Dir
  delay(250);
  intakeTopAsync(in);
  delay(100);
  intakeTopAsync(off);
  intakeBottomAsync(in);
  driveStraight(1.5 TL, 35);
  intakeBottomAsync(off);
  delay(1000);
  intakeAuto(500, 100);
  delay(500);
  intakeBottomAsync(in);
  driveStraight(.7 TL, -45);
  intakeBottomAsync(off);
  // Middle ==========================
  delay(100);
  driveTurn(83, 35, 'l');
  delay(100);
  driveStraight(1.7 TL, -35);
  delay(100);
  driveTurn(172, 35, 'r'); // Deg, Speed, Dir
  delay(100);
  driveStraight(.6 TL, 50);
  intakeAuto(500, 100);
}

void no_auto() {
  // Flipout
  /*intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);*/

  // Temp func testing
  resetDrive();
  driveStraightAuto(12, 'f');
  delay(500);
  driveTurn(180, 35, 'r');
  //driveStraightAuto(24, 'f');
  //driveTurn(180, 35, 'r');
}

void dunk_on_574() {
  resetDrive();
  // Flipout
  intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);
  // Intake first ball
  driveStraightVelo(19, 60, 'f'); // Slower to help intake ball
  delay(100);
  // Turn to face the corner tower
  driveTurn(210, 35, 'l');
  delay(100);
  // Drive up to the corner tower
  intakeBottomAsync(in);
  driveStraightVelo(24, 150,'f');
  intakeBottomAsync(off);
  delay(100);
  // Score first ball in corner
  intakeAuto(500, 100);
  delay(100);
  // Back up from tower
  intakeBottomAsync(in);
  driveStraightVelo(32, 150,'b');
  intakeBottomAsync(off);
  delay(100);
  // Turn towards middle
  driveTurn(225, 35, 'l'); // Turn so back faces wall
  delay(100);
  // Middle edge tower
  driveStraightVelo(15, 150, 'f'); // Drive to middle of field
  delay(100);
  driveTurn(40, 35, 'r'); // Turn to face the tower
  delay(100);
  // Score in edge tower
  driveStraightVelo(17, 150, 'f'); // Drive up to tower
  intakeAuto(1200, 100); // Shoot
  delay(100);
  // Back up and face other corner tower
  //driveStraightVelo(17, 200, 'b'); // Back up from the tower
  // delay(100);
  // TEMP
  intakeBottomAsync(in);
  driveStraightVelo(7, 200, 'b'); // Back up from the tower
  intakeBottomAsync(off);
  delay(100);
  driveStraightVelo(7, 200, 'f');
  intakeAuto(1200, 100); // Shoot
  delay(100);
  intakeBottomAsync(in);
  driveStraightVelo(7, 200, 'b'); // Back up from the tower
  intakeBottomAsync(off);
  // END_TEMP
  /*driveTurn(150, 40, 'l'); // Turn to face the corner wall
  delay(100);
  // Score in corner
  driveStraightVelo(23, 200, 'f'); // Drive to right corner of field
  delay(100);
  driveTurn(10, 40, 'r'); // Turn to face the tower
  delay(100);
  intakeBottomAsync(in); // Intake bottom ball
  driveStraightVelo(37, 90, 'f'); // Drive up to the tower
  intakeTopAsync(off); // Turn off intakes
  delay(100);
  intakeAuto(3000, 100); // Shoot last ball
  driveStraightVelo(5, 200, 'b'); // Pull away quick just in case touching ball (idk if it will count if we are touching)*/
}

void left_and_middle() {
  // Corner ==========================
  resetDrive();
  setBrakeMode(2);
  intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);
  driveStraight(.7 TL, 35);
  driveStraight(.25 TL, 15);
  delay(750);
  driveTurn(138, 35, 'l'); // Deg, Speed, Dir
  delay(500);
  intakeBottomAsync(in);
  driveStraight(1.55 TL, 35);
  intakeBottomAsync(off);
  delay(1000);
  intakeAuto(1000, 100);
  delay(500);
  intakeBottomAsync(in);
  driveStraight(1 TL, -35);
  intakeBottomAsync(off);
  // Middle ==========================
  driveTurn(97, 35, 'r');
  delay(100);
  driveStraight(1.7 TL, -35);
  delay(100);
  driveTurn(120, 35, 'l'); // Deg, Speed, Dir
  delay(100);
  driveStraight(.7 TL, 50);
  intakeAuto(500, 100);
}

void left() {
  resetDrive();
  setBrakeMode(2);
  intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);
  driveStraight(.95 TL, 30);
  delay(750);
  driveTurn(140, 35, 'l'); // Deg, Speed, Dir
  delay(500);
  intakeBottomAsync(in);
  driveStraight(1.1 TL, 35);
  intakeBottomAsync(off);
  delay(1000);
  intakeAuto(450, 100);
  delay(500);
  intakeBottomAsync(in);
  driveStraight(1 TL, -35);
  intakeBottomAsync(off);
}

void skills() {
  resetDrive();
  // Flipout
  intakeTopAsync(out);
  delay(100);
  intakeTopAsync(off);
  // Intake first ball
  intakeBottomAsync(in);
  driveStraightVelo(19, 40, 'f'); // Slower to help intake ball
  intakeBottomAsync(off);
  delay(750);
  // Turn to face the corner tower
  driveTurn(182, 35, 'l');
  delay(500);
  // Drive up to the corner tower
  driveStraightVelo(25, 60,'f');
  delay(1000);
  // Score first ball in corner
  intakeAuto(700, 100);
  delay(500);
  // Back up from tower
  intakeBottomAsync(in);
  driveStraightVelo(36, 60,'b');
  intakeBottomAsync(off);
  // Turn towards middle
  driveTurn(185, 35, 'l'); // Turn so back faces wall
  delay(500);
  // Middle tower
  intakeBottomAsync(in);
  driveStraightVelo(18, 60, 25); // Drive to middle of field
  intakeBottomAsync(off);
  driveTurn(90, 35, 'l'); // Turn to face the tower
  delay(500);
  // Descore middle tower
  driveStraightNoDeccel(1 TL, 100);
  delay(500);
  // Back up and align to score
  driveStraightVelo(12, 60,'b');
  driveTurn(20, 35, 'r');
  // Score middle
  driveStraightVelo(12, 60,'f');
  intakeAuto(700, 100);
  delay(500);
  // Back up from middle
  driveStraightVelo(12, 60,'b');
  delay(500);
  /*
  // Turn to face right wall
  driveTurn(90, 35, 'r');
  delay(500);
  // Go towards wall while intaking to get the ball against the wall
  intakeBottomAsync(in);
  driveStraight(3.5 TL, 30);
  intakeBottomAsync(off);
  // Face back wall
  driveTurn(90, 35, 'r');
  // Drive towards back wall
  driveStraight(5 TL, 30);
  */
}

void Ai_Test() {
  while (true){
    DriveAi("red");
    //IntakeAi();
    delay(10);
  }
}

void Ai_Skills() {
  //DriveAi();
  //IntakeAi();
}

void autonomous() {
  switch(auton) {
    case 0:
    right();
    break;
    case 1:
    right_and_middle();
    break;
    case 2:
    no_auto();
    break;
    case 3:
    dunk_on_574();
    break;
    case 4:
    left_and_middle();
    break;
    case 5:
    left();
    break;
    case 6:
    skills();
    break;
    case 7:
    Ai_Test();
    break;
    case 8:
    Ai_Skills();
    break;
  }
}
