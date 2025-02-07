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
float maxspeed = 0.10;
float minspeed = -0.10;
float duration = 1.2;
char dir_mem;
int R_input;
int L_input;
float current_degree = 0;
const float robot_width = 0.111;
bool straighteningR = false;
bool straighteningL = false;
int total_revolutions = 0;

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
  
  sense();
  updateDegree();
  senseTurnedAround();
  t_prev = t;
}

//Omvandlar hastigheten till en signal för vänstermotor
int motorSpeed_L(float speed){
  //return 600*speed + 1500;
  return 144984707.8954 * pow(speed, 7) - 11484334.6039 * pow(speed, 6) 
        - 4555985.3420 * pow(speed, 5) + 361367.4301 * pow(speed, 4) 
        + 37675.3692 * pow(speed, 3) - 2819.0938 * pow(speed, 2) 
        + 438.4072 * speed + 1499.4989;
  //return (-8E6 * pow(speed, 6) + 2E6 * pow(speed, 5) + 208878 * pow(speed, 4) - 19645 * pow(speed, 3) - 500.59 * pow(speed, 2) + 385.07 * speed + 1500);
 }

//Omvandlar hastigheten till en signal för högermotor
int motorSpeed_R(float speed){
  //return -600*speed + 1500;
  return -119672220.8718 * pow(speed, 7) - 3794954.0015 * pow(speed, 6) 
          + 3788117.7739 * pow(speed, 5) + 141531.3239 * pow(speed, 4) 
          - 30733.7740 * pow(speed, 3) - 1110.6621 * pow(speed, 2) 
          - 484.0677 * speed + 1496.7976;
  //return (-2E7 * pow(speed, 6) + 39899 * pow(speed, 5) + 658712 * pow(speed, 4) - 26779 * pow(speed, 3) - 5803 * pow(speed, 2) - 150.4 * speed + 1500);

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
  accelerator(-0.03,minspeed,deltaT);
}
void reverseRight() {
  accelerator(minspeed,-0.02,deltaT);
}
void turnRight(){
  accelerator(0.02,maxspeed,deltaT);
}
void turnLeft(){
  accelerator(maxspeed,0.02,deltaT);
}
void reverse() {
  accelerator(minspeed, minspeed, deltaT);
}
void forward() {
  accelerator(maxspeed, maxspeed, deltaT);

}
void stop() {
  accelerator(0,0,deltaT);
}

void updateDegree(){
  float deltaV = v_currentL - v_currentR;
  float deltaS = deltaV * deltaT/1000000;

  current_degree += deltaS * 180/(11*M_PI);
  Serial.println(current_degree,3);
}

void senseTurnedAround(){
  if(current_degree > 90){
    //straighteningR = true;
  }
  else if(current_degree < -90){
    //straighteningL = true;
  }
  else if(abs(current_degree) <= 10){
    straighteningR = false;
    straighteningL = false;
  }
  else if(current_degree >= 360)
    current_degree -= 360;
  else if(current_degree <= -360)
    current_degree += 360;
}


bool isStopped(){
  if(!v_currentL && !v_currentR)
    return true;
  else
    return false;
}

void lockOn(char direction){
  lock = true;
  timer = 0;
  dir_mem = direction;
  avoid(direction);
}

void sense(){
  timer += deltaT;
  L_input = 1 - digitalRead(5);
  R_input = 1 - digitalRead(7);

  if(!lock){
    
    if(L_input == 1 && R_input == 1){
      lockOn('F');
    }
    else if(L_input == 1){
      lockOn('L');
    }

    else if(R_input == 1){
      lockOn('R');
    }
    //Om ingen sensor känner av någonting
    else{
      //Om BOE-boten har vänt håll så vänder den framåt igen.
      if(straighteningR){
        turnRight();
      }
      else if (straighteningL){
        turnLeft();
      }
      //Om ingen sensor känner av någonting och boten pekar åt rätt håll, kör fram.
      else{
        forward();
      }
    }

  }
  //Fortsätter att backa i samma riktning
  else{
    avoid(dir_mem);
  }
}

void avoid(char direction){
  if(direction == 'R'){
    if(timer < 0.5*million){
      if(L_input == 1 && timer < 0.1*million){
        direction = 'F';
        dir_mem = 'F';
      }
      reverse();
    }

    else if(timer < 1*million){
      reverseRight();
    }

    else{
      stop();
      if(isStopped()){
        lock = false;
      }
    }
  }
  else if(direction == 'L'){
    if(timer < 0.5*million){
      if(R_input == 1 && timer < 0.1*million){
        direction = 'F';
        dir_mem = 'F';
      }
      reverse();
    }

    else if(timer < 1*million){
      reverseLeft();
    }

    else{
      stop();
      if(isStopped()){
        lock = false;
      }
    }
  }
  //Om båda sensorer träffas
  else{
    if(timer < 2.5*million){
      reverse();
    }

    else if(timer < 3*million){
      reverseLeft();
    }
    else{
      stop();
      if(isStopped()){
        lock = false;
      }
    }
  }
}




