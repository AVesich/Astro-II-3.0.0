#include "main.h"

// Motors
//Motor lIntake(L_INTAKE, E_MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES);
//Motor rIntake(R_INTAKE, E_MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES);
Motor lIntake(L_INTAKE, E_MOTOR_GEARSET_06, 1, MOTOR_ENCODER_DEGREES);
Motor rIntake(R_INTAKE, E_MOTOR_GEARSET_06, 0, MOTOR_ENCODER_DEGREES);
Motor internalIntake(I_INTAKE, E_MOTOR_GEARSET_06, 0, MOTOR_ENCODER_DEGREES);
Motor pooperIntake(POOPER, E_MOTOR_GEARSET_06, 1, MOTOR_ENCODER_DEGREES);

// Sensors
ADIAnalogIn towerSens (TOP_BALL);
ADIAnalogIn midBallSens (MID_BALL);
ADIAnalogIn frontBallSens (FRONT_BALL);

Optical opticalSort (OPTICAL);

#define RED_SIG 1
#define BLUE_SIG 2
#define LIGHT_SIG 3

// Define variables
bool allianceColor; // Local declaration of extern bool

void intakeOp() {
    static int intakeSp;
    static int pooperIntakeSp;
    if(master.get_digital(DIGITAL_L1)) {
      intakeSort(100);
    }
      // manual ===========================================
    /*if(master.get_digital(DIGITAL_L1)) {
      intakeSp = 100;
      pooperIntakeSp = intakeSp;
      if(master.get_digital(DIGITAL_A)) {
        pooperIntakeSp = -intakeSp;
      }
      internalIntake.move(intakeSp);
      pooperIntake.move(pooperIntakeSp);
      lIntake.move(intakeSp);
      rIntake.move(intakeSp);
      }*/
      // smartSpeed ======================================
      /*internalIntake.move(smartSpeed(top));
      pooperIntake.move(smartSpeed(pooper));
      lIntake.move(smartSpeed(bottom));
      rIntake.move(smartSpeed(bottom));*/
    else if(master.get_digital(DIGITAL_L2)) { // All intakes backward
      intakeSp = -100;
        internalIntake.move(intakeSp);
        pooperIntake.move(intakeSp);
        lIntake.move(intakeSp);
        rIntake.move(intakeSp);
    }
    else if (master.get_digital(DIGITAL_R1)) { // Front intakes forward
      intakeSp = 100;
        lIntake.move(intakeSp);
        rIntake.move(intakeSp);
    }
    else if (master.get_digital(DIGITAL_R2)) { // Front intakes backward
      intakeSp = -100;
        lIntake.move(intakeSp);
        rIntake.move(intakeSp);
    } else {
      stopIntakes();
    }
}

// UTILITY
void initIntakes() {
  // Motors
  pooperIntake.set_brake_mode(E_MOTOR_BRAKE_HOLD);
  internalIntake.set_brake_mode(E_MOTOR_BRAKE_HOLD);
  lIntake.set_brake_mode(E_MOTOR_BRAKE_HOLD);
  rIntake.set_brake_mode(E_MOTOR_BRAKE_HOLD);

  // Sensors
  towerSens.calibrate();
  midBallSens.calibrate();
  frontBallSens.calibrate();

  opticalSort.set_led_pwm(100); // Put the optical led to 100% brightness
}

int smartSpeed(smartState m_intakeMode) { // Change speeds based on how many balls are in the bot
    static int topIntakeSp = 100;
    static int bottomIntakeSp = 100;
    static int pooperIntakeSp = 100;

    if ((midBallSens.get_value() > 1750) && (frontBallSens.get_value() > 2600)) { // no middle and no front ball
        topIntakeSp = 100;
        bottomIntakeSp = 100;
        pooperIntakeSp = 100;
    }
    if (midBallSens.get_value() > 1750) { // if no middle ball
        topIntakeSp = 100;
        pooperIntakeSp = 0;
        bottomIntakeSp = 100;
    }
    if (frontBallSens.get_value() < 2600) { // if only front ball
        topIntakeSp = 100;
        pooperIntakeSp = 0;
        bottomIntakeSp = 100;
    }
    if ((midBallSens.get_value() < 1750) && (frontBallSens.get_value() > 2600)) { // if middle ball and no front ball
        topIntakeSp = 10;
        pooperIntakeSp = 10;
        bottomIntakeSp = 100;
    }
    if ((midBallSens.get_value() < 1750) && (frontBallSens.get_value() < 2600)) { // middle and front ball
        topIntakeSp = 0;
        pooperIntakeSp = 0;
        bottomIntakeSp = 0;
    }
    if (towerSens.get_value() < 2880) {
      topIntakeSp = 100;
      pooperIntakeSp = 100;
      bottomIntakeSp = 100;

    }
    if (master.get_digital(DIGITAL_A)) {
      pooperIntakeSp = -100;
    }
    if (m_intakeMode == top)
      return topIntakeSp; // top returns top speed
    else if (m_intakeMode == bottom)
      return bottomIntakeSp; // bottom returns bottom speed
      else if (m_intakeMode == pooper)
        return pooperIntakeSp; // pooper returns pooper speed
    else
      return 0; // nothing returns no speed
}

void resetIntakes() {
    internalIntake.tare_position();
    pooperIntake.tare_position();
    lIntake.tare_position();
    rIntake.tare_position();
}

void stopIntakes() {
  internalIntake.move(0);
  pooperIntake.move(0);
  lIntake.move(0);
  rIntake.move(0);
}

// CONTROL

// Int d is distance, sp is speed
void intakeSort(int sp) {
  static int pooperSp = sp;

  auto blueBall =  opticalSort.get_hue() > 150; // Detects light cyan-green all the way to dark blue-purple
  auto redBall =  opticalSort.get_hue() < 40; // Detects from Red (000) to Reddish orange

  if(blueBall && allianceColor == false) { // if red and has blue ball
      pooperSp = -sp;
  }
  else if (redBall && allianceColor == true) { // if blue and has red ball
      pooperSp = -sp;
  } else {
      pooperSp = sp;
  }

  pooperIntake.move(pooperSp);
  internalIntake.move(sp);
  lIntake.move(sp);
  rIntake.move(sp);
}

void intakeAuto(int d, int sp) {
  resetIntakes();
  while(abs(internalIntake.get_position()) < d) {
    intakeSort(sp);
  }
  resetIntakes();
  stopIntakes();
}

void intakeTopAsync(intakeState m_intakeState) {
    if (m_intakeState == in) {
        internalIntake.move(100);
        pooperIntake.move(100);
    } else if (m_intakeState == out) {
        internalIntake.move(-100);
        pooperIntake.move(-100);
    } else if (m_intakeState == off) {
        internalIntake.move(0);
        pooperIntake.move(0);
    }
    resetIntakes();
}

void intakeBottomAsync(intakeState m_intakeState) {
  if (m_intakeState == in) {
      lIntake.move(100);
      rIntake.move(100);
  } else if (m_intakeState == out) {
      lIntake.move(-100);
      rIntake.move(-100);
  } else if (m_intakeState == off) {
      lIntake.move(0);
      rIntake.move(0);
  }
  resetIntakes();
}
