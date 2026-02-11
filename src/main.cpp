
#include "vex.h"
#include "auton.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
    vexcodeInit();

    Inertial.calibrate();
    while (Inertial.isCalibrating()) {
        wait(50, msec);
    }
    DriveTrain.setTimeout(3000, msec);
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

    Left.spin(reverse, (leftpow * 0.12), volt);
    Right.spin(reverse, (rightpow * 0.12), volt);

    if (Controller.ButtonR1.pressing()) { 
      Intake.spin(fwd, 12, volt);
    } else if (Controller.ButtonR2.pressing()) {
      Intake.spin(reverse, 12, volt);
    } else {
      Intake.stop(brake);
    }



    if (Controller.ButtonL1.pressing()) { //score long
      Outtake.spin(fwd, 12, volt);
    } else if (Controller.ButtonL2.pressing()) {
      Outtake.spin(reverse, 12, volt);
    } else {
      Outtake.stop(brake);
    }

    if (Controller.ButtonX.pressing()) {
      Matchloader.set(true); //open
    } else if (Controller.ButtonA.pressing()) {
      Matchloader.set(false); //retract
    } else {
      //Do nothing
    }
             
    if (Controller.ButtonUp.pressing()) {
      Wing.set(true); //open
    } else if (Controller.ButtonLeft.pressing()) {
      Wing.set(false); //retract
    } else {
      //Do nothing
    }

    wait(20, msec);
  }
}

int main() {
  vexcodeInit();

  Competition.drivercontrol(usercontrol);
  Competition.autonomous(autonomous);

  pre_auton();

  while (true) {
    wait(100, msec);
  };
}
