#include "secon_robot.h"
robot robo;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  robo.AFMS.begin();
}

void loop() {
  
  // put your main code here, to run repeatedly:
  
  robo.anglemove(45.0,100);
  delay(100);
}
