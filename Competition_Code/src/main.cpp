/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    20, 3, 2, 10, 8 
// Front_Lift           motor         9                           
// Ring                 motor         7  
// Bar                  motor         4    
// Claw                 motor         15  

// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
// storage for our auton selection
int   autonomousSelection = -1;

/* 
        James Pearman autoselect functions and definitions. These are modified for Walsh
*/
// collect data for on screen button and include off and on color feedback for button
// prc - instead of radio approach with one button on or off at a time, each button has
//          a state.  ie shootPreload may be low yellow and high yellow when on.
typedef struct _button {
    int    xpos;
    int    ypos;
    int    width;
    int    height;
    bool   state;
    vex::color offColor;
    vex::color onColor;
    const char *label;
} button;

// Button array definitions for each software button. The purpose of each button data structure
// is defined above.  The array size can be extended, so you can have as many buttons as you 
// wish as long as it fits.
button buttons[] = {
    {   30,  30, 60, 60,  false, 0x404040, 0xC0C0C0, "RC alt" },
    {  150,  30, 60, 60,  false, 0x404040, 0xC0C0C0, "RC " },
    {  270,  30, 60, 60,  false, 0x404040, 0xC0C0C0, "RC R " },
    {  390,  30, 60, 60,  false, 0x404040, 0xC0C0C0, "WP" },
    {   30, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "LC alt" },
    {  150, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "LC" },
    {  270, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "LC R" },
    {  390, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "" }
};

// forward ref
void displayButtonControls( int index, bool pressed );

/*-----------------------------------------------------------------------------*/
/** @brief      Check if touch is inside button                                */
/*-----------------------------------------------------------------------------*/
int
findButton(  int16_t xpos, int16_t ypos ) {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      button *pButton = &buttons[ index ];
      if( xpos < pButton->xpos || xpos > (pButton->xpos + pButton->width) )
        continue;

      if( ypos < pButton->ypos || ypos > (pButton->ypos + pButton->height) )
        continue;

      return(index);
    }
    return (-1);
}

/*-----------------------------------------------------------------------------*/
/** @brief      Init button states                                             */
/*-----------------------------------------------------------------------------*/
void
initButtons() {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      buttons[index].state = false;
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been touched                                        */
/*-----------------------------------------------------------------------------*/
void
userTouchCallbackPressed() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      displayButtonControls( index, true );
    }

}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been (un)touched                                    */
/*-----------------------------------------------------------------------------*/
void
userTouchCallbackReleased() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      // clear all buttons to false, ie. unselected
      //      initButtons(); 

      // now set this one as true
      if( buttons[index].state == true) {
      buttons[index].state = false; }
      else    {
      buttons[index].state = true;}

      // save as auton selection
      autonomousSelection = index;

      displayButtonControls( index, false );
    }
}


/*-----------------------------------------------------------------------------*/
/** @brief      Draw all buttons                                               */
/*-----------------------------------------------------------------------------*/
void
displayButtonControls( int index, bool pressed ) {
    vex::color c;
    Brain.Screen.setPenColor( vex::color(0xe0e0e0) );

    for(int i=0;i<sizeof(buttons)/sizeof(button);i++) {

      if( buttons[i].state )
        c = buttons[i].onColor;
      else
        c = buttons[i].offColor;

      Brain.Screen.setFillColor( c );

      // button fill
      if( i == index && pressed == true ) {
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, c );
      }
      else
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height );

      // outline
      Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, vex::color::transparent );

// draw label
      if(  buttons[i].label != NULL )
        Brain.Screen.printAt( buttons[i].xpos + 8, buttons[i].ypos + buttons[i].height - 8, buttons[i].label );
    }
}


// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  //makin sure that the lift motors are moving at 100% speed during auton
  Front_Lift.setVelocity(100, percent);
  Ring.setVelocity(100, percent);
  Bar.setVelocity(100, percent);
  Claw.setVelocity(100, percent);


  //setting lift motors so they hold when braking, this makes our lift not slide
  Front_Lift.setBrake(hold);
  Bar.setBrake(hold);
  Claw.setBrake(hold);

  Front_Lift.setPosition(0, degrees);
  Bar.setPosition(0, degrees);

  //setting max torque for all motors so there is no need for precise positioning when lowering in auton
  
  //TUNE WHEN AT MAC
  Front_Lift.setMaxTorque(100, percent);
  Claw.setMaxTorque(95, percent);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

/*
AUTON NOTES

We start 9 inches from the wall

speed is a large consideration

make fast

For multiple auton routes in the same button, add mass comment brackets and a visual separator


AUTON STATUS

1(button[0]): test R alt, basic is final
2(button[1]): tune speed to get middle goal
3(button[2]): test/tune L Claw
4(button[3]): test/tune Win Point, find alliance placement spots
5(button[4]): tune claw close angle, other parts should work if above work
6(button[5]): ^
7(button[6]): ^
8(button[7]): test route, make option that lifts onto platforms if worth/possible
*/

