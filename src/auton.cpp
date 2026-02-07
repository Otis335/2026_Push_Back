#include "vex.h"
#include "auton.h"
using namespace vex;


//turn constants
double turnkP = 0.1; 
double turnkI = 0;
double turnkD = 0;

//drive constants
double drivekP = 0;
double drivekD = 0;
double driftkP = 0; 

double wrapAngle(double angle) //so inertial deoesn't spin infinitely
{
    while (angle > 180) {
        angle -= 360;
    }
    while (angle < -180) {
        angle += 360;
    }
    return angle;
}

void turn(int setpoint) {

    double error = wrapAngle(setpoint - Inertial.rotation(degrees));
    double prevError = error;
    double integral = 0;
    double derivative = 0;

    while (fabs(error) > 1){ //more than 1 degree away from target, keep running code
        double error = wrapAngle(setpoint - Inertial.rotation(degrees));

    if (fabs(error) < 10) {  //integral windup protection, don't start counting integral until within 10 degrees
        integral = integral + error;
    } else {
        integral = 0;
    }

    if ((error > 0 && prevError < 0) || (error < 0 && prevError > 0)) { //if we cross the target, reset integral
        integral = 0;
    }

    derivative = error - prevError; //change in error, so we can see how fast we're approaching target
    prevError = error;

    double speed = error*turnkP + integral*turnkI + derivative*turnkD; 

    Left.spin(forward, speed, volt);
    Right.spin(reverse, speed, volt);

    wait(15, msec); 
 }
    Left.stop(brake);
    Right.stop(brake);
}

void drive(double targetInches, double targetAngle) {
    Left.resetPosition();
    Right.resetPosition();

    double wheelDiameter = 3.25;  //calculating movement based off size of wheels
    double circum = 3.1415 * wheelDiameter;
    double degreesPerInch = 360 / circum; 
    double targetDegrees = targetInches * degreesPerInch;  

    double prevError = targetDegrees;
    double distError = targetDegrees; 
    double derivative = 0;

    // As long as the absolute error is > 5, keep moving
    while (fabs(distError) > 5) {
        double currentDist = Left.position(degrees);
        distError = targetDegrees - currentDist; 

        derivative = distError - prevError;
        prevError = distError;
        
        double speed = distError * drivekP + derivative * drivekD; 

        double currentAngle = wrapAngle(Inertial.rotation(degrees)); 
        double driftError = targetAngle - currentAngle;
        double correction = driftError * driftkP;

        Left.spin(forward, speed - correction, volt);
        Right.spin(forward, speed + correction, volt);

        wait(15, msec);
    }
    Left.stop(brake);
    Right.stop(brake);
}


//Intake
void spinStorage(int timeMsec) {
    Intake.spin(forward, 12, volt);
    wait(timeMsec, msec);
    Intake.stop(brake);
}

void scoreLong(int timeMsec) {
    ScoreLong.spin(forward, 12, volt);
    wait(timeMsec, msec);
    ScoreLong.stop(brake);
}


int autonselection = 0;

void autonomous(void) {
    //tuning
    if (autonselection == 0) {
    turn(90);
    }

    else if (autonselection == 1) {
    //start halfway in the parking zone facing the wall
    drive(12, 0);
    spinStorage(1000); //make function run at same time as above
    drive(-24, 0);
    scoreLong(2000); //outtaking extra blocks
    turn(-90);
    drive(24, 0);
    turn(-90);
    Matchloader.set(true);
    wait(300, msec);
    drive(36, 0);
    spinStorage(4000);
    //above is clear matchloader
    drive(-24, 0);
    Matchloader.set(false);
    wait(300, msec);
    turn(-90);
    drive(-8,0);
    turn(90);
    drive(-72,0);
    turn(90);
    drive(-8,0);
    turn(90);
    drive(-24,0);
    scoreLong(6000);
    drive(24,0);
    spinStorage(5000); //clear second matchload
    drive(-24,0);
    scoreLong(6000);
    drive(2, 0);
    Matchloader.set(false);//retract piston
    wait(300, msec);
    turn(90);
    drive(96,0);
    turn(-90);
    Matchloader.set(true);
    wait(300, msec);
    drive(24,0);
    spinStorage(5000);//clear third matchload
    drive(-20,0);
    Matchloader.set(false);
    wait(300, msec);
    turn(-90);
    drive(-8,0);


 }
}
