#include "main.h"

// Default maximum speed
#define MAXSP 95;

// Current maximum speed (can be changed)
int currMaxSpeed = MAXSP;

// Motors
Motor lFront(LEFTFRONT, E_MOTOR_GEARSET_06, 0, MOTOR_ENCODER_DEGREES);
Motor lBack(LEFTREAR, E_MOTOR_GEARSET_06, 0, MOTOR_ENCODER_DEGREES);
Motor rFront(RIGHTFRONT, E_MOTOR_GEARSET_06, 1, MOTOR_ENCODER_DEGREES);
Motor rBack(RIGHTREAR, E_MOTOR_GEARSET_06, 1, MOTOR_ENCODER_DEGREES);

// General Drive ========================
// Drive from 0-100
void driveLeft(int spd) {
  lFront.move(spd);
  lBack.move(spd);
}

void driveRight(int spd) {
  rFront.move(spd);
  rBack.move(spd);
}

// Drive using velocity 0-max rpm
void driveLeftVelo(int velo) {
  lFront.move_velocity(velo);
  lBack.move_velocity(velo);
}

void driveRightVelo(int velo) {
  rFront.move_velocity(velo);
  rBack.move_velocity(velo);
}

// Feedback =============================
bool isDriving() {
    static int count = 0;
    static int last = 0;

    int curr = lFront.get_position();

    // If the degrees are at 5, and the last checked degrees are 0, abs(0-5) would be 5, causing the count to be 0;
    if(abs(last-curr) < 3)
      count++;
    else
      count = 0;

    last = curr;

    // If the count is above 4 (it has not moved for 5+ ticks) we are not driving, otherwise we are
    if (count > 4)
      return false;
    else
      return true;
}

// Auton ================================
// Setup auton speeds
int autonSpFast() {
  double speed;
  speed = currMaxSpeed;
  speed *= .75;
  return speed;
}

int autonSpSlow() {
  double speed;
  speed = currMaxSpeed;
  speed *= .25;
  return speed;
}

int turnSp(double deg) {
    // Max degrees should be 360, 360/10 * 2.75 = 99
    double speed;
    double calcDeg;
    calcDeg = deg/10 * 2.75;
    if (speed <= 25)
      speed = 25;
    else
      speed = calcDeg;
    return speed;
}

// Actual driving functions ==========================================================
// variables are tiles (t), direction (dir) (1 forward, -1 backward)

// PID From 1233F, functions and variables adapted to be uniform with the rest of my code
void driveStraightVelo(int inches, int sp, char dir) {
  resetDrive();
  setBrakeMode(0); // Set brake mode, 0 is coast, 1 is brake, 2 is hold
  const int wheelDiam = 4; // Should be set to the diameter of your drive wheels in inches.
  const int target = (inches / (wheelDiam * M_PI)) * 360; // Drive based on inches
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float lPower = 0;
  float rPower = 0;
  float alignErr = 0;
  const float alignKp = 0.15;// Proportional constant used to keep robot straight.
  while(avgTicks < target)
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));
    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    // Decide which side is too far ahead, apply alignment and speed corretions.
    if(lAvgTicks > rAvgTicks)
    {
      lPower = sp - alignErr;
      rPower = sp;
    }
    else if(rAvgTicks > lAvgTicks)
    {
      rPower = sp - alignErr;
      lPower = sp;
    }
    else
    {
      lPower = sp;
      rPower = sp;
    }

    if(dir == 'b')
    {
      lPower = lPower * -1;
      rPower = rPower * -1;
    }

    // Send velocity targets to both sides of the drivetrain.
    driveLeftVelo(lPower);
    driveRightVelo(rPower);
  }
  resetDrive();
  return;

}

