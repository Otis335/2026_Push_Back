#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

brain Brain;

inertial Inertial(PORT21); 

controller Controller;
motor FrontRight = motor(PORT11, ratio18_1, false); 
motor FrontLeft = motor(PORT14, ratio18_1, true);
motor BackRight = motor(PORT13, ratio18_1, false);
motor BackLeft = motor(PORT12, ratio18_1, true);

void vexcodeInit(void) {}

motor_group Left = motor_group(FrontLeft, BackLeft);
motor_group Right = motor_group(FrontRight, BackRight);