void autonomous(void) {
  // if the button in the first position (programmers start counting from 0 for some reason)
  // then it executes the things in this if loop
  // if not, it moves on to the else loop
  if(buttons[0].state){
    //Raise Claw to open
    Claw.spinTo(-310, degrees, false);
    //increase turn speed... because speed
    Drivetrain.setTurnVelocity(20, percent);
    //set drive velocity to 100%
    Drivetrain.setDriveVelocity(100, percent);
    //
    Drivetrain.driveFor(reverse, 56, inches);
    //
    Drivetrain.setDriveVelocity(50, percent);
    //
    Drivetrain.driveFor(reverse, 4, inches);
    //
    Drivetrain.setDriveVelocity(100, percent);
    //
    Claw.spinTo(-200, degrees, true);
    //
    Front_Lift.spinTo(480, degrees, false);
    //
    Drivetrain.turnFor(-60, degrees);
    //
    Drivetrain.driveFor(forward, 12, inches);
    //
    Front_Lift.spinTo(138, degrees, true);
    //
    Drivetrain.turnFor(90, degrees);
    //
    Drivetrain.driveFor(forward, 60, inches);
  }

  //RIGHT CLAW AUTON               RIGHT CLAW AUTON               RIGHT CLAW AUTON
  if(buttons[1].state){
    //raise claw to open position
    Claw.spinTo(-380, degrees, false);
    //increase turn speed... because speed
    Drivetrain.setTurnVelocity(20, percent);
    //setting the drive speed to the max
    Drivetrain.setDriveVelocity(100, percent);
    //drive backwards 40 inches
    Drivetrain.driveFor(reverse, 40, inches);
    //Lower the drive velocity to 20% for stutter
    Drivetrain.setDriveVelocity(20, percent);
    //drive 3 inches to fully reach goal
    Drivetrain.driveFor(reverse, 3, inches);
    //
    Claw.spinTo(-160, degrees, true);
    //raise drive velocity to 80%
    Drivetrain.setDriveVelocity(100, percent);
    //lower front lift
    Front_Lift.spinTo(480, degrees, false);
    //drive forward to alliance goal
    Drivetrain.driveFor(forward, 28, inches);
    //turn to face alliance goal
    Drivetrain.turnFor(-85, degrees);
    //lower drive velocity for consistency
    Drivetrain.setDriveVelocity(100, percent);
    //grab alliance goal
    Drivetrain.driveFor(forward, 14, inches);
    //spin front lift to golden angle
    Front_Lift.spinTo(160, degrees, true);
    //start scoring rings
    Ring.spin(reverse);
    //pause to let rings score
    wait(1000, msec);
    //stop scoring rings
    Ring.stop();
    //Max out drive velocity to stay in time
    Drivetrain.setDriveVelocity(100, percent);
    //Reverse a small amount to give alliance goal space
    Drivetrain.driveFor(reverse, 20, inches);
    //
    Claw.spinTo(-380, degrees, true);
    //
    Drivetrain.turnFor(45, degrees);
    //
    Drivetrain.driveFor(reverse,30, inches);
    //Lower drive speed for stutter
    Drivetrain.setDriveVelocity(20, percent);
    //Stutter
    Drivetrain.driveFor(reverse,6, inches);
    //Raise front lift to grab tall goal
    Claw.spinTo(-160, degrees);
    //Max out drive velocity to stay in time
    Drivetrain.setDriveVelocity(100, percent);
    //Drive into home zone
    Drivetrain.driveFor(forward, 38, inches);
    //lower drive velocity
    Drivetrain.setTurnVelocity(50, percent);

    //Turn inside home zone to make sure we scored both neutrals
    //this is not strictly necessary, as technically both neutral goals are scored when we finish driving
    //but it makes the scoring a bit more consistent and puts us in a good spot for begining elimination
    /*
    Since I have a bit of spare time I'm going to psuedocode the auton out in english
    1. Reverse at max speed to the right neutral goal
    2. While you are doing that, lower the back lift
    3. Stop driving
    4. Lower drive speed
    5. Drive forward a small bit
    6. Raise back lift
    7. Max out drive speed
    8. Drive forward to the alliance goal
    9. While you are doing that, lower front lift
    10. Turn to face alliance goal
    11. Drive at alliance goal
    12. Stop driving
    13. Lower drive speed
    14. Drive forward into alliance goal
    15. Raise front lift into scoring position
    16. Spin ring motor
    17. Wait a bit
    18. Stop spinning ring motor
    19. Max out drive speed
    20. Reverse a bit
    21. Start lowering front lift
    22. Reverse to be in line with tall neutral goal
    23. Turn to face tall goal
    24. Drive forward towards tall goal
    25. Stop driving
    26. Lower drive speed
    27. Drive forward the rest of the way into the tall goal
    28. Raise front lift to grab goal (not as high as other times)
    29. Max out drive speed
    30. Drive forward into home zone
    31. Turn to make sure both goals are scored
    */
  }
  // Right Rings                      Right Rings           Right Rings
  if(buttons[2].state){
    
    
  }
  //    
  if(buttons[3].state){
    //Increase turn velocity because speed
    Drivetrain.setTurnVelocity(20, percent);
    //Lower Front Lift 
    Front_Lift.spinTo(480, degrees);
    //Max out drive speed
    Drivetrain.setDriveVelocity(100, percent);
    //Drive forward mostly into goal
    Drivetrain.driveFor(forward, 12, inches);
    //lower drive speed for stutter
    Drivetrain.setDriveVelocity(20, percent);
    //stutter
    Drivetrain.driveFor(forward, 4, inches);
    //raise front lift
    Front_Lift.spinTo(160, degrees);
    //begin scoring rings
    Ring.spin(reverse);
    //wait for a ring to be scored
    wait(400, msec);
    //stio scoring rings
    Ring.stop();
    //lower front lift
    Front_Lift.spinTo(480, degrees, false);
    //reverse slowly to give time for goal to lower
    Drivetrain.driveFor(reverse, 8, inches);
    //max out drive velocity
    Drivetrain.setDriveVelocity(100, percent);
    //drive the rest of the way
    Drivetrain.driveFor(reverse, 8, inches);
    //raise front lift
    Front_Lift.spinTo(160, degrees, false);
    //turn 90 degrees left
    Drivetrain.turnFor(-90, degrees);
    //max out drive velocity
    Drivetrain.setDriveVelocity(100, percent);
    //drive forward 26 inches
    Drivetrain.driveFor(forward, 26, inches);
    //turn 90 degrees right
    Drivetrain.turnFor(90, degrees);
    //lower front lift
    Front_Lift.spinTo(480, degrees, false);
    //drive forwards 92 inches (to alliance goal)
    Drivetrain.driveFor(forward, 92, inches);
    //lower drive velocity for stutter
    Drivetrain.setDriveVelocity(20, percent);
    //stutter
    Drivetrain.driveFor(forward, 14, inches);
    //max out drive velocity
    Drivetrain.setDriveVelocity(100, percent);
    //raise front lift to scoring position
    Front_Lift.spinTo(160, degrees);
    //score rings
    Ring.spin(reverse);
    //wait half a second
    wait(1000, msec);
    //stop scoring rings
    Ring.stop();

  }

  //Left Claw Alt          Left Claw Alt          Left Claw Alt
  if(buttons[4].state){
    
  }
  //Left Claw          Left Claw          Left Claw
  if(buttons[5].state){
    //
    Claw.spinTo(-380, degrees, false);
    //
    Drivetrain.setDriveVelocity(100, percent);
    //
    Drivetrain.driveFor(reverse, 40, inches);
    //
    Drivetrain.setDriveVelocity(20, percent);
    //
    Drivetrain.driveFor(reverse, 4, inches);
    //
    Drivetrain.setDriveVelocity(100, percent);
    //
    Claw.spinTo(-160, degrees);
    //
    Bar.spinTo(-80, degrees, true);
    //
    Drivetrain.driveFor(forward, 40, inches);
    //
    Drivetrain.turnFor(-45, degrees);
    //
    Claw.spinTo(-380, degrees);
    //
    Bar.spinTo(0, degrees, false);
    //
    Drivetrain.turnFor(79, degrees);
    //
    Drivetrain.driveFor(reverse, 50, inches);
    //
    Drivetrain.setDriveVelocity(20, percent);
    //
    Drivetrain.driveFor(reverse, 4, inches);
    //
    Drivetrain.setDriveVelocity(100, percent);
    //
    Claw.spinTo(-160, degrees);
    //
    Front_Lift.spinTo(480, degrees, false);
    //
    Drivetrain.driveFor(forward, 48, inches);
    
  }
  //
  if(buttons[6].state){
    
  }
  //WIN POINT         WIN POINT          WIN POINT
  if(buttons[7].state){
    //
    Claw.spinTo(-380, degrees, false);
    //
    Front_Lift.spinTo(480, degrees);
    //
    Drivetrain.driveFor(forward, 12, inches);
    //
    Front_Lift.spinTo(140, degrees);
    //
    Drivetrain.driveFor(forward, 36, inches);
    //
    Drivetrain.turnFor(90, degrees);
    //
    Drivetrain.driveFor(reverse, 24, inches);
    //
    Claw.spinTo(-160, degrees);
    //
    Drivetrain.turnFor(90, degrees);
    //
    Drivetrain.driveFor(reverse, 48, inches);
    //
    Drivetrain.turnFor(90, degrees);
    //
    Front_Lift.spinTo(480, degrees, false);
    //
    Drivetrain.driveFor(reverse, 24, inches);
    //
    Drivetrain.turnFor(-90, degrees);
    //
    Drivetrain.driveFor(reverse, 24, inches);
    //
    Drivetrain.turnFor(90, degrees);
    //
    Drivetrain.driveFor(forward, 24, inches);
    //
    Front_Lift.spinTo(140, degrees);
    


  }
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

/*
Button Controlls 

Up - Raise 4 bar
Down - Lower 4 bar
Left - Compress piston
Right - Front lift to golden angle

X - 
B - 
Y - Brake
A - 

*/


void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main(){
  // Run the pre-autonomous function.
  pre_auton(); 

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
   
  Brain.Screen.pressed( userTouchCallbackPressed );
  Brain.Screen.released( userTouchCallbackReleased );

  // initial display
  displayButtonControls( 0, false );
  
  // Prevent main from exiting with an infinite loop.
  while (true) {
    if( !Competition.isEnabled() )
            Brain.Screen.setFont(fontType::mono20);
    Brain.Screen.setFillColor( vex::color(0xFFFFFF) );

    Brain.Screen.setPenColor( vex::color(0xc11f27));

    Brain.Screen.printAt( 0,  135, "  MONKEY MODE ACTIVATED   " );
    
    wait(100, msec);
  }
}

