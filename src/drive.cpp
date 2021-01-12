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
void driveStraightVelo(double inches, int maxPower, char dir) {
  resetDrive();
  //setBrakeMode(0); // Set brake mode, 0 is coast, 1 is brake, 2 is hold
  setBrakeMode(2);
  const int wheelDiam = 4; // Should be set to the diameter of your drive wheels in inches.
  const double target = (inches / (wheelDiam * M_PI)) * 360 * 1.81; // with 600 carts, it moves about 1.81 times less than a 200 rpm cart, thus x1.81.
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float lPower = 0;
  float rPower = 0;
  float alignErr = 0;
  const float alignKp = 0.15;// Proportional constant used to keep robot straight.
  const float deccelKp = 0.50;
  //const double deccelPt = maxPower/3;
  const double deccelPt = target*deccelKp;

  float lastTime = millis();
  float deltaTime = 0;          // Time between loop runs
  float total_Dt = 0;           // Total deltaTime
  float pctAccelerated = 0;        // Percent completion of the time
  const float curveTime = 650;  // Hardcoded 650ms long acceleration curve

  float distFromTarget = 0;
  float pctToTarget = 0;

  while(avgTicks < target)
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));
    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    //---
    // Accel curve [based on time]
    //---
    deltaTime = (millis()-lastTime);
    lastTime = millis();
    total_Dt += deltaTime;
    pctAccelerated = total_Dt/curveTime;

    //---
    // Deccel curve [based on distance]
    //---
    distFromTarget = target - avgTicks;
    pctToTarget = distFromTarget/deccelPt;

    //---
    // Decide which side is too far ahead, apply alignment and speed corretions.
    //---
    if(lAvgTicks > rAvgTicks)
    {
      lPower = maxPower - alignErr;
      rPower = maxPower;
    }
    else if(rAvgTicks > lAvgTicks)
    {
      rPower = maxPower - alignErr;
      lPower = maxPower;
    }
    else
    {
      lPower = maxPower;
      rPower = maxPower;
    }

    // Reverse speed if necessary
    if(dir == 'b')
    {
      lPower = lPower * -1;
      rPower = rPower * -1;
    }

    //---
    // Accel [Variables declared earlier in function]
    //---
    if (total_Dt <= curveTime) {
      lPower *= pctAccelerated;
      rPower *= pctAccelerated;
    }

    //---
    // Deccel
    //---
    if (avgTicks <= distFromTarget) {
        lPower *= pctToTarget;
        rPower *= pctToTarget;
        if (abs(lPower) && abs(rPower) < 50) {
            lPower *= 50;
            rPower *= 50;
        }
    }

    // Deccel
    /*if(target-avgTicks < deccelPt) {
      setBrakeMode(0);
      lPower = 0;
      rPower = 0;
    }*/

    // Accel
    /*if(deccelPt > avgTicks) {
      lPower /= (maxPower/(((target/10)-avgTicks)));
      rPower /= (maxPower/(((target/10)-avgTicks)));
    }*/

    //---
    // Send velocity targets to both sides of the drivetrain.
    //---
    driveLeftVelo(lPower);
    driveRightVelo(rPower);

    delay(10); // Save brain resources

  }
  //setBrakeMode(2);
  resetDrive();
  return;

}

void driveStraightAuto(double inches, char dir) {
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

void driveToTower(float maxPower, float curveTime, double sensitivity) {
  resetDrive();
  setBrakeMode(1); // Set brake mode, 0 is coast, 1 is brake, 2 is hold
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float currentPower = 0;
  float lPower = 0;
  float rPower = 0;
  float alignErr = 0;   // fAlignErr
  const float alignKp = 0.05; // Proportional constant used to keep robot straight.

  //float motorEff = 1;

  float lastTime = millis();
  float deltaTime = 0;          // Time between loop runs
  float total_Dt = 0;           // Total deltaTime
  float pctComplete = 0;        // Percent completion of the time

  //bool bRun = true;
  auto atTower = towerDist.get() != errno && towerDist.get() < sensitivity;
  while( !atTower )//bRun )
  //while (motorEff > sensitivity) // The average efficiency of both drive sides is higher than 15%
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));
    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    //motorEff = (lFront.get_efficiency() + rFront.get_efficiency())/2.0;
    //cout << motorEff << endl;

    // Accel curve
    deltaTime = (millis()-lastTime);
    lastTime = millis();
    total_Dt += deltaTime;
    pctComplete = total_Dt/curveTime;
    currentPower = pctComplete*maxPower;

    if (currentPower <= 30) // Make sure the motor power is not 0
      currentPower = 30;

    //Decide which side is too far ahead, apply alignment and speed corretions.
    alignErr = abs((lAvgTicks - rAvgTicks)) * alignKp;
    if(lAvgTicks > rAvgTicks)
    {
      lPower = (currentPower) - alignErr;
      rPower = currentPower;
    }
    else if(rAvgTicks > lAvgTicks)
    {
      rPower = (currentPower) - alignErr;
      lPower = currentPower;
    }
    else
    {
      lPower = currentPower;
      rPower = currentPower;
    }

    //Send velocity targets to both sides of the drivetrain.
    driveLeftVelo(lPower);
    driveRightVelo(rPower);

    atTower = towerDist.get() != errno && towerDist.get() < sensitivity; // Update tower status

    delay(10); // Save brain resources

    // Determine efficiency, should it run
    //if( motorEff <= sensitivity ) bRun = false;
  }
  resetDrive();
  return;
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
  const float distKp = 0.025; // Proportional constant used to control speed of robot.
  const float alignKp = 0.45; // Proportional constant used to keep robot straight.
  const float kDecel = 2.0; // Constant used to determine when to decelerate.
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

void driveTurnSkills(double deg, char dir) {
  const double target = deg * 4.93;
  int lAvgTicks = 0;
  int rAvgTicks = 0;
  int avgTicks = 0;
  float currentPower = 0;
  float lPower = 0;
  float rPower = 0;
  float distErr = 0;
  float alignErr = 0;
  float distKp = .15;
  float alignKp = .15;
  const float SLEW = .00175;
  while(avgTicks < target)
  {
    lAvgTicks = abs(getAvgDriveSideDeg('l'));
    rAvgTicks = abs(getAvgDriveSideDeg('r'));
    avgTicks = (lAvgTicks + rAvgTicks) / 2;

    //Make sure we dont accelerate/decelerate too fast with slew
    distErr = (target - avgTicks) * distKp;
    if(distErr > SLEW)
    {
      distErr = SLEW;
    }

    //Decide wether to accelerate or decelerate
    if(currentPower * 2.75 > (target - avgTicks))
    {
      distErr = distErr * -1;
      if(currentPower < 10)
      {
        distErr = 10 - currentPower;
      }
    }

    //Decide which side is too far ahead, apply alignment and speed corretions
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
      rPower = currentPower + distErr;
      lPower = currentPower + distErr;
    }

    //Check to see what direction to turn, adjust motor power accordingly
    if(dir == 'l')
    {
      lPower = lPower * -1;
    }
    else if(dir == 'r')
    {
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
