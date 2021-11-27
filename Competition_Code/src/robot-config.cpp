#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftMotorA = motor(PORT20, ratio18_1, false);
motor leftMotorB = motor(PORT3, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT2, ratio18_1, true);
motor rightMotorB = motor(PORT10, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
inertial DrivetrainInertial = inertial(PORT8);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 319.19, 320, 40, mm, 1);
motor Front_Lift = motor(PORT9, ratio36_1, false);
motor Back_Lift = motor(PORT4, ratio36_1, false);
motor Ring = motor(PORT7, ratio18_1, true);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool Controller1UpDownButtonsControlMotorsStopped = true;
bool Controller1XBbuttonControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3
      // right = Axis2
      int drivetrainLeftSideSpeed = Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis2.position();
      
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
      // check the ButtonL1/ButtonL2 status to control Back_Lift
      if (Controller1.ButtonL1.pressing()&& !Controller1.ButtonR1.pressing()) {
        Back_Lift.spinTo(0, degrees, false);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL2.pressing()&& !Controller1.ButtonR2.pressing()) {
        Back_Lift.spinTo(-480, degrees, false);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        Back_Lift.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      // check the ButtonR1/ButtonR2 status to control Front_Lift
      if (Controller1.ButtonR1.pressing()&& !Controller1.ButtonL1.pressing()) {
        Front_Lift.spinTo(0,degrees, false);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing( )&& !Controller1.ButtonL2.pressing()) {
        Front_Lift.spinTo(480, degrees, false);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        Front_Lift.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }

      // check the ButtonR1&&ButtonL1 status to control intake
      if (Controller1.ButtonR1.pressing()&& Controller1.ButtonL1.pressing()){
        Ring.spin(forward);
      } else if (Controller1.ButtonR2.pressing() && Controller1.ButtonL2.pressing()){
        Ring.spin(reverse);
      } 
      
      //Stop the Ring Motor when the A button is pressed
      if (Controller1.ButtonA.pressing()){
        Ring.stop();
      }

      //Set front motor to -160 degrees when left is pressed
      if(Controller1.ButtonRight.pressing()){
        Front_Lift.spinTo(-145, degrees);
      } 

      //set Back Motor to -160 degrees when right is pressed
      if(Controller1.ButtonLeft.pressing()){
        Back_Lift.spinTo(-145, degrees);
      }

      if(Controller1.ButtonUp.pressing()){
        Front_Lift.spin(forward);
        Controller1UpDownButtonsControlMotorsStopped = false;
      } else if(Controller1.ButtonDown.pressing()){
        Front_Lift.spin(reverse);
        Controller1UpDownButtonsControlMotorsStopped = false;
      } else if (!Controller1UpDownButtonsControlMotorsStopped){
        Front_Lift.stop();
        Controller1UpDownButtonsControlMotorsStopped = true;
      }

      if(Controller1.ButtonX.pressing()){
        Back_Lift.spin(forward);
        Controller1XBbuttonControlMotorsStopped = false;
      } else if(Controller1.ButtonB.pressing()){
        Back_Lift.spin(reverse);
        Controller1XBbuttonControlMotorsStopped = false;
      } else if(!Controller1XBbuttonControlMotorsStopped){
        Back_Lift.stop();
        Controller1XBbuttonControlMotorsStopped = true;
      }

      //Macro to drop a goal on the back, reverse 12 inches, rotate 180 degrees
      //drive into the goal, lift to scoring angle
      /*
      if(Controller1.ButtonB.pressing()){
        Back_Lift.spinTo(-480, degrees, true);
        Drivetrain.driveFor(forward, 12, inches);
        Drivetrain.turnFor(180, degrees);
        Drivetrain.driveFor(forward, 12, inches);
        Front_Lift.spinTo(-155, degrees);
        Back_Lift.spinTo(0, degrees, false);
      }
      */


    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void){
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain Inertial
  wait(200, msec);
  DrivetrainInertial.calibrate();
  Brain.Screen.print("Calibrating Inertial for Drivetrain");
  // wait for the Inertial calibration process to finish
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}
