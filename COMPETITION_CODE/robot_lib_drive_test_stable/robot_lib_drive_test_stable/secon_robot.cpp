#include "Arduino.h"
#include "secon_robot.h"

robot::robot(){
  Zangle = 0.0;
  timeOld = micros();
  //Serial.println("moving...");
  AFMS = Adafruit_MotorShield();
  pwm = Adafruit_PWMServoDriver();
  //SoftwareSerial portLidar(8,7);
  motor1 = AFMS.getMotor(1);
  motor2 = AFMS.getMotor(2);
  motor3 = AFMS.getMotor(3);
  motor4 = AFMS.getMotor(4);
  p = 30;
  //portLidar.listen();
  
  /*
   // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(150);
  motor1->run(FORWARD);
  // turn on motor
  motor1->run(RELEASE);
  //myservo.attach(9);
  motor2->run(BACKWARD);
  motor1->run(FORWARD);
  motor3->run(FORWARD);
  motor4->run(BACKWARD);*/
  
}
void robot::calibrateSensors(){
  //SoftwareSerial portLidar(7,8);
  portLidar.begin(TFMINI_BAUDRATE);
  rearLidar.begin(TFMINI_BAUDRATE);
  tfmini2.begin(&rearLidar);
  tfmini1.begin(&portLidar);
  mpu.initialize();
  offset = getZaccelOffset();
  updateY();
  updateX();
}
void robot::updateGyro(){
  mpu.getRotation(&gx, &gy, &gz);
}

int robot::getZaccel(){
  updateGyro();
  if(gz==0){
    return 0;
  }
  else{
   return gz; 
  }
  
}

int robot::getZaccelOffset(){
  int avg = 0;
  for(int x = 0; x < 1000; x++)
    avg= avg+getZaccel();
  return avg/1000;
}
void robot::updateAngle(){
  Zangle = ((gz-offset) / 131) * ((float)(micros() - timeOld) / 1000000)+Zangle;
  timeOld = micros();
  //angle = angle + (millis()-timeOld)*getZaccel();
}
int robot::serialTalk(){
 int x = 0; 
  if(Serial.available()>0){
    if(Serial.read() == "X"){
     x = Serial.parseInt();
    }
  }
  return x;
}

void robot::setCoordinates(int x, int y){
  target_x = x;
  target_y = y;
}


void robot::moveToCoordinates(int mode){
 portLidar.listen();
 updateGyro();
 updateAngle();
  while(y_coordinate != target_y){
   updateGyro();
 updateAngle();
    if(y_coordinate > target_y){
      anglemove(180,maptovel((y_coordinate-target_y)*p,mode));
    }
    else if(y_coordinate<target_y){
      anglemove(0,maptovel((target_y-y_coordinate)*p,mode));
    }
   //delay(100);
   updateY();
   
   Serial.print("Angle: ");
   Serial.println(Zangle);  
  }
  
  rearLidar.listen();
  while(x_coordinate != target_x){
     updateGyro();
 updateAngle();
    
    if(x_coordinate > target_x){
      
      
      anglemove(270.0,maptovel((x_coordinate-target_x)*p,mode));
     
    }
    else if(x_coordinate<target_x){
      anglemove(90.0,maptovel((target_x-x_coordinate)*p,mode));
    }
  //delay(100);
  updateX();
  updateAngle();
   Serial.print("X: ");
   Serial.println(x_coordinate);
  }
  
 
}
void robot::anglemove(float angle, int velocity){
  if(angle>=360){
    angle = angle - 360.0;
  }
  if(angle <= 90){

    float vel = (510.0/90.0)*angle;
    motor1->run(FORWARD);
    motor3->run(FORWARD);
  if(vel < 255){
    motor1->run(BACKWARD);
    motor3->run(BACKWARD);
  }
  motor2->run(FORWARD);
  motor4->run(FORWARD);
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity+int(Zangle)));
  motor2->setSpeed(maptovel(255, velocity-int(Zangle)));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity-int(Zangle)));
  motor4->setSpeed(maptovel(255, velocity+int(Zangle)));
  }
  ///NEED TO COMPLETE FOR OTHER QUADRANTS
  else if(angle <=180)
  {

    float vel = (510.0/90.0)*(angle-90.0);
    
  motor1->run(FORWARD);
  motor2->run(FORWARD);
  motor3->run(FORWARD);
  motor4->run(FORWARD);
    if(vel >= 255){
    motor2->run(BACKWARD);
    motor4->run(BACKWARD);
  }
  motor1->setSpeed(maptovel(255, velocity));
  motor2->setSpeed(maptovel(abs(int(vel-255)),  velocity));
  motor3->setSpeed(maptovel(255,  velocity));
  motor4->setSpeed(maptovel(abs(int(vel-255)), velocity));
    }
  else if(angle <= 270)
  {
  float vel = (510.0/90.0)*(angle-180.0);
  motor1->run(FORWARD);
  motor2->run(BACKWARD);
  motor3->run(FORWARD);
  motor4->run(BACKWARD);
  if(vel>=255){
    motor1->run(BACKWARD);
    motor3->run(BACKWARD);
    }
    motor1->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor2->setSpeed(maptovel(255, velocity));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor4->setSpeed(maptovel(255, velocity));
    }
  else if(angle <= 360.0){
    float vel = (510.0/90.0)*(angle-270.0);
    
    motor2->run(FORWARD);
    motor4->run(FORWARD);
  if(vel < 255){
    
    motor2->run(BACKWARD);
    motor4->run(BACKWARD);
  }
  motor1->run(BACKWARD);
  motor3->run(BACKWARD);
  motor1->setSpeed(maptovel(255, velocity));
  motor2->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor3->setSpeed(maptovel(255, velocity));
  motor4->setSpeed(maptovel(abs(int(vel-255)), velocity));
  }
    
  
}

int robot::maptovel(int in,int velocity){
  if(velocity > 255){
    velocity = 255;
  }
  if(velocity<0){
    velocity = 0;
  }
  return map(in, 0, 255, 0, velocity);
  
}
void robot::updateY(){
  portLidar.listen();
  y_coordinate = leftDistance();
}
void robot::updateX(){
  rearLidar.listen();
  x_coordinate = rearDistance();
}
int robot::leftDistance(){
  
  return tfmini1.getDistance();
}
int robot::rearDistance(){
  return tfmini2.getDistance();
}

