#include "Arduino.h"
#include "secon_robot.h"
 SharpIR rearSharp = SharpIR(0,20150);
 SharpIR leftSharp = SharpIR(3,20150);
 Stepper myStepper(200,2,3,4,5); 
 //Stepper 
robot::robot(){
  
  myStepper.setSpeed(60);
  Zangle = 0.0;
  timeOld = micros();
  //Serial.println("moving...");
  AFMS = Adafruit_MotorShield(0x61);
  pwm = Adafruit_PWMServoDriver();
  
  //SoftwareSerial portLidar(8,7);
  motor1 = AFMS.getMotor(1);
  motor2 = AFMS.getMotor(2);
  motor3 = AFMS.getMotor(3);
  motor4 = AFMS.getMotor(4);
  p = 35.0;
  //portLidar.listen();
  
  /*
   * 
   // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(150);
  motor1->run(FORWARD);
  // turn 
  motor1->run(RELEASE);
  //myservo.attach(9);
  motor2->run(BACKWARD);
  motor1->run(FORWARD);
  motor3->run(FORWARD);
  motor4->run(BACKWARD);*/
  
}
void robot::calibrateSensors(){
  //SoftwareSerial portLidar(7,8);
  //portLidar.begin(TFMINI_BAUDRATE);
  //rearLidar.begin(TFMINI_BAUDRATE);
  //tfmini2.begin(&rearLidar);
  //tfmini1.begin(&portLidar);
  pinMode(13, INPUT_PULLUP);
  pwm.begin();
  // In theory the internal oscillator is 25MHz but it really isn't
  // that precise. You can 'calibrate' by tweaking this number till
  // you get the frequency you're expecting!
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
  
  mpu.initialize();
  
  anglemove(0,0);
  delay(1000);
  offset = getZaccelOffset();
  delay(1000);
  updateY();
  updateX();
}
void robot::updateGyro(){
  mpu.getRotation(&gx, &gy, &gz);
}
void robot::updateOffset(){
  anglemove(0,0);
  delay(1000);
  offset=getZaccelOffset();
  delay(500);
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
  
}
int robot::serialTalk(){
  Serial.begin(115200);
  if(Serial.available()>0){
    if(Serial.read() == 'x'){
     Serial_x = Serial.parseInt();
    }
  }
  else
  //Serial.print("Serial: ");
  //Serial.println(Serial_x);
  Serial.end();
  return Serial_x;
}

void robot::setCoordinates(int x, int y){
  target_x = x;
  target_y = y;
}


