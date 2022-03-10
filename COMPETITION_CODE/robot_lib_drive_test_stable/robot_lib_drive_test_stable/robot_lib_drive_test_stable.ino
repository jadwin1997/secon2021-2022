#include "secon_robot.h"
robot robo;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  robo.AFMS.begin();
  robo.calibrateSensors();
  robo.setCoordinates(100,100);
  //robo.portLidar.begin(TFMINI_BAUDRATE);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  Serial.print("angle: ");
  Serial.print(robo.angle);
  Serial.print(" y: ");
  Serial.println(robo.y_coordinate);
  
  
  robo.moveToCoordinates(1);
  //robo.anglemove(90.0,100);
  robo.updateGyro();
  robo.updateAngle();
  robo.updateY();
}
