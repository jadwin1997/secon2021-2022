#include "Arduino.h"
#include "secon_robot.h"

robot::robot(){
  
  //Serial.println("moving...");
  AFMS = Adafruit_MotorShield();
  pwm = Adafruit_PWMServoDriver();
  

  motor1 = AFMS.getMotor(1);
  motor2 = AFMS.getMotor(2);
  motor3 = AFMS.getMotor(3);
  motor4 = AFMS.getMotor(4);
  
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
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor2->setSpeed(maptovel(255, velocity));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor4->setSpeed(maptovel(255, velocity));
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

  return map(in, 0, 255, 0, velocity);
  
}
