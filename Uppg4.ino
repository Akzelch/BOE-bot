#include <Servo.h>
#include <math.h>

Servo motorL;
Servo motorR;
int ServopinL = 11;
int ServopinR = 10;
float v_currentL = 0;
float v_currentR = 0;
float v_targetL = 0;
float v_targetR = 0;
int t_prev = 0;
const long million = 1000000;
unsigned long t=0;
bool lock = false;
unsigned long timer = 0;
int deltaT = 0;
float maxspeed = 0.17;
float minspeed = -0.17;
void setup() {
  
  motorL.attach(ServopinL);
  motorR.attach(ServopinR);
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(5, INPUT);
  Serial.println("Setup Successful");
  

}

void loop() {
  t = micros();
  deltaT = t - t_prev; 



  t_prev = t;
}

unsigned long pingTime(int pingPin) {
    // Set the pin to OUTPUT mode to send the trigger pulse
    pinMode(pingPin, OUTPUT);
    
    // Start with a LOW signal for 2 microseconds
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    
    // Send a HIGH signal for 5 microseconds to trigger the ultrasonic sensor
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    
    // Set the signal back to LOW
    digitalWrite(pingPin, LOW);
    
    // Switch the pin to INPUT mode to listen for the echo
    pinMode(pingPin, INPUT);
    
    // Measure the duration of the HIGH pulse (time taken for the echo to return)
    unsigned long duration = pulseIn(pingPin, HIGH);
    
    // Return the measured duration
    return duration;
}

float distance(unsigned long pingTime)

//Omvandlar hastigheten till en signal för vänstermotor
int motorSpeed_L(float speed){
  return 144984707.8954 * pow(speed, 7) - 11484334.6039 * pow(speed, 6) 
        - 4555985.3420 * pow(speed, 5) + 361367.4301 * pow(speed, 4) 
        + 37675.3692 * pow(speed, 3) - 2819.0938 * pow(speed, 2) 
        + 438.4072 * speed + 1499.4989;
 }

//Omvandlar hastigheten till en signal för högermotor
int motorSpeed_R(float speed){
  return -119672220.8718 * pow(speed, 7) - 3794954.0015 * pow(speed, 6) 
          + 3788117.7739 * pow(speed, 5) + 141531.3239 * pow(speed, 4) 
          - 30733.7740 * pow(speed, 3) - 1110.6621 * pow(speed, 2) 
          - 484.0677 * speed + 1496.7976;

}

//Skickar rätt signal enligt önskad hastighet till varje motor
void motorWriter(float speedR, float speedL){
  motorL.writeMicroseconds(motorSpeed_L(speedL));
  motorR.writeMicroseconds(motorSpeed_R(speedR));
  v_currentL = speedL;
  v_currentR = speedR; 
 }

//Denna gör en mjuk acceleration/retardation till önskad hastighet
void accelerator(float targetSpeedL, float targetSpeedR, float looptime, bool reversebraking = false){
  float a_max;
  float a_min;
  
  if(reversebraking){
    a_max = 1;
    a_min = -0.2;    
  }
  else{
    a_max = 0.2;
    a_min = -1;
  }
  
  looptime = looptime/1000000;                                        //Omvandlar loop-tiden till sekunder
  float a_targetL = ((targetSpeedL-v_currentL)/looptime);             //delta V delat på looptiden ger planerad acceleration
  float a_targetR = ((targetSpeedR-v_currentR)/looptime);
  
  float accelerationL = constrain(a_targetL, a_min, a_max);           //Ger tillåten accelation för nuvarande loop-instans
  float accelerationR = constrain(a_targetR, a_min, a_max);
  float v_allowedL = v_currentL + accelerationL * looptime;           //Ger tillåten hastighet för nuvarande loop-instans
  float v_allowedR = v_currentR + accelerationR * looptime;

  motorWriter(v_allowedR, v_allowedL);                                //skickar tillåten/önskad hastighet till vänster- och högermotor

}

void reverseLeft() {
  accelerator(0,minspeed,deltaT,true);
}
void reverseRight() {
  accelerator(minspeed,0,deltaT,true);
}
void turnRight(){
  accelerator(0,maxspeed,deltaT);
}
void turnLeft(){
  accelerator(maxspeed,0,deltaT);
}
void reverse() {
  accelerator(minspeed, minspeed, deltaT);
}
void forward() {
  if v_currentL < 
  accelerator(maxspeed, maxspeed, deltaT);

}

void reverseStop(){
  accelerator(0,0,deltaT,true);
}

void stop() {
  accelerator(0,0,deltaT);
}


bool isStopped(){
  if(!v_currentL && !v_currentR)
    return true;
  else
    return false;
}