void driveStraightAuto(int inches, char dir) {
  resetDrive();
  setBrakeMode(1); // Set brake mode, 0 is coast, 1 is brake, 2 is hold
  const int wheelDiam = 4;//Should be set to the diameter of your drive wheels in inches.
  const double target = (inches / (wheelDiam * M_PI)) * 360 * 1.81; // with 600 carts, it moves about 1.81 times less than a 200 rpm cart, thus x1.81.
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float currentPower = 0;
  float lPower = 0;
  float rPower = 0;
  float distErr = 0;
  float alignErr = 0;
  const float distKp = 0.002; // Proportional constant used to control speed of robot.
  const float alignKp = 0.15; // Proportional constant used to keep robot straight.
  const float kDecel = 1.9; // Constant used to determine when to decelerate.
  const float SLEW = .004; // Constant used to control acceleration in RPM/cycle.
  while(avgTicks < target)
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));
    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    //Make sure we dont accelerate/decelerate too fast with slew.
    distErr = (target - avgTicks) * distKp;
    if(distErr > SLEW)
    {
      distErr = SLEW;
    }

    //Decide wether to accelerate or decelerate.
    if(currentPower * kDecel > (target - avgTicks))
    {
      distErr = distErr * -1;
      if(currentPower < 5)
      {
        distErr = 5 - currentPower;
      }
    }

    //Decide which side is too far ahead, apply alignment and speed corretions.
    alignErr = abs((lAvgTicks - rAvgTicks)) * alignKp;
    if(lAvgTicks > rAvgTicks)
    {
      lPower = (currentPower + distErr) - alignErr;
      rPower = currentPower + distErr;
    }
    else if(rAvgTicks > lAvgTicks)
    {
      rPower = (currentPower + distErr) - alignErr;
      lPower = currentPower + distErr;
    }
    else
    {
      lPower = currentPower + distErr;
      rPower = currentPower + distErr;
    }

    //Check what direction we should go, change motor velocities accordingly.
    if(dir == 'b')
    {
      lPower = lPower * -1;
      rPower = rPower * -1;
    }

    //Send velocity targets to both sides of the drivetrain.
    driveLeftVelo(lPower);
    driveRightVelo(rPower);

    // Set current power for next cycle, make sure it doesn't get too high/low.
    /*As a side note, the distance(in ticks) at which deceleration starts is
      determined by the upper limit on currentPower's and kDecel's product.*/
    currentPower = currentPower + distErr;
    if(currentPower > 200)
    {
      currentPower = 200;
    }
    else if(currentPower < 0)
    {
      currentPower = 0;
    }
  }
  resetDrive();
  return;
}

void driveStraight(int t, int sp) {

  resetDrive();

  //declare variables
  double lPos = 0.0;
  double rPos = 0.0;
  float err = 0.0;

  float leftSp = sp;
  float rightSp = sp;

  float intakeSp = 100;

  //const, because this value shouuld only ever be changes manually in the code, not while the robot is running.
  const float fixAmt = 1.1;

  while (abs(lPos) < t) {
    // Left os behind
    if ((abs(lPos)-abs(rPos)) < 0) {
      // Set err to the difference in degrees
      err = abs(lPos)-abs(rPos);
      // Set the speed to the inputted speed * positive err * fixAmt
      leftSp = (sp * abs(err) * fixAmt);
      rightSp = sp;
    }
    // Right is behind
    if ((abs(rPos)-abs(lPos)) < 0) {
      // Set err to the difference in degrees
      err = abs(lPos)-abs(rPos);
      // Set the speed to the inputted speed * positive err * fixAmt
      leftSp = sp;
      rightSp = (sp * abs(err) * fixAmt);
    }
    // Both sides are equal
    if ((abs(lPos)-abs(rPos)) == 0) {
      leftSp = sp;
      rightSp = sp;
    }

    // Deccel
    if (abs(lPos) > (t*.75)) { // if we are 75% of the way there
        leftSp /= 2;//log10(t-abs(lPos))*((t*.75)/2);
        rightSp /= 2;//log10(t-abs(lPos))*((t*.75)/2);
    } else if (abs(lPos) > (t*.85)) { // if we are 85% of the way there
        leftSp /= 3;
        rightSp /= 3;
    }
    // Set both sides to the inputted speed
    driveLeft(leftSp);
    driveRight(rightSp);

    // Update values
    lPos = lFront.get_position();
    rPos = rFront.get_position();
  }
  resetDrive();
}

void driveStraightNoDeccel(int t, int sp) {

  resetDrive();

  //declare variables
  double lPos = 0.0;
  double rPos = 0.0;
  float err = 0.0;

  float leftSp = sp;
  float rightSp = sp;

  float intakeSp = 100;

  //const, because this value shouuld only ever be changes manually in the code, not while the robot is running.
  const float fixAmt = 1.1;

  while (abs(lPos) < t) {
    // Left os behind
    if ((abs(lPos)-abs(rPos)) < 0) {
      // Set err to the difference in degrees
      err = abs(lPos)-abs(rPos);
      // Set the speed to the inputted speed * positive err * fixAmt
      leftSp = (sp * abs(err) * fixAmt);
      rightSp = sp;
    }
    // Right is behind
    if ((abs(rPos)-abs(lPos)) < 0) {
      // Set err to the difference in degrees
      err = abs(lPos)-abs(rPos);
      // Set the speed to the inputted speed * positive err * fixAmt
      leftSp = sp;
      rightSp = (sp * abs(err) * fixAmt);
    }
    // Both sides are equal
    if ((abs(lPos)-abs(rPos)) == 0) {
      leftSp = sp;
      rightSp = sp;
    }

    // Set both sides to the inputted speed
    driveLeft(leftSp);
    driveRight(rightSp);

    // Update values
    lPos = lFront.get_position();
    rPos = rFront.get_position();
  }
  resetDrive();
}

