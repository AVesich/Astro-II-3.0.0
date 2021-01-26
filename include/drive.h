#ifndef _DRIVE_H_
#define _DRIVE_H_

// General Drive
void driveLeft(int spd);
void driveRight(int spd);

// Auton
int autonSpFast();
int autonSpSlow();

bool isDriving();

void driveStraightVelo(double inches, int maxPower, char dir);
void driveStraightAuto(double inches, char dir);
void driveToTower(float maxPower, float curveTime, double sensitivity);

void inertCalib(double target);

void driveStraight(int t, int sp);
void driveStraightNoDeccel(int t, int sp);
void driveTurn(double deg, int sp, char dir);
void driveTurnSkills(double deg, char dir);

// Driver Control
void driveOp();

// Utility
void resetDrive();
int getAvgDriveSideDeg(char side);
void setBrakeMode(int mode);

#endif
