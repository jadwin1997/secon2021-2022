
#include "secon_robot.h"

robot robo;
int starting_y;
void setup() {
  
  //analogReference();
  Serial.begin(115200);
  Serial.setTimeout(2000);
  // put your setup code here, to run once:
  robo.AFMS.begin();
  robo.calibrateSensors();
  starting_y = robo.y_coordinate-2;
  robo.setCoordinates(18,30);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  pinMode(13,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  //robo.portLidar.begin(TFMINI_BAUDRATE);
}

void loop() {

    while(robo.Serial_x==-1000){
    robo.driveBelt(LOW);
    robo.serialTalk();
  }
  /*
  //digitalRead(13)&&
  while(LOW&&robo.Serial_x==-1000){
    robo.driveBelt(LOW);
    robo.serialTalk();
  }
  digitalWrite(9,HIGH);
  digitalWrite(8,LOW);
  robo.load();
  delay(1000);

  robo.moveCam(10);
  delay(1000);
  robo.moveCam(90);
  delay(1000);
  robo.moveCam(170);
  delay(1000);
  robo.moveCam(13);
  robo.moveCam(170);
  
  //while(digitalRead(13)){
  //this how u lift 
  robo.lift(-1);
  delay(1000);
  robo.lift(0);
  //}
  //while(1==1){
  
  //delay(100);
  //}
  //delay(1000);
  robo.lift(0);
  robo.lift(0);
  //robo.Shoot();
  //robo.move_arm(180);
  delay(1000);
  while(1){
    //robo.load();
    delay(1000);
    robo.load();
    //delay(1000);
    //robo.load();
  }
  while(1==1){
    //robo.lift(1);
    //delay(1000);
    //robo.lift(0);
  //robo.Shoot();
  // put your main code here, to run repeatedly:
 /* Serial.print("angle: ");
  Serial.print(robo.Zangle);
  Serial.print(" x: ");
  Serial.print(robo.x_coordinate);
  Serial.print(" y: ");
  Serial.println(robo.y_coordinate);*/

  //robo.anglemove(90.0,0);
  
  //Serial.flush();
  /*
  robo.driveBelt(HIGH);
  delay(1000);
  robo.driveBelt(LOW);
  delay(1000);
  */
  robo.moveToCoordinates(220);


  delay(1000);
  for(int x = 0; x < 80; x++){
    robo.move_arm(x);
    delay(40);
  }
  robo.anglemove(270.0,50);
  delay(500);
  robo.anglemove(270.0,0);
  robo.driveBelt(HIGH);
  for(int x = 80; x <130; x++){
    robo.move_arm(x);
    delay(40);
  }
  robo.driveBelt(LOW);
    robo.setCoordinates(21,23);
  robo.moveToCoordinates(200);
  robo.setCoordinates(70,23);
  robo.moveToCoordinates(200);
  
  delay(1000);
  //robo.pid_enable=LOW;
  robo.setCoordinates(21,23);
  robo.moveToCoordinates(200);
  robo.setCoordinates(21,30);
  //robo.pid_enable = HIGH ;
  //robo.updateGyro();
  //robo.updateAngle();
  robo.updateY();
  robo.updateX();
  
  //delay(0);
}
