#include <AFMotor.h>

#define motorPortBack 2
#define motorPortFront 1

AF_DCMotor backMotor(motorPortBack);
AF_DCMotor frontMotor(motorPortFront);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  backMotor.setSpeed(100);
  frontMotor.setSpeed(100);
}

# define STALLSPEED 50

int i = STALLSPEED;
int speed = 0;
bool decreasing = false;
bool increasing = true;

char a = 0;
unsigned long time = 0;
unsigned long clock = millis();
void loop() { 
//   // put your main code here, to run repeatedly:
// while (increasing){ // ramp up
//     if (i == 200) {
//       i = 0;
//       decreasing = true;
//       increasing = false;
//       break;
//     }
//     speed = i;
//     delay(5000/(200 - STALLSPEED)); // setting time of ramp to 5 seconds
//     mymotor.run(FORWARD);
//     i++;
// }
// while (decreasing){ // ramp down
//     if ( i == (200 - STALLSPEED) ) {
//       i = STALLSPEED;
//       decreasing = false;
//       increasing = true;
//       break;
//     }
    
//     speed = 200 - i;
//     delay(5000/(200 - STALLSPEED)); // setting time of ramp to 5 seconds
//     mymotor.run(FORWARD);
//     i++;
// }

  if(Serial.available()) {
    a  = Serial.read();
    //time = millis();
    // TODO: check to see if motor ordering matters(it definitely will)
  }
  /*
  if (time + 10000 < clock) {
    delay(1000);
  }
  */
  else if (a == 'w'){
      // go forward
      rightMotorForward();
      leftMotorForward();
    }
    else if (a == 'a') {
      // spin left
      rightMotorForward();
      leftMotorBackward();
    }  
    else if (a == 'd') {
      // spin right
      rightMotorBackward();
      leftMotorForward();
    }
    else if (a == 's') {
      // go backwards
      rightMotorBackward();
      leftMotorBackward();
    }
     else if (a == 'r') {
      //trigger on sensor input(IR)
      //stops the motors
      stopMotors();
    }
}

void rightMotorForward(){
  // right motor goes forward
  frontMotor.run(FORWARD);
}
void rightMotorBackward(){
  //you know what to do
  frontMotor.run(BACKWARD);
}
void leftMotorForward(){
  backMotor.run(FORWARD);
}
void leftMotorBackward(){
  backMotor.run(BACKWARD);
}
void stopMotors() {
  frontMotor.run(RELEASE);
  backMotor.run(RELEASE);
  delay(3000); 
}
