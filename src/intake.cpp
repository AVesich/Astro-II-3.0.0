#include "main.h"

// Motors
//Motor lIntake(L_INTAKE, E_MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES);
//Motor rIntake(R_INTAKE, E_MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES);
Motor lIntake(L_INTAKE, E_MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES);
Motor rIntake(R_INTAKE, E_MOTOR_GEARSET_18, 0, MOTOR_ENCODER_DEGREES);
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
      intakeSort(560);
      //smartSpeedAuto();
    }
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
    } else if (master.get_digital(DIGITAL_X)) {
        internalIntake.move_velocity(smartSpeed(top));
        pooperIntake.move_velocity(smartSpeed(pooper));
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
    static int topIntakeSp = 600;
    static int bottomIntakeSp = 200;
    static int pooperIntakeSp = 600;

    auto blueBall =  opticalSort.get_hue() > 150; // Detects light cyan-green all the way to dark blue-purple
    auto redBall =  opticalSort.get_hue() < 40; // Detects from Red (000) to Reddish orange

    auto frontBall = frontBallSens.get_value() < 2600;
    auto midBall = midBallSens.get_value() < 2600;

    if (!midBall && !frontBall) { // no middle and no front ball
        topIntakeSp = 600;
        bottomIntakeSp = 200;
        pooperIntakeSp = 600;
    }
    if (!midBall) { // if no middle ball
        topIntakeSp = 400;
        pooperIntakeSp = 400;
        bottomIntakeSp = 200;
    }

    if (frontBall) { // if only front ball
        topIntakeSp = 400;
        pooperIntakeSp = 400;
        bottomIntakeSp = 200;
    }

    if (midBall && !frontBall) { // if middle ball and no front ball
      if(blueBall && allianceColor == false) { // if red and has blue ball
        topIntakeSp = 400;
        pooperIntakeSp = -400;
        bottomIntakeSp = 200;
      }
      else if (redBall && allianceColor == true) { // if blue and has red ball
        topIntakeSp = 400;
        pooperIntakeSp = -400;
        bottomIntakeSp = 200;
      } else {
        topIntakeSp = 0;
        pooperIntakeSp = 0;
        bottomIntakeSp = 200;
      }
    }

    if (midBall && frontBall) { // middle and front ball
      if(blueBall && allianceColor == false) { // if red and has blue ball
        topIntakeSp = 400;
        pooperIntakeSp = -400;
        bottomIntakeSp = 200;
      }
      else if (redBall && allianceColor == true) { // if blue and has red ball
        topIntakeSp = 400;
        pooperIntakeSp = -400;
        bottomIntakeSp = 200;
      } else {
        topIntakeSp = 0;
        pooperIntakeSp = 0;
        bottomIntakeSp = 0;
      }
    }

    if (towerDist.get() != errno && towerDist.get() < 45) {
      if(blueBall && allianceColor == false) { // if red and has blue ball
        topIntakeSp = 400;
        pooperIntakeSp = -400;
        bottomIntakeSp = 200;
      }
      else if (redBall && allianceColor == true) { // if blue and has red ball
        topIntakeSp = 400;
        pooperIntakeSp = -400;
        bottomIntakeSp = 200;
      } else {
      topIntakeSp = 600;
      pooperIntakeSp = 600;
      bottomIntakeSp = 200;
      }
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

void smartSpeedAuto() {
  internalIntake.move_velocity(smartSpeed(top));
  pooperIntake.move_velocity(smartSpeed(pooper));
  lIntake.move_velocity(smartSpeed(bottom));
  rIntake.move_velocity(smartSpeed(bottom));
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
  static int bottomSp;

  if (sp > 0)
    bottomSp = 600;
  if (sp < 0)
    bottomSp = -600;

  auto blueBall =  opticalSort.get_hue() > 50; // Detects light cyan-green all the way to dark blue-purple
  auto redBall =  opticalSort.get_hue() < 40; // Detects from Red (000) to Reddish orange

  if(blueBall && allianceColor == false) { // if red and has blue ball
      pooperSp = -sp;
  }
  else if (redBall && allianceColor == true) { // if blue and has red ball
      pooperSp = -sp;
  } else {
      pooperSp = sp;
  }



  pooperIntake.move_velocity(pooperSp);
  internalIntake.move_velocity(sp);
  lIntake.move_velocity(bottomSp);
  rIntake.move_velocity(bottomSp);
}

void intakeAuto(int d, int sp) {
  resetIntakes();
  double avgIntakes = 0.0;
  while(avgIntakes/*abs(internalIntake.get_position())*/ < d) {
    avgIntakes = abs(internalIntake.get_position());
    intakeSort(sp*6);
    delay(10);
  }
  resetIntakes();
  stopIntakes();
}

void intakeTopAsync(intakeState m_intakeState) {
    if (m_intakeState == INTAKE_IN) {
        internalIntake.move(100);
        pooperIntake.move(100);
    } else if (m_intakeState == INTAKE_OUT) {
        internalIntake.move(-100);
        pooperIntake.move(-100);
    } else if (m_intakeState == INTAKE_AUTO) {
        internalIntake.move_velocity(smartSpeed(top));
        pooperIntake.move_velocity(smartSpeed(pooper));
    } else if (m_intakeState == INTAKE_OFF) {
        internalIntake.move(0);
        pooperIntake.move(0);
    }
    resetIntakes();
}

void intakeBottomAsync(intakeState m_intakeState) {
  if (m_intakeState == INTAKE_IN) {
      lIntake.move(100);
      rIntake.move(100);
  } else if (m_intakeState == INTAKE_OUT) {
      lIntake.move(-100);
      rIntake.move(-100);
  } else if (m_intakeState == INTAKE_AUTO) {
      lIntake.move_velocity(smartSpeed(bottom));
      rIntake.move_velocity(smartSpeed(bottom));
  } else if (m_intakeState == INTAKE_OFF) {
      lIntake.move(0);
      rIntake.move(0);
  }
  resetIntakes();
}

void flipout() {
  pooperIntake.move(-100); // Intake deploy
  delay(250);
  internalIntake.move(100); // Hood deploy
  delay(250);
  pooperIntake.move(0);
  internalIntake.move(0);
}
