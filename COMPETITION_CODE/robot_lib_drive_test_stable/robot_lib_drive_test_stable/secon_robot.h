



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
#include <MPU6050.h>
#include <SharpIR.h>
//TFMini tfmini1;
//TFMini tfmini2;

#define SERVOMIN_BELT 100  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_BELT  500 // This is the 'maximum' pulse length count (out of 4096)

class robot
{
  
  public:
    robot();
    void anglemove(float angle, int velocity);
    void calibrateSensors();
    int leftDistance();
    int rearDistance();
    void updateGyro();
    void updateAngle();
    int serialTalk();
    int getZaccel();
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    
    unsigned long timeOld;
    void setCoordinates(int x, int y);
    void moveToCoordinates(int mode, int x_tol = 2, int y_tol = 2);
    void updateY();
    void updateX();
    void updateOffset();
    int x_coordinate = 0;
    int y_coordinate = 0;
    //int target_x;
    //int target_y;
    float Zangle;
    //const int stepsPerRevolution = 200;
    MPU6050 mpu;
    TFMini tfmini1;
    int p;
    TFMini tfmini2;
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
    SoftwareSerial portLidar{7, 8};
    SoftwareSerial rearLidar{10, 9};
    int getZaccelOffset();
    int16_t offset;
    //Stepper myStepper(stepsPerRevolution,3,4,5,6);
    //float angle2 = 0.0;
    
    
  
    
  private:
  int maptovel(int in,int velocity);
  int target_x;
  int target_y;

    
};

#endif
