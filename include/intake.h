#ifndef _INTAKE_H_
#define _INTAKE_H_

void intakeOp();

void stopIntakes();

enum smartState { top, bottom, pooper };
int smartSpeed(smartState m_intakeMode);
void initIntakes();

void intakeSort(int sp);
void intakeAuto(int d, int sp);
enum intakeState { in, out, off };
void intakeTopAsync(intakeState m_intakeState);
void intakeBottomAsync(intakeState m_intakeState);

#endif
