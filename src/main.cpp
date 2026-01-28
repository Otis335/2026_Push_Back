
#include "vex.h"

using namespace vex;

competition Competition;


void pre_auton(void) {

}



void autonomous(void) {
 
}



void usercontrol(void) {
  
 while (1) {
    float throttle = Controller.Axis3.value();
    if (throttle < 5 && throttle > -5) {
      throttle = 0;
    }
    throttle = throttle / 1.27;

    float turn = Controller.Axis1.value();
    if (turn < 5 && turn > -5) {
      turn = 0;
    }
    turn = turn / 1.27;

    throttle = pow(throttle, 3) / 10000;
    turn = pow(turn, 3) / 10000;

    float leftpow = throttle + turn;
    float rightpow = throttle - turn;

    if (leftpow == 0) {
      Left.stop(brake);
    }
    if (rightpow == 0) {
      Right.stop(brake);
    }

    Left.spin(fwd, (leftpow * 0.12), volt);
    Right.spin(fwd, (rightpow * 0.12), volt);

    wait(20, msec);
  }
}

int main() {
  vexcodeInit();

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  };
}


int main() {
 
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();


  while (true) {
    wait(100, msec);
  }
}
