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
// Back_Lift            motor         4               
// Ring                 motor         7               
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
    {   30,  30, 60, 60,  false, 0xE00000, 0x0000E0, "1" },
    {  150,  30, 60, 60,  false, 0x303030, 0xD0D0D0, "2" },
    {  270,  30, 60, 60,  false, 0x303030, 0xF700FF, "3" },
    {  390,  30, 60, 60,  false, 0x303030, 0xDDDD00, "4" },
    {   30, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "5" },
    {  150, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "6" },
    {  270, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "7" },
    {  390, 150, 60, 60,  false, 0x404040, 0xC0C0C0, "8" }
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
  Back_Lift.setVelocity(100, percent);
  Ring.setVelocity(100, percent);


  //setting lift motors so they hold when braking, this makes our lift not slide
  Front_Lift.setBrake(hold);
  Back_Lift.setBrake(hold);

  Front_Lift.setPosition(0, degrees);
  Back_Lift.setPosition(0, degrees);


  // All activities that occur before the competition starts
  // Example: clearing encoders, seting servo positions, ...
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

void autonomous(void) {
  // if the button in the first position (programmers start counting from 0)
  // then it executes the things in this if loop
  // if not, it moves on to the else loop
  if(buttons[0].state){
    //lower lift
    Back_Lift.spinTo(460, degrees, true);

    //drive forward 12 inches
    Drivetrain.driveFor(reverse, 16, inches);

    //raise front lift to golden angle
    Back_Lift.spinTo(143, degrees, true);

    //set the ring motor velocity to max
    Ring.setVelocity(100, percent);

    //start spinning the Ring motor
    Ring.spin(forward);
  }

  //right autonomous
  if(buttons[1].state){
    //lower lift
    Back_Lift.spinTo(460, degrees, true);

    //drive forward 12 inches
    Drivetrain.driveFor(reverse, 16, inches);

    //raise front lift to golden angle
    Back_Lift.spinTo(143, degrees, true);

    //set the ring motor velocity to max
    Ring.setVelocity(100, percent);

    //start spinning the Ring motor
    Ring.spin(forward);
    
    //turn 90 degrees left
    Drivetrain.turnFor(-90, degrees);

    //drive forward 6 inches
    Drivetrain.driveFor(reverse, 24, inches);

    //rotate 90 degrees right
    Drivetrain.turnFor(-88, degrees);

    Front_Lift.spinTo(460, degrees);

    //drive forward 12 inches
    Drivetrain.driveFor(forward, 36, inches);

    Front_Lift.spinTo(143, degrees);

    //drive reverse 36 inches (return to start)
    Drivetrain.driveFor(reverse, 48, inches);
  }

  // LEFT AUTON               LEFT AUTON               LEFT AUTON
  if(buttons[2].state){
    //lower lift
    Back_Lift.spinTo(480, degrees, true);

    //drive backwards 6 inches
    Drivetrain.driveFor(reverse, 14, inches);

    //raise front lift to golden angle
    Back_Lift.spinTo(143, degrees, true);

    //set the ring motor velocity to max
    Ring.setVelocity(100, percent);

    //start spinning the Ring motor
    Ring.spin(forward);

    //set the drive speed to 60% of its total
    Drivetrain.setDriveVelocity(60, percent);

    //drive forward 12 inches
    Drivetrain.driveFor(forward, 12, inches);

    //turn 90 degrees right
    Drivetrain.turnFor(90, degrees);

    //stop spinning the ring motor (so it doesn't hit the neutral goal)
    Ring.stop();

    //lower the front lift to grabbing position
    Front_Lift.spinTo(460, degrees);

    //drive forward 36 inches
    Drivetrain.driveFor(forward, 36, inches);

    //turn 20 degrees (to drive into the neutral goal)
    Drivetrain.turnFor(20, degrees);

    //drive 20 inches forward (into the neutral goal)
    Drivetrain.driveFor(forward, 20, inches);

    //spin front lift to golden angle
    Front_Lift.spinTo(143, degrees);

    //turn 20 degrees left (back to perpendicular with the home zone)
    Drivetrain.turnFor(-20, degrees); 

    //drive 38 inches backwards (into the home zone)
    Drivetrain.driveFor(reverse, 38, inches);

    //preping for driver control
    Drivetrain.setDriveVelocity(100, percent);
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
int main() {
  // Run the pre-autonomous function.
  pre_auton(); 

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
   
  Brain.Screen.pressed( userTouchCallbackPressed );
  Brain.Screen.released( userTouchCallbackReleased );

  // initial display
  displayButtonControls( 0, false );
  
  //creating a random number between 0 and 10 to use to display text
  //"for fun"
  int textInt = rand() % 10 ;
  // Prevent main from exiting with an infinite loop.
  while (true) {
    if( !Competition.isEnabled() )
            Brain.Screen.setFont(fontType::mono20);
    Brain.Screen.setFillColor( vex::color(0xFFFFFF) );

    Brain.Screen.setPenColor( vex::color(0xc11f27));




    if(textInt == 0){
      Brain.Screen.printAt( 0,  135, "  GO APES GO  " );
    }
    else if(textInt == 1){
      Brain.Screen.printAt( 0,  135, "  I'm KANZI   " );
    }
    else if(textInt == 2){
      Brain.Screen.printAt( 0,  135, "  MONKEEEEEEHHHH  " );
    }
    else if(textInt == 3){
      Brain.Screen.printAt( 0,  135, "  MONKEY MODE ACTIVATED   " );
    }
    else if(textInt == 4){
      Brain.Screen.printAt( 0,  135, "  APES TOGETHER STRONG  " );
    }
    else if(textInt == 5){
      Brain.Screen.printAt( 0,  135, "  ROBO BONOBOS  " );
    }
    else if(textInt == 6){
      Brain.Screen.printAt( 0,  135, "  8756A  " );
    }
    else if(textInt == 7){
      Brain.Screen.printAt( 0,  135, "  Banana powered  " );
    }
    else if(textInt == 8){
      Brain.Screen.printAt( 0,  135, "  f  " );
    }
    else if(textInt == 9){
      Brain.Screen.printAt( 0,  135, "  f  " );
    }
    else if(textInt == 10){
      Brain.Screen.printAt( 0,  135, "  f  " );
    }

    wait(100, msec);
  }
}

