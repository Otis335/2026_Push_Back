#include "vex.h"
#include "auton.h"
using namespace vex;

competition Competition;

//global constants
double kP = 0.1; 
double kI = 0;
double kD = 0;


void turn(int setpoint) {

    double error = setpoint - Inertial.rotation(degrees);
    double prevError = error;
    double integral = 0;
    double derivative = 0;

    while (fabs(error) > 1){
        double sensorValue = Inertial.rotation(degrees);
        error = setpoint - sensorValue;

    if (fabs(error) < 10) {  //integral windup protection
        integral = integral + error;
    } else {
        integral = 0;
    }

    if ((error > 0 && prevError < 0) || (error < 0 && prevError > 0)) {
        integral = 0;
    }

    derivative = error - prevError;
    prevError = error;

    double speed = error*kP + integral*kI + derivative*kD;

    Left.spin(forward, speed, volt);
    Right.spin(reverse, speed, volt);

    wait(15, msec); 
 }
    Left.stop(brake);
    Right.stop(brake);
}

void drivestraight(double targetInches, double targetAngle) { //targetAngle always 0 cuz drive straight
    Left.resetPosition();
    Right.resetPosition();

    //use inches? tile on the field is 24 x 24 inches
    double wheelDiameter = 3.25;
    double circum = 3.1415 * wheelDiameter;
    double degreesPerInch = 360 / circum;  

    double targetDegrees = targetInches * degreesPerInch;

    while (fabs(targetDegrees - Left.position(degrees)) > 5) {

        double currentDist = Left.position(degrees);
        double error = targetDegrees - currentDist;
        double speed = error * kP; 

        double currentAngle = Inertial.rotation(degrees);
        double driftError = targetAngle - currentAngle;
        double correction = driftError * kP; 

        Left.spin(forward, speed + correction, volt);
        Right.spin(forward, speed - correction, volt);

        wait(15, msec);
    }
    Left.stop(brake);
    Right.stop(brake);
}

//scoring and intake functions
void spinStorage(int timeMsec) {
    Intake.spin(forward, 12, volt);
    Outtake.spin(reverse, 8, volt);
    wait(timeMsec, msec);
    Intake.stop(brake);
    Outtake.stop(brake);
}

void scoreLong(int timeMsec) {
    ScoreLong.spin(forward, 12, volt);
    wait(timeMsec, msec);
    ScoreLong.stop(brake);
}

void scoreMiddle(int timeMsec) { //most likely not going to be used in auton
    Intake.spin(forward, 12, volt);
    Outtake.spin(reverse, 8, volt);
    Middle.set(false);
    wait(timeMsec, msec);
    Intake.stop(brake);
    Outtake.stop(brake);
    Middle.set(true);
}

int autonselection = 1;

void autonomous(void) {
 
}
