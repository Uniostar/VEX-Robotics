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
/*vex-vision-config:begin*/
vision::signature Vision20__RED_CAP = vision::signature (1, 7835, 11491, 9662,-2127, -727, -1426,3, 0);
vision::signature Vision20__SIG_2 = vision::signature (2, 0, 0, 0,0, 0, 0,3, 0);
vision::signature Vision20__SIG_3 = vision::signature (3, 0, 0, 0,0, 0, 0,3, 0);
vision::signature Vision20__SIG_4 = vision::signature (4, 0, 0, 0,0, 0, 0,3, 0);
vision::signature Vision20__SIG_5 = vision::signature (5, 0, 0, 0,0, 0, 0,3, 0);
vision::signature Vision20__SIG_6 = vision::signature (6, 0, 0, 0,0, 0, 0,3, 0);
vision::signature Vision20__SIG_7 = vision::signature (7, 0, 0, 0,0, 0, 0,3, 0);
vision Vision20 = vision (PORT20, 150, Vision20__RED_CAP, Vision20__SIG_2, Vision20__SIG_3, Vision20__SIG_4, Vision20__SIG_5, Vision20__SIG_6, Vision20__SIG_7);
/*vex-vision-config:end*/
motor DriveMotorA = motor(PORT2, ratio18_1, false);
motor DriveMotorB = motor(PORT1, ratio18_1, true);
motor_group Drive = motor_group(DriveMotorA, DriveMotorB);



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

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

int main() 
{
  while (true)
  {
    Vision20.takeSnapshot(Vision20__RED_CAP);
    Brain.Screen.setCursor(10, 40);
    Brain.Screen.print(Vision20.objectCount);
    for(int i = 0; i < Vision20.objectCount; i++) 
    {
      Brain.Screen.setCursor(5, 5);
      Brain.Screen.print("X: %d", Vision20.objects[i].centerX);
      Brain.Screen.setCursor(10, 5);
      Brain.Screen.print("Y: %d", Vision20.objects[i].centerY);
    }
    if ((Vision20.objects[0].centerX) > 170)
    {
      DriveMotorB.spin(forward);
      DriveMotorA.spin(reverse);
    }
    else if ((Vision20.objects[0].centerX) < 155)
    {
      DriveMotorB.spin(reverse);
      DriveMotorA.spin(forward);
    }
    else 
    {
      Drive.stop();
    }
  }
}