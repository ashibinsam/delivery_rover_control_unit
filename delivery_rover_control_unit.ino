#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define FULL_SPEED_DIST 130
#define TURN_DIST 80
#define EMERGENCY_DIST 50
#define SIDE_MIN_DIST 15

int ControlArr[5]={0,0,0,0,0};
int rFDist=0,lFDist=0,rDist=0,lDist=0,dLDist=0,dRDist=0;
unsigned long t1 = 0;
unsigned long t2 = 0;
float headingDegrees;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup() {
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
  pinMode(5,INPUT);
  pinMode(6,OUTPUT);
  pinMode(7,INPUT);
  pinMode(8,OUTPUT);
  pinMode(9,INPUT);

  pinMode(10,OUTPUT);
  pinMode(11,INPUT);
  pinMode(12,OUTPUT);
  pinMode(13,INPUT);

  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

}

void loop() {
  t1 = millis(); //The millis function returns the number of milliseconds since Arduino board has been powered up.
  //COMPASS CODE START
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = -1.58;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;
  //COMPASS CODE END
  // Serial.print(leftDis());
  // Serial.print("   ");
  // delay(50);
  // Serial.println(rightDis());
  // delay(50);
  ping();
  sendData(ControlArr);
  

  while(lFDist > FULL_SPEED_DIST && rFDist > FULL_SPEED_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST) {
    ControlArr[0]=1;
    ControlArr[1]=0;
    ControlArr[2]=255;
    ControlArr[3]=20;
    ping();
    sendData(ControlArr);
  }
  while(lFDist <= FULL_SPEED_DIST || rFDist <= FULL_SPEED_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST) {
    // rover reduced speed according to distance using an equation
    ControlArr[0]=1;
    ControlArr[1]=0;
    ControlArr[2]=min(lFDist,rFDist)+80;
    ControlArr[3]=20;
    ping();
    sendData(ControlArr);
  }
  while(lFDist <= TURN_DIST && rFDist <= TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
    // Turn rover according to the distance value using an equation
    ping();
    sendData(ControlArr);
  }

  while(lFDist <= TURN_DIST && rFDist > TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
    // Turn rover left slightly according to the distance value using an equation
    ping();
    sendData(ControlArr);
  }

  while(lFDist > TURN_DIST && rFDist <= TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
    // Turn rover right slightly according to the distance value using an equation
    ping();
    sendData(ControlArr);
  }

  while(rDist <= SIDE_MIN_DIST && lFDist > TURN_DIST && rFDist > TURN_DIST) {
    ping();
    //slightly turn left
    sendData(ControlArr);
    
  }
  while(lDist <= SIDE_MIN_DIST && lFDist > TURN_DIST && rFDist > TURN_DIST) {
    ping();
    //slightly turn right
    sendData(ControlArr);
  }

ping();
Serial.print(headingDegrees);
Serial.println("");

ping();
Serial.print(headingDegrees);
Serial.println("");
sendData(ControlArr);
}
