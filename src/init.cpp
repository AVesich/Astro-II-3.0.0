#include "main.h"

int auton = 0; //The auton that is selected
int accelType = 0;
int selector = 0;
bool color =  false;

void switchAuton (){
  auton++;
  if (auton > 8)
    auton = 0;
}

void switchMirror (){
  color = !color;
  lcd::print(4, "%s", color ? "blue" : "red");
}

void selectors(void* parameter){
  delay(200); // prevent the bug where buttons trigger randomly on startup

  const int autonNum = 9;
  const char *autonNames[autonNum] = {
    "Right",
    "Right_and_middle",
    "No_auto",
    "Home_row",
    "Left_and_middle",
    "Left",
    "Skills",
    "Ai_Test",
    "Ai_Skills"
  };

  //init screen
  lcd::initialize();

  while (1) {
      lcd::set_text(0, "Select an Auton");
      lcd::print(2, "%s", autonNames[auton]);
      lcd::print(4, "%s", color ? "blue" : "red");
      delay(300);

      lcd::register_btn0_cb(switchMirror);
      lcd::register_btn1_cb(autonomous);
      lcd::register_btn2_cb(switchAuton);

      auton = auton;
      allianceColor = color; // False -> Red, True -> Blue
    }
}

void initialize() {
  Task lcd_task(selectors);
  initIntakes();
  //autonomous();
}

void disabled() {}

void competition_initialize() {}
