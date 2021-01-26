#include "main.h"

// Used for Tasks === (Declared in externattributes) ============================================================
// bool bTaskExit;

// Smartspeed intake used with tasks
void smartIntake(void* param) {
  while(!bTaskExit) {
    smartSpeedAuto();
    delay(10);
  }
}

void right() {
  resetDrive();
  setBrakeMode(2);
  flipout();
  driveStraightVelo(34,300,'f');
  delay(250);
  driveTurn(76, 35, 'r');
  intakeBottomAsync(INTAKE_IN);
  intakeTopAsync(INTAKE_IN);
  driveStraightVelo(22,300,'f');
  delay(1000); // Get ball
  intakeBottomAsync(INTAKE_OFF);
  intakeTopAsync(INTAKE_OFF);
  driveToTower(300,100,45); // Ram tower after getting ball // Max power, curve time [ms], sensitivity
  intakeAuto(3000, 100);
  intakeBottomAsync(INTAKE_IN); // Prevent possible bottom ball from getting us pushed away from middle tower
  driveStraightVelo(34,300,'b');
  intakeBottomAsync(INTAKE_OFF);
}

void right_and_middle() {
  resetDrive();
  setBrakeMode(2);
  flipout();
  driveStraightVelo(34,300,'f');
  delay(250);
  driveTurn(76, 35, 'r');
  intakeBottomAsync(INTAKE_IN);
  intakeTopAsync(INTAKE_IN);
  driveStraightVelo(22,300,'f');
  delay(1000); // Get ball
  intakeBottomAsync(INTAKE_OFF);
  intakeTopAsync(INTAKE_OFF);
  driveToTower(600,100,45); // Ram tower after getting ball // Max power, curve time [ms], sensitivity
  intakeAuto(2500, 100);
  intakeBottomAsync(INTAKE_IN); // Prevent possible bottom ball from getting us pushed away from middle tower
  driveStraightVelo(35,300,'b');
  intakeBottomAsync(INTAKE_OFF);
  driveTurn(180, 35, 'r');
  intakeBottomAsync(INTAKE_IN);
  driveStraightVelo(36,300,'f');
  delay(250);
  driveTurn(40, 35, 'l');
  driveToTower(350,100,45);
  delay(250);
  intakeBottomAsync(INTAKE_OFF);
  intakeAuto(2000, 100);
  driveStraightVelo(25,300,'b');
  delay(250);
}

void no_auto() {
  // Flipout
  //flipout();
  inertCalib(90.0);
}

void dunk_on_574() {
  resetDrive();
  //setBrakeMode(2);
  driveStraightVelo(36,300,'f');
  delay(250);
  driveTurn(85, 45, 'l');
  driveStraightVelo(22,450,'f');
  delay(750); // Get ball
  driveToTower(300,100,50); // Ram tower after getting ball // Max power, curve time [ms], sensitivity
  delay(750); // Stop and shoot
  driveStraightVelo(34,300,'b');
  delay(250);
  driveTurn(36, 45, 'r');
  driveStraightVelo(78,400,'b');
  delay(250);
  driveTurn(137, 45, 'l');
  driveStraightVelo(22,450,'f');
  delay(750); // Get ball
  driveToTower(300,100,50); // Ram tower after getting ball // Max power, curve time [ms], sensitivity
  delay(750); // Stop and shoot
  driveStraightVelo(10,600,'b'); // Backup for no dq
}

void homerowInit() {
  flipout();
  // Skills multithreading
  Task taskIntake(smartIntake);
  dunk_on_574();
  bTaskExit = true;
}

void left_and_middle() {
  resetDrive();
  setBrakeMode(2);
  flipout();
  driveStraightVelo(34,300,'f');
  delay(250);
  driveTurn(76, 35, 'l');
  intakeBottomAsync(INTAKE_IN);
  intakeTopAsync(INTAKE_IN);
  driveStraightVelo(22,300,'f');
  delay(1000); // Get ball
  intakeBottomAsync(INTAKE_OFF);
  intakeTopAsync(INTAKE_OFF);
  driveToTower(600,100,45); // Ram tower after getting ball // Max power, curve time [ms], sensitivity
  intakeAuto(2500, 100);
  intakeBottomAsync(INTAKE_IN); // Prevent possible bottom ball from getting us pushed away from middle tower
  driveStraightVelo(35,300,'b');
  intakeBottomAsync(INTAKE_OFF);
  delay(5000);
  driveTurn(170, 35, 'l');
  driveStraightVelo(36,300,'f');
  delay(250);
  intakeAuto(2000, 100);
  driveStraightVelo(25,300,'b');
  delay(250);
}

