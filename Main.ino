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
#define FIREBASE_HOST "123.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "123"
#define WIFI_SSID "Redmi"
#define WIFI_PASSWORD "rusikloh"

int RelayFan = D2;
int Fan = 1;
int RelayTurbine = D3;
int Turbine = 1;

float Target = 25.0;
float Hyst = 1.0;
float MaxTemp = 70.0;
int Preset = 0;
float EcoOffset = -2.0;
float BoostOffset = 3.0;
int Manual = 0;

long MaxRuntimeSec = 3600;
long onStartMs = 0;
long nowMs = 0;
int turbineState = 0;

float readTempC() {
  int raw = analogRead(A0);
  return (raw / 1023.0) * 100.0;
}

void setTurbine(int s) {
  turbineState = s;
  if (s == 1) {
    digitalWrite(RelayTurbine, LOW);
  }
  else {
    digitalWrite(RelayTurbine, HIGH);
  }
}

void setFan(int s) {
  if (s == 1) {
    digitalWrite(RelayFan, LOW);
  }
  else {
    digitalWrite(RelayFan, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RelayFan, OUTPUT);
  pinMode(RelayTurbine, OUTPUT);
  digitalWrite(RelayFan, HIGH);
  digitalWrite(RelayTurbine, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  nowMs = millis();
  Manual = (int)Firebase.getFloat("Manual");
  Target = Firebase.getFloat("Target");
  Hyst = Firebase.getFloat("Hyst");
  MaxTemp = Firebase.getFloat("MaxTemp");
  MaxRuntimeSec = (long)Firebase.getFloat("MaxRuntimeSec");
  Preset = (int)Firebase.getFloat("Preset");
  EcoOffset = Firebase.getFloat("EcoOffset");
  BoostOffset = Firebase.getFloat("BoostOffset");

  float tempC = readTempC();
  float effTarget = Target;
  if (Preset == 1) {
    effTarget = Target + EcoOffset;
  }
  if (Preset == 2) {
    effTarget = Target + BoostOffset;
  }

  if (Manual == 1) {
    Fan = (int)Firebase.getFloat("Fan");
    Turbine = (int)Firebase.getFloat("Turbine");
    setTurbine(Turbine);
    setFan(Fan);
    if (turbineState == 1 && onStartMs == 0) {
      onStartMs = nowMs;
    }
    if (turbineState == 0) {
      onStartMs = 0;
    }
  }
  else {
    if (turbineState == 0 && tempC <= (effTarget - Hyst)) {
      setTurbine(1);
      onStartMs = nowMs;
    }
    if (turbineState == 1 && tempC >= (effTarget + Hyst)) {
      setTurbine(0);
      onStartMs = 0;
    }
    if (tempC >= MaxTemp) {
      setTurbine(0);
      onStartMs = 0;
    }
    if (turbineState == 1) {
      long runSec = (nowMs - onStartMs) / 1000L;
      if (MaxRuntimeSec > 0 && runSec >= MaxRuntimeSec) {
        setTurbine(0);
        onStartMs = 0;
      }
    }
    if (turbineState == 1) {
      setFan(1);
    }
    else {
      setFan(0);
    }
  }

  Serial.print("tempC=");
  Serial.print(tempC, 2);
  Serial.print(" target=");
  Serial.print(Target, 2);
  Serial.print(" effTarget=");
  Serial.print(effTarget, 2);
  Serial.print(" hyst=");
  Serial.print(Hyst, 2);
  Serial.print(" preset=");
  Serial.print(Preset);
  Serial.print(" manual=");
  Serial.print(Manual);
  Serial.print(" maxT=");
  Serial.print(MaxTemp, 2);
  Serial.print(" maxRun=");
  Serial.print(MaxRuntimeSec);
  Serial.print("s ");
  Serial.print("turbine=");
  Serial.print(turbineState);
  Serial.print(" fan=");
  Serial.println((digitalRead(RelayFan) == LOW) ? 1 : 0);

  delay(250);
}
