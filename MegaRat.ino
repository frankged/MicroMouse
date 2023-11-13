//Le MegaRat
#include <AFMotor.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define motorPortBack 2
#define motorPortFront 3

AF_DCMotor backMotor(motorPortBack);
AF_DCMotor frontMotor(motorPortFront);


int DefaultSpeed = 100; //adjust(rubber bands over the washers allow for lower speeds)
int IRdata       = 0;
int ThisPath     = 0;

// PID constants(need to calibrate by guessing)
double Kp = 1.0;  // Proportional constant
double Ki = 0.0;  // Integral constant
double Kd = 0.0;  // Derivative constant

// Variables for PID
double setAngle       = 0.0;
double previousError  = 0.0;
double integral       = 0.0;
double error          = 0.0;
double derivative     = 0.0;
double currentAngle   = 0.0;
double PIDoutput      = 0.0;
double leftMotorSpeed = 0.0;
double rightMotorSpeed= 0.0;

void setup() {
  Serial.begin(9600);
  backMotor.setSpeed(DefaultSpeed);
  frontMotor.setSpeed(DefaultSpeed);
  srand(time(NULL));  //this should ideally go in main but this is good enough

}

void loop() {
/* 
use other arduino board for sensor inputs, and have the sensor data be sent to an analog pin
will need 5 digital pins(4 IR, 1 gryo) and 2 analog pin(gyro) on the sensor arduino
the gyro will spit data in degrees from 0 to 360, so we can just send the data to A5 on the main board
for the 4 digital inputs(IR sensors), just send the data to A4 in blocks of the 0-1023 analog range
*/

 // Replace the following line with actual sensor reading from the gyro
  //data collection block
  currentAngle = analogRead(A5);  // Assuming A5 is connected to the gyro
  IRdata = analogRead(A4);

  //IR logic(forward sensors are naturally low)
  switch(IRdata) { 

    case 1: // turn left case or forward(left sensor is low)
    ThisPath = RNG(2);
    if(ThisPath == 1){ //when turning we would want to use the gyro sensor to track exactly how much we turned
      //go left    
      Left();     
    }
    break;

    case 2: // turn right case or forward(right sensor is low)
    ThisPath = RNG(2);
    if(ThisPath == 1){
      //go right
      Right();
    }
    break;

    case 3: // turn either left or right or forward(right and left are low)
    ThisPath =RNG(3);
    if(ThisPath == 1){
      //go left
      Left();
    }    
    else if(ThisPath == 2){
      //go right
      Right();
    }
    break;

    case 4: // left or right(right and left are low, but forward are high)
    ThisPath = RNG(2);
    if(ThisPath == 1){
      //go left
      Left();
    }    
    else if(ThisPath == 2){
      //go right
      Right();
    }
    break;

    case 5: // all sensors are high(reached a deadend, do a 180)
    //gyro the problem away
    FullTurn();
    break;

    case 6: //forward sensors are high, but only left sensors are low
    Left();
    break;

    case 7: //forward sensors are high, but only right sensors are low
    Right();
    break;

  } 


  //PID Block *****************
  error = setAngle - currentAngle;  //when the rover turns we will have to rotate setpoint by i
  integral += error;
  derivative = error - previousError;

  PIDoutput = (Kp * error) + (Ki * integral) + (Kd * derivative); // Calculate PID output
  
  leftMotorSpeed = DefaultSpeed + PIDoutput;
  rightMotorSpeed = DefaultSpeed - PIDoutput;   // Adjust motor speeds based on PID output
  
  frontMotor.setSpeed(constrain(rightMotorSpeed, 0, 255));
  backMotor.setSpeed(constrain(leftMotorSpeed, 0, 255)); // Set motor speeds

  previousError = error;      // Store the current error for the next iteration
   
}


void Forward(){
  // Déjà vu
  frontMotor.run(FORWARD);
  backMotor.run(FORWARD);
}
void Reverse(){  //will hopefully never have to use
  //-(Déjà vu)
  frontMotor.run(BACKWARD);
  backMotor.run(BACKWARD);
}
void Left(){
  // i(Déjà vu)
  while(currentAngle < setAngle + 90){
    frontMotor.run(FORWARD);
    backMotor.run(BACKWARD); 
    currentAngle = analogRead(A5);  
      
  }
  setAngle =+ 90;
  
}
void Right(){ 
  // -i(Déjà vu)
  while(currentAngle > setAngle - 90){
    frontMotor.run(BACKWARD);
    backMotor.run(FORWARD);
    currentAngle = analogRead(A5);  

  }
  setAngle =- 90;

}
void FullTurn(){

  while(currentAngle > setAngle - 180){
    frontMotor.run(BACKWARD);
    backMotor.run(FORWARD);
    currentAngle = analogRead(A5);  

  }
  setAngle =- 180;
}
void STOP(){  
  //no Déjà vu 
  frontMotor.run(RELEASE);
  backMotor.run(RELEASE);
  delay(200); 
}

int RNG(int paths){
  // Seed the random number generator with the current time
    int random_number_range = rand() % (paths) + 1;
    return 0;
}