void left() {
  resetDrive();
  setBrakeMode(2);
  flipout();
  driveStraightVelo(34,300,'f');
  delay(250);
  driveTurn(76, 35, 'l');
  intakeBottomAsync(INTAKE_IN);
  intakeTopAsync(INTAKE_IN);
  driveStraightVelo(22,300,'f');
  delay(1000); // Get ball
  intakeBottomAsync(INTAKE_OFF);
  intakeTopAsync(INTAKE_OFF);
  driveToTower(300,100,45); // Ram tower after getting ball // Max power, curve time [ms], sensitivity
  intakeAuto(3000, 100);
  intakeBottomAsync(INTAKE_OUT); // Prevent possible bottom ball from getting us pushed away from middle tower
  driveStraightVelo(34,300,'b');
  intakeBottomAsync(INTAKE_OFF);
}

void oldSkills() {
  resetDrive();
  setBrakeMode(2);
  flipout();

  // Drive to middle
  intakeBottomAsync(INTAKE_IN);
  driveStraightVelo(24,300,'f');
  delay(250);
  driveTurn(33, 65, 'l');
  driveStraightVelo(28,300,'f');
  delay(250);
  intakeBottomAsync(INTAKE_OFF);
  driveTurn(83, 65, 'l');
  intakeAuto(500, 100);
  // Drive up to tower
  driveToTower(300,1000,45); // Max power, curve time [ms], sensitivity
  // Score/descore
  intakeAuto(2250, 100);

  // Backup
  intakeBottomAsync(INTAKE_OUT);
  driveStraightAuto(9,'b');
  intakeBottomAsync(INTAKE_OFF);
  // Drive to corner
  driveTurn(80, 65, 'r');
  intakeBottomAsync(INTAKE_IN);
  driveStraightVelo(45,350,'f');
  intakeBottomAsync(INTAKE_OFF);
  delay(250);
  driveTurn(48, 35, 'l');
  intakeAuto(1000, 100); // Help index ball
  driveToTower(300,2000,45);
  delay(250);
  // Score/descore
  intakeAuto(3000, 100);

  // turn to face other side of field
  driveStraightAuto(24,'b');
  delay(250);
  driveTurn(126.5, 35, 'r');
  // Go to middle edge
  intakeBottomAsync(INTAKE_IN);
  driveStraightVelo(40,300,'f');
  intakeBottomAsync(INTAKE_OFF);
  intakeAuto(1500, 100);
  // Face edge tower
  driveTurn(90, 35, 'l');
  // Drive up to tower
  driveToTower(300,1000,45);
  // Score/descore
  intakeAuto(3000, 100);

  // Back up
  driveStraightAuto(20,'b');
  delay(250);
  // Pickup ball
  driveTurn(85, 65, 'r');
  intakeBottomAsync(INTAKE_IN);
  driveStraightVelo(40,300,'f');
  delay(250);
  driveStraightAuto(13,'b');
  driveTurn(40, 65, 'l');
  driveStraightVelo(26,300,'f');
  delay(250);
  intakeBottomAsync(INTAKE_OFF);
}

// New skills stuff ============================================================
void skillsMovement() {
  // Controls task exit
  bTaskExit = false;

  resetDrive();
  setBrakeMode(2);

  // Drive to middle
  driveStraightVelo(24,350,'f');
  delay(250);
  driveTurn(36,45,'l');
  driveStraightVelo(30,350,'f');
  delay(250);
  driveTurn(85, 45, 'l');
  // Drive up to tower
  driveToTower(300,1000,45); // Max power, curve time [ms], sensitivity
  // Score/descore
  delay(500); // Give bot time to shoot

  // Backup
  driveStraightVelo(10,300,'b');
  // Drive to corner
  driveTurn(83.75, 65, 'r');
  driveStraightVelo(51.75,350,'f');
  delay(250);
  driveTurn(48, 35, 'l');
  driveToTower(600,1000,45);
  delay(250);
  // Score/descore
  delay(500); // Give bot time to shoot

  // turn to face other side of field
  driveStraightVelo(16,450,'b');
  delay(250);
  driveTurn(126.95, 35, 'r');
  // Go to middle edge
  driveStraightVelo(46,450,'f');
  // Face edge tower
  driveTurn(85, 35, 'l');
  // Drive up to tower
  driveToTower(600,1000,45);
  // Score/descore
  delay(850); // Give bot time to shoot

  // Back up
  driveStraightAuto(20,'b');
  delay(250);
  // Pickup ball
  driveTurn(86, 65, 'r');
  driveStraightVelo(55,350,'f');
  delay(250);
  driveStraightAuto(13,'b');
  driveTurn(40, 65, 'l');
  driveToTower(600,1000,45);
  delay(850); // Give bot time to shoot

  // Back middle edge
  driveStraightVelo(48,450,'b'); // Back up from corner
  driveTurn(40, 65, 'l'); // Turn to face wall
}

void skillsInit() {
  flipout();
  // Skills multithreading
  Task taskIntake(smartIntake);
  skillsMovement();
  bTaskExit = true;
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
      homerowInit();
      break;
    case 4:
      left_and_middle();
      break;
    case 5:
      left();
      break;
    case 6:
      skillsInit();
      break;
    case 7:
      Ai_Test();
      break;
    case 8:
      Ai_Skills();
      break;
  }
}