// Same variables as driveStraight
void driveTurn(double deg, int sp, char dir) {
  resetDrive();
  setBrakeMode(1); // Set brake mode, 0 is coast, 1 is brake, 2 is hold
  const double turnAmt = deg * 4.93; //4.80 old
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float currentPower = 0;
  float lPower = 0;
  float rPower = 0;
  float distErr = 0;
  float alignErr = 0;
  const float distKp = 0.01; // Proportional constant used to control speed of robot.
  const float alignKp = 0.45; // Proportional constant used to keep robot straight.
  const float kDecel = 3.0; // Constant used to determine when to decelerate.
  const float SLEW = .001; // Constant used to control acceleration in RPM/cycle.
  while(avgTicks < turnAmt)
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));
    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    //Make sure we dont accelerate/decelerate too fast with slew.
    distErr = (turnAmt - avgTicks) * distKp;
    if(distErr > SLEW)
    {
      distErr = SLEW;
    }

    //Decide wether to accelerate or decelerate.
    if(currentPower * kDecel > (turnAmt - avgTicks))
    {
      distErr = distErr * -1;
      if(currentPower < 5)
      {
        distErr = 5 - currentPower;
      }
    }

    //Decide which side is too far ahead, apply alignment and speed corretions.
    alignErr = abs((lAvgTicks - rAvgTicks)) * alignKp;
    if(lAvgTicks > rAvgTicks)
    {
      lPower = (currentPower + distErr) - alignErr;
      rPower = currentPower + distErr;
    }
    else if(rAvgTicks > lAvgTicks)
    {
      rPower = (currentPower + distErr) - alignErr;
      lPower = currentPower + distErr;
    }
    else
    {
      lPower = currentPower + distErr;
      rPower = currentPower + distErr;
    }

    //Check what direction we should go, change motor velocities accordingly.
    if(dir == 'l')
    {
      lPower = lPower * -1;
      rPower = rPower;
    } else {
      lPower = lPower;
      rPower = rPower * -1;
    }

    //Send velocity targets to both sides of the drivetrain.
    driveLeftVelo(lPower);
    driveRightVelo(rPower);

    // Set current power for next cycle, make sure it doesn't get too high/low.
    /*As a side note, the distance(in ticks) at which deceleration starts is
      determined by the upper limit on currentPower's and kDecel's product.*/
    currentPower = currentPower + distErr;
    if(currentPower > 600)
    {
      currentPower = 600;
    }
    else if(currentPower < 0)
    {
      currentPower = 0;
    }
  }
  resetDrive();
  return;
}

void driveTurnWide(double deg, char dir) {

    resetDrive();

    // Left
    if(dir == 'l') {
        while (rFront.get_position() < deg) {
            //driveRight(39);
            //driveLeft(6);
            driveRight(60);
            driveLeft(12);
        }
    }
    // Right
    else if (dir == 'r') {
        while (lFront.get_position() < deg) {
            //driveRight(6);
            //driveLeft(39);
            driveRight(12);
            driveLeft(60);
        }
    }
    resetDrive();
}

// DriveOp ==============================
void driveOp() {
  setBrakeMode(0); // Set brake mode to coast
  int lStick = master.get_analog(ANALOG_LEFT_Y);
  int rStick = master.get_analog(ANALOG_LEFT_X);

  if (abs(rStick) < 20) { // Eliminate accidental turning
    lStick *= 1.5; // 50% extra forward/back influence
    rStick *= 0.5; // 50% less turning influence
  }

  driveLeft(lStick + rStick);
  driveRight(lStick - rStick);
}

// Utility ==============================
void resetDrive() {
  lFront.tare_position();
  lBack.tare_position();
  rFront.tare_position();
  rBack.tare_position();
  driveLeft(0);
  driveRight(0);
}

int getAvgDriveSideDeg(char side) {
  int avg = 0;

  if (side == 'l') {
    avg = (lFront.get_position() + lBack.get_position())/2;
  }
  else if (side == 'r') {
    avg = (rFront.get_position() + rBack.get_position())/2;
  }

  return avg;
}

void clrMods() {
  currMaxSpeed = MAXSP;
  setBrakeMode(0);
}

void setSp(int newSp) {
  currMaxSpeed = newSp;
}

void setBrakeMode(int mode) {
  motor_brake_mode_e_t brakeMode;
  switch(mode){
    case 0:
      brakeMode = E_MOTOR_BRAKE_COAST;
      break;
    case 1:
      brakeMode = E_MOTOR_BRAKE_BRAKE;
      break;
    case 2:
      brakeMode = E_MOTOR_BRAKE_HOLD;
      break;
  }

  lFront.set_brake_mode(brakeMode);
  lBack.set_brake_mode(brakeMode);
  rFront.set_brake_mode(brakeMode);
  rBack.set_brake_mode(brakeMode);
}
