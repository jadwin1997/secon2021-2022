#include "Arduino.h"
#include "secon_robot.h"

robot::robot(){
  angle = 0.0;
  timeOld = micros();
  //Serial.println("moving...");
  AFMS = Adafruit_MotorShield();
  pwm = Adafruit_PWMServoDriver();
  //SoftwareSerial portLidar(8,7);
  motor1 = AFMS.getMotor(1);
  motor2 = AFMS.getMotor(2);
  motor3 = AFMS.getMotor(3);
  motor4 = AFMS.getMotor(4);
  p = 20;
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
  tfmini1.begin(&portLidar);
  mpu.initialize();
  offset = getZaccelOffset();
  updateY();
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
  for(int x = 0; x < 100; x++)
    avg= avg+getZaccel();
  return avg/100;
}
void robot::updateAngle(){
  angle = ((gz-offset) / 131) * ((float)(micros() - timeOld) / 1000000000)+angle;
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
void robot::moveToCoordinates(int mode){/*
  while(x_coordinate != target_x){
    
    if(x_coordinate > target_x){
      anglemove(270.0,50);
    }
    else if(x_coordinate<target_x){
      anglemove(90.0,50);
    }
  delay(100);
  }*/
  while(y_coordinate != target_y){
    if(y_coordinate > target_y){
      anglemove(180,(y_coordinate-target_y)*p);
    }
    else if(y_coordinate<target_y){
      anglemove(0,(target_y-y_coordinate)*p);
    }
   //delay(100);
   updateY();  
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
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity+int(this->angle)));
  motor2->setSpeed(maptovel(255, velocity-int(this->angle)));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity-int(this->angle)));
  motor4->setSpeed(maptovel(255, velocity+int(this->angle)));
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
  motor2->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor3->setSpeed(maptovel(255, velocity));
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

float robot::maptovel(int in,int velocity){
  if(velocity > 255){
    velocity = 255;
  }
  return map(in, 0, 255, 0, velocity);
  
}
void robot::updateY(){
  y_coordinate = leftDistance();
}
int robot::leftDistance(){
  
  return tfmini1.getDistance();
}

