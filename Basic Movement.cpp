#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
controller Controller1 = controller(primary);
motor LeftDriveSmart = motor(PORT11, ratio18_1, false);
motor RightDriveSmart = motor(PORT12, ratio18_1, true);
inertial DrivetrainInertial = inertial(PORT15);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 319.19, 320, 40, mm, 0.7142857142857143);

digital_out Wings = digital_out(Brain.ThreeWirePort.A);
digital_out Intake_Piston = digital_out(Brain.ThreeWirePort.B);
motor Intake = motor(PORT13, ratio6_1, false);

motor Launch = motor(PORT14, ratio36_1, false);

/*vex-vision-config:begin*/
vision::signature Vision4__GREEN_TRIBALL = vision::signature (1, -4525, 1, -2262,-4157, 1, -2078,0.9, 0);
vision::signature Vision4__BLUE_TRIBALL = vision::signature (2, -2945, -169, -1557,1351, 10103, 5727,0.7, 0);
vision::signature Vision4__RED_TRIBAL = vision::signature (3, 3247, 9907, 6576,-841, 153, -344,1.1, 0);
vision Vision4 = vision (PORT4, 50, Vision4__GREEN_TRIBALL, Vision4__BLUE_TRIBALL, Vision4__RED_TRIBAL);
/*vex-vision-config:end*/
digital_out Barrier_piston = digital_out(Brain.ThreeWirePort.C);


// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}

bool vexcode_initial_drivetrain_calibration_completed = false;
void calibrateDrivetrain() {
  wait(200, msec);
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("Inertial");
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  vexcode_initial_drivetrain_calibration_completed = true;
  // Clears the screen and returns the cursor to row 1, column 1.
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
}

void vexcodeInit() {

  // Calibrate the Drivetrain
  calibrateDrivetrain();

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1RightShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // stop the motors if the brain is calibrating
      if (DrivetrainInertial.isCalibrating()) {
        LeftDriveSmart.stop();
        RightDriveSmart.stop();
        while (DrivetrainInertial.isCalibrating()) {
          wait(25, msec);
        }
      }
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonR1/ButtonR2 status to control Intake
      if (Controller1.ButtonR1.pressing()) {
        Intake.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        Intake.spin(reverse);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        Intake.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       {Aryan K}                                                 */
/*    Created:      {6/1/23}                                                  */
/*    Description:  Simple Movement Code for Armus Fortis                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

competition Competition = competition();

void setup()
{ 
  DrivetrainInertial.calibrate();

  Intake.setVelocity(100, percent);
  Intake.setStopping(coast);
  
  Launch.setMaxTorque(100, percent);
  Launch.setVelocity(100, percent);
  Launch.setStopping(coast);

  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setStopping(brake);
  Drivetrain.setTurnVelocity(100, percent);

  Intake_Piston.set(true);
}

void openIntake()
{
  Intake_Piston.set(true);
}

void closeIntake()
{
  Intake_Piston.set(false);
}

void onOpenWings()
{
  Wings.set(true);
}

void onCloseWings()
{
  Wings.set(false);
}

void windCatapult()
{
  Launch.spin(reverse);
}

void stopCatapult()
{
  Launch.stop();
}

void driverControl()
{
  Controller1.ButtonL1.pressed(onOpenWings);
  Controller1.ButtonL2.pressed(onCloseWings);

  Controller1.ButtonR1.pressed(openIntake);
  Controller1.ButtonR2.pressed(closeIntake);

  Controller1.ButtonX.pressed(windCatapult);
  Controller1.ButtonX.released(stopCatapult);
}

void autonomousPeriod()
{
  Vision4.takeSnapshot(Vision4__GREEN_TRIBALL);
  vex::timer Timer = vex::timer();
  Timer.clear();
  float currentTime = Timer.time(vex::timeUnits::sec);
  while(currentTime <= 15){
    if(Vision4.largestObject.exists){
      if(Vision4.largestObject.centerX > 60){
        Drivetrain.turn(right);
      }
      if(Vision4.largestObject.centerX < 60){
        Drivetrain.turn(left);
      }
      if(Vision4.largestObject.centerX > 60 && Vision4.largestObject.centerX < 60){
        if(Vision4.largestObject.width <200){
          Drivetrain.drive(forward);
          if(Vision4.largestObject.width <150){
            //turn on the claw
          }
        } else {
          //make robot score
        
        }
      }
    } else {
      // mke it turn until it detects its
      while(!Vision4.largestObject.exists){
        Drivetrain.turn(right);
      }
    }
  } 
}

int main()
{
  //Setup Variables Before Program Starts
  setup();

  Competition.drivercontrol(driverControl);
  Competition.autonomous(autonomousPeriod);
}