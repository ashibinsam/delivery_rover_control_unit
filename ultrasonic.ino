
int readDis(int trigPin, int echoPin)
{ 
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

void ping() {
  if (abs(t2 - t1)> 20) {
    rFDist=fRightDis();
    Serial.print(rFDist);
    Serial.print("   ");
  }
  if (abs(t2 - t1)> 40) {
    lDist=leftDis();
    Serial.print(lDist);
    Serial.print("   ");
  }
  if (abs(t2 - t1)> 60) {
    lFDist=fLeftDis();
    Serial.print(lFDist);
    Serial.print("   ");
  }
  if (abs(t2 - t1)> 80) {
    rDist=rightDis();
    Serial.println(rDist);
    t2=millis();
  }
  
  
  
  
}
