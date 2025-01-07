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
motor DriveMotorA = motor(PORT2, ratio18_1, false);
motor DriveMotorB = motor(PORT1, ratio18_1, true);
motor_group Drive = motor_group(DriveMotorA, DriveMotorB);

inertial Inertial5 = inertial(PORT5);



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



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       {author}                                                  */
/*    Created:      {date}                                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

int main() 
{
  Drive.setVelocity(20, percent);
  Inertial5.calibrate();
  Inertial5.setRotation(0, degrees);
  wait(3, seconds);
  for(int i = 0; i <= 2; i++)
  {
    int rot = Inertial5.orientation(yaw, degrees);
    while (rot < 85)
    {
      rot = Inertial5.orientation(yaw, degrees);
      DriveMotorA.spin(forward);
      DriveMotorB.spin(reverse);
    }
    Drive.stop();
    Inertial5.setRotation(0, degrees);
  }
}
