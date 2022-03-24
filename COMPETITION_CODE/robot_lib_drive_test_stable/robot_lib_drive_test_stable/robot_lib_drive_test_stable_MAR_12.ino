
#include "secon_robot.h"

robot robo;
int starting_y;
void setup() {
  //analogReference();
  Serial.begin(115200);
  // put your setup code here, to run once:
  robo.AFMS.begin();
  robo.calibrateSensors();
  starting_y = robo.y_coordinate;
  robo.setCoordinates(21,22);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  //robo.portLidar.begin(TFMINI_BAUDRATE);
}

void loop() {
  robo.Shoot(LOW);
  // put your main code here, to run repeatedly:
  Serial.print("angle: ");
  Serial.print(robo.Zangle);
  Serial.print(" x: ");
  Serial.print(robo.x_coordinate);
  Serial.print(" y: ");
  Serial.println(robo.y_coordinate);

  //robo.anglemove(90.0,0);
  
  
  robo.driveBelt(HIGH);
  delay(1000);
  robo.driveBelt(LOW);
  robo.moveToCoordinates(255);
  robo.setCoordinates(70,23);
  robo.moveToCoordinates(100);
  delay(1000);
  robo.setCoordinates(203, 23);
  robo.moveToCoordinates(230);
  robo.Shoot(HIGH);
  delay(1800);
  robo.Shoot(LOW);
  digitalWrite(10,LOW);
  robo.setCoordinates(21,23);
  robo.moveToCoordinates(230);
  delay(1000);
  robo.setCoordinates(21,23);
  robo.moveToCoordinates(200);
  delay(1000);
  robo.setCoordinates(21,starting_y);
  robo.moveToCoordinates(255);
  delay(2000);
  robo.setCoordinates(21,23);



  
  //robo.anglemove(90.0,0);
  
  /*
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
  delay(50);
}
