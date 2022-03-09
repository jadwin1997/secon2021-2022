#ifndef robot_lib
#define robot_lib

#include "Arduino.h"
#include <Wire.h>
#include <Stepper.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include "TFMini.h"
#include <SoftwareSerial.h>

//TFMini tfmini1;
//TFMini tfmini2;

#define SERVOMIN_BELT 100  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_BELT  500 // This is the 'maximum' pulse length count (out of 4096)
/*
const int stepsPerRevolution = 200;
Servo belt;
Servo shooter;
Servo arm;
Servo cam;

Stepper myStepper(stepsPerRevolution,3,4,5,6);
float angle2 = 0.0;
SoftwareSerial rearLidar(6,5);
SoftwareSerial portLidar(7,8);
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60); 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
*/
class robot
{
  public:
    robot();
    void anglemove(float angle, int velocity);
    //const int stepsPerRevolution = 200;
    Servo belt;
    Servo shooter;
    Servo arm;
    Servo cam;
    Adafruit_MotorShield AFMS;
    Adafruit_PWMServoDriver pwm;
    Adafruit_DCMotor *motor1;
    Adafruit_DCMotor *motor2;
    Adafruit_DCMotor *motor3;
    Adafruit_DCMotor *motor4;
    //Stepper myStepper(stepsPerRevolution,3,4,5,6);
    //float angle2 = 0.0;
    //SoftwareSerial rearLidar(6,5);
    //SoftwareSerial portLidar();
  
    
  private:
  float maptovel(int in,int velocity);

    
};

#endif
