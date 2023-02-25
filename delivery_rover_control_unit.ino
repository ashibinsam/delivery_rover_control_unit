#define FULL_SPEED_DIST 130
#define TURN_DIST 80
#define EMERGENCY_DIST 50
#define SIDE_MIN_DIST 20


int rFDist=0,lFDist=0,rDist=0,lDist=0;
unsigned long t1 = 0;
unsigned long t2 = 0;



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
}

void loop() {
  t1 = millis(); //The millis function returns the number of milliseconds since Arduino board has been powered up.
  // Serial.print(leftDis());
  // Serial.print("   ");
  // delay(50);
  // Serial.println(rightDis());
  // delay(50);
  ping();

  while(lFDist > FULL_SPEED_DIST && rFDist > FULL_SPEED_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST) {
    // rover straight full speed
    ping();
  }
  while(lFDist <= FULL_SPEED_DIST && rFDist <= FULL_SPEED_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST) {
    // rover reduced speed according to distance using an equation
    ping();
  }
  while(lFDist <= TURN_DIST && rFDist <= TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
    // Turn rover according to the distance value using an equation
    ping();
  }

  while(lFDist <= TURN_DIST && rFDist > TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
    // Turn rover left slightly according to the distance value using an equation
    ping();
  }

  while(lFDist > TURN_DIST && rFDist <= TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
    // Turn rover right slightly according to the distance value using an equation
    ping();
  }

  while(rDist <= SIDE_MIN_DIST) {
    //slightly turn left
  }
  while(lDist <= SIDE_MIN_DIST) {
    //slightly turn right
  }


}
