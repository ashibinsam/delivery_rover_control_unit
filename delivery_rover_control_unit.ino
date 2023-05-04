#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#define FULL_SPEED_DIST 100
#define TURN_DIST 60
#define EMERGENCY_DIST 15
#define SIDE_MIN_DIST 30
#define CLEARANCE_DIST 15
#define MIN_SPEED 180

static const int RXPin = 14, TXPin = 15;
String s = "www.google.com/maps/dir/";
unsigned long interval = 10000;
static const uint32_t GPSBaud = 9600;
unsigned long previousMillis = 0;
int data_counter;

const size_t BUFSIZE = 300;
char f_buffer[BUFSIZE];
float *f_buf = (float*)f_buffer;

TinyGPSPlus gps;// The TinyGPSPlus object
SoftwareSerial ss(RXPin, TXPin);// The serial connection to the GPS device


int ControlArr[5] = { 0, 0, 0, 0, 0 };
int rFDist = 0, lFDist = 0, rDist = 0, lDist = 0, dLDist = 0, dRDist = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
float headingDegrees;
bool stopFlag = false;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);

  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);

  // if (!mag.begin()) {
  //   /* There was a problem detecting the HMC5883 ... check your connections */
  //   Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
  //   while (1)
  //     ;
  // }

  Serial.println("Starting A9G...");
  ss.println("\r");
  ss.println("AT\r");
  delay(10);

  ss.println("\r");
  ss.println("AT+GPS=1\r");

  delay(100);
  // ss.println("AT+CREG=2\r");
  // delay(6000);

  // //ss.print("AT+CREG?\r");
  // ss.println("AT+CGATT=1\r");
  // delay(6000);

  // ss.println("AT+CGDCONT=1,\"IP\",\"WWW\"\r");
  // delay(6000);

  // // ss.println("AT+LOCATION=1\r");
  // ss.println("AT+CGACT=1,1\r");
  // delay(6000);

  //Initialize ends
  //Initialize GPS
  ss.println("\r");
  ss.println("AT+GPS=1\r");
  delay(1000);

  //ss.println("AT+GPSMD=1\r");   // Change to only GPS mode from GPS+BDS, set to 2 to revert to default.
  ss.println("AT+GPSRD=10\r");
  delay(100);

  // set SMS mode to text mode
  ss.println("AT+CMGF=1\r");
  delay(1000);

  //ss.println("AT+LOCATION=2\r");

  Serial.println("Setup Executed");
}

