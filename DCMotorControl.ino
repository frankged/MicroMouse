#include <AFMotor.h>

#define motorPort 4

AF_DCMotor mymotor(motorPort);

void setup() {
  // put your setup code here, to run once:
  mymotor.setSpeed(200);
}

# define STALLSPEED 50

int i = STALLSPEED;
int speed = 0;
bool decreasing = false;
bool increasing = true;

void loop() {
  // put your main code here, to run repeatedly:
while (increasing){ // ramp up
    if (i == 200) {
      i = 0;
      decreasing = true;
      increasing = false;
      break;
    }
    speed = i;
    delay(5000/(200 - STALLSPEED)); // setting time of ramp to 5 seconds
    mymotor.run(FORWARD);
    i++;
}
while (decreasing){ // ramp down
    if ( i == (200 - STALLSPEED) ) {
      i = STALLSPEED;
      decreasing = false;
      increasing = true;
      break;
    }
    
    speed = 200 - i;
    delay(5000/(200 - STALLSPEED)); // setting time of ramp to 5 seconds
    mymotor.run(FORWARD);
    i++;
}
}
