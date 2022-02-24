/*    UNUSED BUTTONS
X - Controller1.ButtonX
B - Controller1.ButtonB
A - Controller1.ButtonZ


*/

#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors

// Drive motors
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
motor leftMotorA = motor(PORT20, ratio18_1, false);
motor leftMotorB = motor(PORT3, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT2, ratio18_1, true);
motor rightMotorB = motor(PORT10, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
inertial DrivetrainInertial = inertial(PORT8);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 319.19, 320, 40, mm, 1);
motor Bar = motor(PORT4, ratio36_1, false);

// Old architecture
motor Front_Lift = motor(PORT9, ratio36_1, false);
motor Ring = motor(PORT7, ratio18_1, true);


// 4bar motors

motor Claw = motor(PORT6, ratio36_1, false);

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

//Y button variable and function
int YINT = 0;
void YPRESS (){
  YINT=1;


}

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
      
      if(YINT==1){
        drivetrainLeftSideSpeed = drivetrainLeftSideSpeed /2;
        drivetrainRightSideSpeed = drivetrainRightSideSpeed /2;
        
        leftMotorA.setBrake(hold);
        leftMotorB.setBrake(hold);
        rightMotorA.setBrake(hold);
        rightMotorB.setBrake(hold);
      } else {
        drivetrainLeftSideSpeed = Controller1.Axis3.position();
        drivetrainRightSideSpeed = Controller1.Axis2.position();
        
        leftMotorA.setBrake(coast);
        leftMotorB.setBrake(coast);
        rightMotorA.setBrake(coast);
        rightMotorB.setBrake(coast);

      }

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
      if ((Controller1.ButtonL1.pressing()&& !Controller1.ButtonR1.pressing())||(Controller2.ButtonL1.pressing()&&!Controller2.ButtonR1.pressing()) ){
        Bar.spinTo(-900,degrees, false);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if ((Controller1.ButtonL2.pressing()&& !Controller1.ButtonR2.pressing())||Controller2.ButtonL2.pressing()) {
        Bar.spinTo(0, degrees, false);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        Bar.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      // check the ButtonR1/ButtonR2 status to control Front_Lift
      if ((Controller1.ButtonR1.pressing()&& !Controller1.ButtonL1.pressing())||(Controller2.ButtonR1.pressing()& !Controller2.ButtonL1.pressing())) {
        Front_Lift.spinTo(0,degrees, false);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if ((Controller1.ButtonR2.pressing()&& !Controller1.ButtonL2.pressing())||Controller2.ButtonR2.pressing()) {
        Front_Lift.spinTo(480, degrees, false);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        Front_Lift.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }

      //If up button is pressed, raise 4 bar lift
      //If down button is pressed, lower 4 bar lift
      if(Controller1.ButtonUp.pressing()||Controller2.ButtonUp.pressing()){
        Claw.spinTo(-380, degrees, false);
        Controller1UpDownButtonsControlMotorsStopped = false;
      } else if((Controller1.ButtonDown.pressing()||Controller2.ButtonDown.pressing())){
        Claw.spinTo(0, degrees, false);
        Controller1UpDownButtonsControlMotorsStopped = false;
      } else if (!Controller1UpDownButtonsControlMotorsStopped){
        Claw.stop(); 
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1UpDownButtonsControlMotorsStopped = true;
      }

      // check the ButtonR1&&ButtonL1 status to control intake
      if ((Controller1.ButtonR1.pressing()&& Controller1.ButtonL1.pressing())||(Controller2.ButtonR1.pressing()&&Controller2.ButtonL1.pressing())){
        Ring.spin(reverse);
      } else if ((Controller1.ButtonR2.pressing() && Controller1.ButtonL2.pressing())||(Controller2.ButtonR2.pressing()&&Controller2.ButtonL2.pressing())){
        Ring.spin(forward);
      } 
      
      //Stop the Ring Motor when the A button is pressed
      if ((Controller1.ButtonA.pressing())||Controller2.ButtonA.pressing()){
        Ring.stop();
      }

      //Set front motor to 138 degrees when left is pressed
      if(Controller1.ButtonRight.pressing()){
        Front_Lift.spinTo(138, degrees);
      } 

      Controller1.ButtonY.pressed(YPRESS);
      Controller1.ButtonX.pressed()
      

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
