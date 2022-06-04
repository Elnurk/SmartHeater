#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <WiFiClientSecure.h>
#include <stdio.h>
//https://www.grc.com/fingerprints.html Fingerprints site
// Set these to run example.
#define FIREBASE_HOST "raushan-52f9a-default-rtdb.europe-west1.firebasedatabase.app" //Firebase project link
#define FIREBASE_AUTH "8abA4M3rDkOX8Jh2TmWlDFRaYeyVhYqYqNi4RPR3" //Firebase key
#define WIFI_SSID "TEACHERS" //WIFI name
#define WIFI_PASSWORD "Sch2021pass" //WIFI password

int RelayFan = D2;
int RelayTurbine = D3;
int Fan = 0;
int Turbine = 0;

void setup() {
  // connect to wifi.
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(RelayFan, OUTPUT);
  pinMode(RelayTurbine, OUTPUT);
}

void loop() {
  Turbine = Firebase.getFloat("Turbine");
  Fan = Firebase.getFloat("Fan");
  Serial.println(Fan);
  Serial.println(Turbine);
  if(Turbine == 0){
    digitalWrite(RelayTurbine, LOW);
  }
  else if(Turbine == 1){
    digitalWrite(RelayTurbine, HIGH);
  }
  if(Fan == 0){
    digitalWrite(RelayFan, LOW);
  }
  else if (Fan == 1){
    digitalWrite(RelayFan, HIGH);
  }
}
