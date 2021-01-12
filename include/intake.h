#ifndef _INTAKE_H_
#define _INTAKE_H_

void intakeOp();

void stopIntakes();

enum smartState { top, bottom, pooper };
int smartSpeed(smartState m_intakeMode);
void initIntakes();
void resetIntakes();

void intakeSort(int sp);
void intakeAuto(int d, int sp);
void smartSpeedAuto();
enum intakeState { INTAKE_IN, INTAKE_OUT, INTAKE_AUTO, INTAKE_OFF };
void intakeTopAsync(intakeState m_intakeState);
void intakeBottomAsync(intakeState m_intakeState);
void flipout();

#endif
