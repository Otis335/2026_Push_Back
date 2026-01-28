#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

brain Brain;

inertial Inertial(PORT21); 

controller Controller;
motor FrontRight = motor(PORT11, ratio6_1, false); 
motor FrontLeft = motor(PORT14, ratio6_1, true);
motor BackRight = motor(PORT13, ratio6_1, false);
motor BackLeft = motor(PORT12, ratio6_1, true);

motor Intake = motor(PORT8, ratio6_1, true);
motor Outtake = motor(PORT6, ratio6_1, false);
pneumatics Matchloader = pneumatics(Brain.ThreeWirePort.A);
pneumatics Wing = pneumatics(Brain.ThreeWirePort.B);

void vexcodeInit(void) {}

motor_group Left = motor_group(FrontLeft, BackLeft);
motor_group Right = motor_group(FrontRight, BackRight);
motor_group ScoreLong = motor_group(Intake, Outtake);