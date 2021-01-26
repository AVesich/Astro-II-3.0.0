#include "main.h"

Controller master(CONTROLLER_MASTER);

void opcontrol() {
	bTaskExit = true; // Extern variable to stop threads // End any threads that are running if auto didnt finish
	while (true){
		driveOp();
		intakeOp();

		if(master.get_digital(DIGITAL_UP) && competition::is_connected()) // If plugged in (skills/failsafe in match)
			flipout();

		if(master.get_digital(DIGITAL_LEFT) && !competition::is_connected())
			autonomous();

		delay(10);
	}
}
