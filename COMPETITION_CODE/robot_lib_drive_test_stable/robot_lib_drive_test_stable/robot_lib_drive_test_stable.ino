#include "secon_robot.h"
robot robo;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  robo.AFMS.begin();
  robo.calibrateSensors();
  
  robo.setCoordinates(robo.rearDistance(),20);
  //robo.portLidar.begin(TFMINI_BAUDRATE);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  Serial.print("angle: ");
  Serial.print(robo.Zangle);
  Serial.print(" x: ");
  Serial.print(robo.x_coordinate);
  Serial.print(" y: ");
  Serial.println(robo.y_coordinate);
  robo.anglemove(90.0,100);
  /*
  //crobo.anglemove(90.0,0);
  robo.moveToCoordinates(1);
  robo.setCoordinates(40,15);
  robo.moveToCoordinates(1);
  robo.setCoordinates(10,15);
  robo.moveToCoordinates(1);
  delay(10000);
  //robo.anglemove(90.0,100);

  Serial.println(robo.tfmini2.getRecentSignalStrength());*/
  robo.updateGyro();
  robo.updateAngle();
  robo.updateY();
  robo.updateX();
}
