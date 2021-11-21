/************
 * Haresh Nayak
 * Traffic Light Information System
 ***********/

#include<ESP8266WiFi.h>
#include<FirebaseArduino.h>

#define FIREBASE_HOST "trafficlightnodemcu-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "xwUiAslXRP9gEeB8s94c2utnWF0OLuyHeBpdjuuq"
#define WIFI_SSID "TinyTim" 
#define WIFI_PASSWORD "Tim@98chAr#8"

const int trigPin = 12;
const int echoPin = 14;

float d1 = 0.0;
float d2 = 0.0;
float Speed = 0.0;

int red_duration = 3;
int yellow_duration = 7;
int green_duration = 11;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

int traffic_light_timer(int duration){
  int d = duration;
  if(d>0){
    d -= 1;
  }
 else if(d == 0){
    d = 12;
  }
  return d;
}
void  traffic_light(){
  red_duration = traffic_light_timer(red_duration);
  yellow_duration = traffic_light_timer(yellow_duration);
  green_duration = traffic_light_timer(green_duration);
}

void setup() {
 Serial.begin(115200); // Starts the serial communication  
   
  // connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conecting...");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("connecting...");
    delay(500); 
  }
  Serial.println();
  Serial.print("connected");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  d1 = (int)(0.1723 * readUltrasonicDistance(trigPin,echoPin));
     
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {

  traffic_light();
  Firebase.setFloat("distance_1", d1);
  Firebase.setInt("red",red_duration);
  Firebase.setInt("yellow", yellow_duration);
  Firebase.setInt("green", green_duration);
  
  d2 = (int)(0.01723 * readUltrasonicDistance(trigPin, echoPin));

  Speed = (d2-d1);
  Serial.println(Speed);  
   
  Serial.print("distance1 : ");
  Serial.println(d1);
  
  Serial.print("distance2 : ");
  Serial.println(d2);
  Firebase.setFloat("distance_2", d2);
  
  Serial.print("speed : ");
  Serial.println(Speed);
  Firebase.setFloat("currentSpeed", Speed);
  Serial.println(); 
  

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /distance failed:");
      Serial.println(Firebase.error());  
      return;
  }  
  d1 = d2;
  delay(1000);
}