void loop() {
  t1 = millis();  //The millis function returns the number of milliseconds since Arduino board has been powered up.
  //COMPASS CODE START
  // sensors_event_t event;
  // mag.getEvent(&event);
  // float heading = atan2(event.magnetic.y, event.magnetic.x);
  // float declinationAngle = -1.58;
  // heading += declinationAngle;

  // Correct for when signs are reversed.
  // if (heading < 0)
  //   heading += 2 * PI;

  // // Check for wrap due to addition of declination.
  // if (heading > 2 * PI)
  //   heading -= 2 * PI;

  // // Convert radians to degrees for readability.
  // float headingDegrees = heading * 180 / M_PI;
  //COMPASS CODE END
  // Serial.print(leftDis());
  // Serial.print("   ");
  // delay(50);
  // Serial.println(rightDis());
  // delay(50);
  ping();
  sendData(ControlArr);


  while (lFDist > FULL_SPEED_DIST && rFDist > FULL_SPEED_DIST && dRDist > CLEARANCE_DIST && dLDist > CLEARANCE_DIST && rDist > CLEARANCE_DIST-10 && lDist > CLEARANCE_DIST-10) {
    if (stopFlag) break;
    ControlArr[0] = 1;
    ControlArr[1] = 0;
    ControlArr[2] = 255;
    ControlArr[3] = 5;
    sendData(ControlArr);
    ping();
  }
  while ((lFDist <= FULL_SPEED_DIST || rFDist <= FULL_SPEED_DIST) && lFDist > TURN_DIST && rFDist > TURN_DIST && dRDist > CLEARANCE_DIST && dLDist > CLEARANCE_DIST && rDist > CLEARANCE_DIST-10 && lDist > CLEARANCE_DIST-10) {
    // rover reduced speed according to distance using an equation
    if (stopFlag) break;
    ControlArr[0] = 1;
    ControlArr[1] = 0;
    ControlArr[2] = min(lFDist, rFDist) + 125;
    ControlArr[3] = 10;
    sendData(ControlArr);
    ping();
  }
  while (lFDist <= TURN_DIST || rFDist <= TURN_DIST || dRDist <= CLEARANCE_DIST || dLDist <= CLEARANCE_DIST) {  
    // Turn rover according to the distance value using an equation
    if (stopFlag) break;
    if (lFDist <= SIDE_MIN_DIST || rFDist <= SIDE_MIN_DIST && rDist > SIDE_MIN_DIST && rDist > lDist) {
      //full turn right
      if (stopFlag) break;
      ControlArr[0] = 1;
      ControlArr[1] = 1;
      ControlArr[2] = MIN_SPEED;
      ControlArr[3] = 3;
      ControlArr[4] = 255;
      sendData(ControlArr);
      ping();
    }
    else if (lFDist <= SIDE_MIN_DIST || rFDist <= SIDE_MIN_DIST && lDist > SIDE_MIN_DIST && lDist >= rDist) {
      //full turn lrft
      if (stopFlag) break;
      ControlArr[0] = 1;
      ControlArr[1] = -1;
      ControlArr[2] = MIN_SPEED;
      ControlArr[3] = 3;
      ControlArr[4] = 255;
      sendData(ControlArr);
      ping();
    }
    
    else if (rDist > TURN_DIST && lDist > TURN_DIST ) {
      if (stopFlag) break;
      
      /* Code to turn according to GPS bearing data */
      if (rDist > lDist ) {
        if (stopFlag) break;
        ControlArr[0] = 1;
        ControlArr[1] = 1;
        ControlArr[2] = rDist + min(rFDist, lFDist) + MIN_SPEED;
        ControlArr[3] = 40;
        ControlArr[4] = 255  - min(min(min(lDist, dLDist), lFDist), rFDist);
        if (ControlArr[4] > 255) ControlArr[4] = 255;
        if (ControlArr[2] > 255) ControlArr[2] = 255;
      } 
      else {
        if (stopFlag) break;
        ControlArr[0] = 1;
        ControlArr[1] = -1;
        ControlArr[2] = lDist + min(lFDist, rFDist) + MIN_SPEED;
        ControlArr[3] = 40;
        ControlArr[4] = 240 - min(min(min(rDist, dRDist), rFDist), lFDist);
        if (ControlArr[4] > 255) ControlArr[4] = 255;
        if (ControlArr[2] > 255) ControlArr[2] = 255;
      }
    } else if (rDist > SIDE_MIN_DIST && rDist > lDist) {
      if (stopFlag) break;
      ControlArr[0] = 1;
      ControlArr[1] = 1;
      ControlArr[2] = MIN_SPEED;
      ControlArr[3] = 3;
      ControlArr[4] = 255;
      sendData(ControlArr);
      ping();
    }  else if (lDist > SIDE_MIN_DIST) {
      if (stopFlag) break;
      ControlArr[0] = 1;
      ControlArr[1] = -1;
      ControlArr[2] = MIN_SPEED;
      ControlArr[3] = 3;
      ControlArr[4] = 255;
      sendData(ControlArr);
      ping();
    } else if (dRDist >= TURN_DIST  && dLDist >= TURN_DIST ) {
      if (stopFlag) break;
      if (dRDist > dLDist) {
        if (stopFlag) break;
        /* turn 45 degree right */
        ControlArr[0] = 1;
        ControlArr[1] = 1;
        ControlArr[2] = dRDist + min(rFDist, lFDist) + MIN_SPEED;
        ControlArr[3] = 40;
        ControlArr[4] = 240 - min(min(min(lDist, dLDist), lFDist), rFDist);
        if (ControlArr[4] > 255) ControlArr[4] = 255;
        if (ControlArr[2] > 255) ControlArr[2] = 255;
      } else {
        if (stopFlag) break;
        /* turn 45 degree left */
        ControlArr[0] = 1;
        ControlArr[1] = -1;
        ControlArr[2] = dLDist + min(lFDist, rFDist) + MIN_SPEED;
        ControlArr[3] = 40;
        ControlArr[4] = 240 - min(min(min(rDist, dRDist), rFDist), lFDist);
        if (ControlArr[4] > 255) ControlArr[4] = 255;
        if (ControlArr[2] > 255) ControlArr[2] = 255;
      }
    } else if (dRDist >= SIDE_MIN_DIST && dRDist > dLDist) {
      if (stopFlag) break;
      ControlArr[0] = 1;
      ControlArr[1] = 1;
      ControlArr[2] = MIN_SPEED;
      ControlArr[3] = 3;
      ControlArr[4] = 255;
      sendData(ControlArr);
      ping();

    } else if (dLDist >= SIDE_MIN_DIST) {
      if (stopFlag) break;
      ControlArr[0] = 1;
      ControlArr[1] = -1;
      ControlArr[2] = MIN_SPEED;
      ControlArr[3] = 3;
      ControlArr[4] = 255;
      sendData(ControlArr);
      ping();
    }


    ping();
    sendData(ControlArr);
  }
  while (stopFlag) {
    ControlArr[0] = 0;
    ControlArr[1] = 0;
    ControlArr[2] = 0;
    ControlArr[3] = 0;
    ControlArr[4] = 0;
    ping();
    sendData(ControlArr);

  }

  // while(lFDist <= TURN_DIST && rFDist > TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
  //   // Turn rover left slightly according to the distance value using an equation
  //   if(stopFlag) break;

  //   ping();
  //   sendData(ControlArr);
  // }

  // while(lFDist > TURN_DIST && rFDist <= TURN_DIST && rDist > SIDE_MIN_DIST && lDist > SIDE_MIN_DIST){
  //   // Turn rover right slightly according to the distance value using an equation
  //   if(stopFlag) break;

  //   ping();
  //   sendData(ControlArr);
  // }

  // while(rDist <= SIDE_MIN_DIST && lFDist > TURN_DIST && rFDist > TURN_DIST) {
  //   ping();
  //   //slightly turn left
  //   sendData(ControlArr);

  // }
  // while(lDist <= SIDE_MIN_DIST && lFDist > TURN_DIST && rFDist > TURN_DIST) {
  //   ping();
  //   //slightly turn right
  //   sendData(ControlArr);
  // }

  ping();
  // Serial.print(headingDegrees);
  // Serial.println("");
  // Serial.print(headingDegrees);
  // Serial.println("");
  sendData(ControlArr);
  GPSGPRS();
}




