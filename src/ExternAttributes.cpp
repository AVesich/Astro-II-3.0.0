#include "main.h"

// Used for stopping threads in both auto and at the beginning of driver control
bool bTaskExit;

// Used in both intake.cpp and drive.cpp for determining distance from the tower
Distance towerDist (DIST);
