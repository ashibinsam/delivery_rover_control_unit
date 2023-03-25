const int CDArrSize = 5;

int readDis(int trigPin, int echoPin)
{ 
  //  pinMode(trigPin,OUTPUT);
  //  pinMode(echoPin,INPUT);
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
}
int fRightDis(){
  return readDis(4, 5)+2;
}
int fLeftDis() {
  return readDis(2,3);
}
int leftDis() {
  return readDis(6,7);
}
int rightDis() {
  return readDis(8,9);
}
int dLeftDis() {
  return readDis(10,11);
}
int dRightDis() {
  return readDis(12,13);
}

void printDist() {
    Serial.print(" | ");
    Serial.print(rFDist);
    Serial.print("   ");
    Serial.print(lFDist);
    Serial.print("   ");
    Serial.print(rDist);
    Serial.print("   ");
    Serial.print(lDist);
    Serial.print("   ");
    Serial.print(dRDist);
    Serial.print("   ");
    Serial.print(dLDist);
    Serial.print("   ");
    Serial.print(" | ");
}
void ping() {
  
  t1 = millis();
  
  if (abs(t2 - t1)> 10) {
    //Serial.println("hello");
    rFDist=fRightDis();
    // Serial.print(rFDist);
    // Serial.print("   ");
  }
  if (abs(t2 - t1)> 20) {
    lDist=leftDis();
    // Serial.print(lDist);
    // Serial.print("   ");
  }
  if (abs(t2 - t1)> 30) {
    lFDist=fLeftDis();
    // Serial.print(lFDist);
    // Serial.print("   ");
  }
  if (abs(t2 - t1)> 40) {
    rDist=rightDis();
    // Serial.print(rDist);
    // Serial.print("   ");
    
  }
    if (abs(t2 - t1)> 50) {
    dLDist=dLeftDis();
    // Serial.print(dLDist);
    // Serial.print("   ");
    
  }
    if (abs(t2 - t1)> 60) {
    dRDist=dRightDis();
    // Serial.print(dRDist);
    // Serial.print("   ");
    t2=millis();
    // printDist();
  }
  
  if(rFDist<= CLEARANCE_DIST || lFDist<= CLEARANCE_DIST || lDist<= CLEARANCE_DIST || rDist<= CLEARANCE_DIST || dLDist<= CLEARANCE_DIST || dRDist<= CLEARANCE_DIST) {
    stopFlag =true;
  }
  else {
    stopFlag = false;
  }
  
  
}

void sendData(int controlData[]) {
  for(int i=0; i<CDArrSize; i++) {
    Serial.print(controlData[i]);
    Serial.print(",");
  }
  Serial.println();
}
