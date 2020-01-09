#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
SoftwareSerial dfserial(2,3); // To Define Pin for Software Serial
#define trigPin1 12 // For Ultra Sonic Sensor                               
#define echoPin1 11 // For Ultra Sonic Sensor                                
int SMS = 8;
int buzzer = 13;
int ldr_pin = A0;
int water_pin = A2;
int key1 = 7; // Buzzer Receiver part
int key2 = A1; // Emergency Button
int dif2=100, ldr, wtr, btnstate1 , btnstate2;
//btnstate1 - For Buzzer ; btnstate2 - For GPS-GSM Module

long duration, distance, UltraSensor1 ;

void SonarSensor(int trigPinSensor,int echoPinSensor) 
{
  digitalWrite(trigPinSensor, LOW);// Put trigpin LOW 
  delayMicroseconds(2);// Wait 2 microseconds
  digitalWrite(trigPinSensor, HIGH);// Switch trigpin HIGH
  delayMicroseconds(10); // Wait 10 microseconds
  digitalWrite(trigPinSensor, LOW);// Turn it LOW again
  duration = pulseIn(echoPinSensor, HIGH);//PulseIn funtion will return the time on how much the configured pin remain the level HIGH or LOW; in this case it will return how much time echoPinSensor stay HIGH
  distance= (duration/2) / 29.1; // First we have to divide the duration by two  
}
void Hcsr_04()
{
  SonarSensor(trigPin1, echoPin1);              // Look Below to Find the Definition of the SonarSensor Function
  UltraSensor1 = distance;
  Serial.print("D1 ");Serial.print(UltraSensor1);
  
  if(UltraSensor1 > 0 && UltraSensor1 < 200)
  {
    if(UltraSensor1 < 30 )
    {
      myDFPlayer.playFolder(1, 4);   // Sound 004 Obstacle Alert 
      delay(2000);
    } 
  }
  
}


// This function is used to define the states of pin that we have chosen
void setup()
{
  
  Serial.begin(9600);
  dfserial.begin(9600);
 // Serial.println();
  pinMode(SMS , OUTPUT);
  digitalWrite(SMS, HIGH);
  pinMode(buzzer , OUTPUT);
  digitalWrite(buzzer, LOW);
  pinMode(key1,INPUT_PULLUP);
  pinMode(key2,INPUT_PULLUP);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT); 
  if (!myDFPlayer.begin(dfserial)) 
  {  //Use softwareSerial to communicate with mp3.
    Serial.print("MP3 oFFline.     "); 
    while(true);
  }
  Serial.print("MP3 online.     ");
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  delay(1000);
  myDFPlayer.playFolder(1, 1);    // Welcome Sound 001
  delay(1000);
  SonarSensor(trigPin1, echoPin1);              // Look Below to find the definition of the SonarSensor function
  UltraSensor1 = distance;
}

void loop()
{
 Hcsr_04(); 
 wtr=analogRead(water_pin);
 Serial.print("  Water : ");Serial.print(wtr);
 ldr=analogRead(ldr_pin);
 Serial.print("  ldr : ");Serial.print(ldr);
 btnstate1=digitalRead(key1);
 Serial.print("  Key1 : ");Serial.print(btnstate1);
 
 btnstate2=digitalRead(key2);
 Serial.print("  Key2 : ");Serial.print(btnstate2);
 Serial.println("");
 
 if(wtr > dif2)
 {
     Serial.println("Water Alert");
     myDFPlayer.playFolder(1, 2);   // Sound 002 Water Alert
     delay(2000);
 }
 if(ldr > 950)
 {
     Serial.println("Dark Alert");
     myDFPlayer.playFolder(1, 3);  // Sound 003 Darkness Alert
     delay(2000);
 }
 if(btnstate1 == 0)
 {
     Serial.println("Find Me Stick");
     digitalWrite(buzzer, HIGH);
     myDFPlayer.playFolder(1, 5);   // Sound 005 Find me
     delay(2000);
 }
 else
 {
     digitalWrite(buzzer, LOW);
 }
 if(btnstate2 == 0)
 {
     Serial.println("Send SmS ");   
     digitalWrite(SMS, LOW);
 }
 else
 {
     digitalWrite(SMS, HIGH);
 }
  
}

//This Part tells about the status of DFPlayer

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
