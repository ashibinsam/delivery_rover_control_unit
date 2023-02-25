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


}
