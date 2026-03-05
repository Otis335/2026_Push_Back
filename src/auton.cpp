#include "vex.h"
#include "auton.h"
using namespace vex;


//turn constants //negative is right, positive is left
double turnkP = 0.083; 
double turnkI = 0;
double turnkD = 0.03;

//drive constants
double drivekP = 5;
double drivekD = 0;
double driftkP = 0; 

double wrapAngle(double angle) 
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
    double elapsed = 0;

    while (fabs(error) > 5 && elapsed < 3000){ //more than 1 degree away from target, keep running code
        error = wrapAngle(setpoint - Inertial.rotation(degrees));

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

    Left.spin(reverse, speed, volt);
    Right.spin(forward, speed, volt);

    elapsed += 15;
    wait(15, msec); 
 }
    Left.stop(brake);
    Right.stop(brake);
}

void drive(double distance, double speed) {

    DriveTrain.setDriveVelocity(speed, percent);
    
    if (distance > 0) {
        DriveTrain.driveFor(forward, distance, distanceUnits::in, true);
    } else {
        DriveTrain.driveFor(reverse, fabs(distance), distanceUnits::in, true);
    }

    if (distance < 10 && distance > -10) { //if we're trying to move a short distance, be more precise
        while (DriveTrain.isMoving()) {
            wait(20, msec);
        }
    } else { //if we're moving a long distance, stop as soon as we reach the target
        while (DriveTrain.isMoving()) {
            wait(20, msec);
        }
    }
    DriveTrain.stop(hold);
}


void move(double targetInches, double targetAngle) {
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


int autonselection = 2;

void autonomous(void) {
    //tuning
if (autonselection == 1) { //left side auto
   drive(32,40); //match
   wait(300, msec);
   turn(-90);
   //Matchloader.set(false);
   //wait(300, msec);
   //drive(12,40);
    //Intake.spin(forward, 12, volt);
    //wait(5000, msec);
    //Intake.stop(brake);
   drive(-24,30); //-30
   wait(300, msec);
   scoreLong(5000);
}

if (autonselection == 2) { //right side auton
   drive(32,40); //match
   turn(90);
   drive(-24,30); //-30
   //wait(300, msec);
   Matchloader.set(true);
   wait(300, msec);
   drive(34,25);
        Intake.spin(forward, 12, volt);
        wait(500, msec);
        Intake.stop(brake);
    drive(-34,40);
   scoreLong(3000);
}

if (autonselection == 3) { //alliance awp
    drive(5,100);
}


if (autonselection == 4) { //clear and park
    Intake.spin(reverse, 12, volt);
    drive(24,100);
    wait(2000,msec);
    Intake.stop(brake);
}

   

    else if (autonselection == 7) {
    //start halfway in the parking zone facing the wall
    Intake.spin(forward, 12, volt);
    drive (12, 100);
    Intake.stop(brake);
    drive(-24, 100);
    turn(-90);
    drive(-12, 100);
    turn(90);
    scoreLong(3000); 
    Matchloader.set(true);
    wait(300, msec);
    Intake.spin(forward,12, volt);
    drive(24, 100);
    Intake.stop(brake);
    drive(-24,100);
    Matchloader.set(false);
    wait(300, msec);
    turn(90);
    drive(-8,100);
    turn(90);
    drive(-75,100);
    turn(-90);
    drive(-8,100);
    turn(90);
    drive(24,100);
    scoreLong(5000);

 }
}
