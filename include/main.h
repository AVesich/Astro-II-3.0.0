#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

// Main includes
#include "api.h"
#include "drive.h"
#include "intake.h"

// Ports
  // Broken
  /*
  1
  */

  // Motors
#define LEFTFRONT 10
#define LEFTREAR 9
#define RIGHTFRONT 4
#define RIGHTREAR 2
#define R_INTAKE 5
#define L_INTAKE 8
#define I_INTAKE 13
#define POOPER 11

  // Sensors
#define TOP_BALL 'F'
#define MID_BALL 'G'
#define FRONT_BALL 'H'
#define OPTICAL 18
#define DIST 19
#define INERT 3
#define VIS 20

// Definition of a tile in encoder ticks
#define TL *545

// Other declarations/Ai stuff
#include "Astro.AI/Api.h"

using namespace pros;
using namespace pros::literals;
using namespace std;

// AI
using namespace Astro_Ai;
using namespace Astro_Ai_Field;

// Extern Controller
extern Controller master;

// Extern Distance
extern Distance towerDist;

// Extern Inertial
extern Imu driveInert;

// Extern variables
extern bool allianceColor;
extern int auton;
extern bool bTaskExit;
/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif  // _PROS_MAIN_H_