void robot::moveToCoordinates(int mode, int x_tol, int y_tol){
  Serial.flush();

  while(y_coordinate > target_y+y_tol || y_coordinate < target_y-y_tol ){
  updateAngle();
 
    if(y_coordinate > target_y){
      anglemove(180,maptovel((y_coordinate-target_y)*p,mode));
    }
    else if(y_coordinate<target_y){
      anglemove(0,maptovel((target_y-y_coordinate)*p,mode));
    }
   delay(10);
   updateY();
   
   Serial.print("Y: ");
   Serial.println(y_coordinate);  
  }
  

  while(x_coordinate > target_x+x_tol || x_coordinate < target_x-x_tol){
updateAngle();
    if(x_coordinate > target_x){
      
      
      anglemove(270.0,maptovel((x_coordinate-target_x)*p,mode));
     
    }
    else if(x_coordinate<target_x){
      anglemove(90.0,maptovel((target_x-x_coordinate)*p,mode));
    }
  delay(10);
  updateX();

   Serial.print("X: ");
   Serial.println(x_coordinate);
  }
  anglemove(180.,0);
 
}
void robot::anglemove(float angle, int velocity){
  timeNew = millis();
  int y_error  = y_coordinate-target_y;
  int x_error = x_coordinate-target_x;
  updateY();
  updateX();
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
  
  if(angle == 90){
    serialTalk();
  updateY();
  int pid = -(Serial_x - 45);
  /*if(pid_enable){
  int pid = map((y_error*lidar_angle_p)+((lidar_angle_d*(y_error - y_error_last))/int(timeNew-timeLast)),0,255,0,velocity);
  }
  else{
  int pid = -(Serial_x - 50);
  }*/
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity-pid));
  motor2->setSpeed(maptovel(255, velocity+pid));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity+pid));
  motor4->setSpeed(maptovel(255, velocity-pid));
  }
  else if(angle == 0){
  int pid = map((x_error*lidar_angle_p)+((lidar_angle_d*(x_error - x_error_last))/int(timeNew-timeLast)),0,255,0,velocity);
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity-pid));
  motor2->setSpeed(maptovel(255, velocity-pid));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity+pid));
  motor4->setSpeed(maptovel(255, velocity+pid));  
  }
  else{
  
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor2->setSpeed(maptovel(255, velocity));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor4->setSpeed(maptovel(255, velocity));
  }
  
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
  if(angle == 180){
  int pid = map((x_error*lidar_angle_p_2)+((lidar_angle_d_2*(x_error - x_error_last))/int(timeNew-timeLast)),0,255,0,velocity);
  motor1->setSpeed(maptovel(255, velocity-pid));
  motor2->setSpeed(maptovel(abs(int(vel-255)),  velocity-pid));
  motor3->setSpeed(maptovel(255,  velocity+pid));
  motor4->setSpeed(maptovel(abs(int(vel-255)), velocity+pid));}
  else{
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor2->setSpeed(maptovel(255, velocity));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor4->setSpeed(maptovel(255, velocity));   
  }
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

    if(angle == 270.0){
      serialTalk();
      
  updateY();
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity-(Serial_x - 50)));
  motor2->setSpeed(maptovel(255, velocity+(Serial_x - 50)));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity+(Serial_x - 50)));
  motor4->setSpeed(maptovel(255, velocity-(Serial_x - 50)));
  }
  else{
  motor1->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor2->setSpeed(maptovel(255, velocity));
  motor3->setSpeed(maptovel(abs(int(vel-255)), velocity));
  motor4->setSpeed(maptovel(255, velocity));
  }

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
    
  y_error_last = y_error;
  x_error_last = x_error;
  timeLast = timeNew;
}

int robot::maptovel(int in,int velocity){
  if(velocity > 255){
    velocity = 255;
  }
  if(velocity<0){
    velocity = 0;
  }
  if(in <0){
    in = 0;
  }
  if(in>255){
    in = 255;
  }
  return map(in, 0, 255, 0, velocity);
  
}
void robot::updateY(){
  //portLidar.listen
   
  y_coordinate = leftDistance();
  if(y_coordinate > 90){
    y_coordinate = 90;
  }
}
void robot::updateX(){

  x_coordinate = rearDistance();
    if(x_coordinate > 203){
    x_coordinate = 203;
  }
  
}
int robot::leftDistance(){
                                                             
  return leftSharp.distance();
}
int robot::rearDistance(){
  //return tfmini2.getDistance();
  return rearSharp.distance();
}

void robot::driveBelt(bool state){
  int pulselength = 0;
  if(state){
  pulselength = map(180, 0, 180, SERVOMIN_BELT, SERVOMAX_BELT);}
  else{
  pulselength = map(90, 0, 180, SERVOMIN_BELT, SERVOMAX_BELT);}
  pwm.setPWM(0,0,pulselength);
  //delay(msec);
  //belt.write(90);
}
void robot::load(){
    while(digitalRead(13)){
    digitalWrite(9,HIGH);
    digitalWrite(8,LOW);
    }
    digitalWrite(9,LOW);
}
void robot::Shoot(){
  while(digitalRead(13)){
    digitalWrite(9,HIGH);
    digitalWrite(8,LOW);
    }
    digitalWrite(9,LOW);
    delay(250);
    digitalWrite(9,HIGH);
    delay(100); 
    digitalWrite(9,LOW);   
}
void robot::lift(int dir){
  if(dir == 1){
   myStepper.step(200); 
  }
  else if(dir == -1){
   myStepper.step(-200); 
  }
  else{
   myStepper.step(0);
  }
  
}
void robot::moveCam(int camangle){
  int pulselength = map(camangle, 0, 180, 100, 500);
  pwm.setPWM(14,0,pulselength);
}

void robot::move_arm(int armangle){
  int pulselength = map(armangle, 0, 180, 100, 500);
  pwm.setPWM(15,0,pulselength);
  //delay(20);
  //}
}