void GPSGPRS() {
  smartDelay(10);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {

    send_gps_data();
    previousMillis = currentMillis;
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void send_gps_data()
{
  if (gps.location.lat() == 0 || gps.location.lng() == 0)
  {
    //Serial.println("Return Executed");
    return;
  }

  data_counter++;

  //Serial.print("Latitude (deg): ");
  f_buf[data_counter] = gps.location.lat();
  //Serial.println(f_buf[data_counter]);

  //Serial.print("Longitude (deg): ");
  f_buf[data_counter + 1] = gps.location.lng();
  //Serial.println(f_buf[data_counter + 1]);

  //Serial.println(data_counter);
  //Serial.println();

  // s += String(gps.location.lat(), 6);
  // s += ",";
  // s += String(gps.location.lng(), 6);
  // s += "/";

  //Serial.println(s);

  if (data_counter >= 10)
  {
    data_counter = 0;

    // Serial.println("Sending Message");

    // ss.println("AT+CMGF=1\r");
    // delay(1000);

    // ss.println("AT+CNMI=2,2,0,0,0\r");
    // delay(1000);

    // ss.print("AT+CMGS=\"+91xxxxxxxxxx\"\r");//Replace this with your mobile number
    // delay(1000);
    // ss.print(s);
    // ss.write(0x1A);
    // delay(1000);
    // s = "www.google.com/maps/dir/";
  }
}

