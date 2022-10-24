#define BLYNK_TEMPLATE_ID "TMPLdP-Z23DH"
#define BLYNK_DEVICE_NAME "Competition1A Team 1"
#define BLYNK_AUTH_TOKEN "qd_pLBMjkiFF6Pq4s6S6dLFzXl3m9tEg"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hotspotify";
char pass[] = "huhuhuhu";

int echoPin0 = D5;
int echoPin1 = D1;
int echoPin2 = D4;
int echoPin3 = D2;
int echoPin4 = D3;
int trigPin0 = D6;
int trigPin1 = D7;
int trigPin2 = D8;

int sensorArray[] = {D5, D1, D4, D2, D3};
int trigArray[] = {D7, D8, D8, D6, D7};
int minDistanceArray[] = {1, 30, 5, 29, 2};
int maxDistanceArray[] = {9, 38, 13, 37, 10};
//change this for each team
int virtualLEDPinArray[] = {V8, V9, V10, V11, V12};
int trialPinArray[] = {V0, V1, V2};
boolean state = false;
int LEDTrialPoint[] = {0, 0, 0};
int soundTrialPoint[] = {0, 0, 0};
int point[] = {0, 0 , 0};
int trial = -1;
int timer = 0;
#define SOUND_SPEED 0.034

void setup()
{
  Serial.begin(115200);
  pinMode(echoPin0, INPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin0, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}

//Change this for each team
BLYNK_WRITE(V6)
{
  if (param.asInt() == 1) {
    state = false;
    trial = trial + 1;
    for (int i = 0; i < 5; i++) {
      timer = 0;
      while (state == false) {
        state = measureSensor(sensorArray[i], trigArray[i], minDistanceArray[i], maxDistanceArray[i]);
        timer = timer + 1;
        if (timer > 160) {
          goto bailout;
        }
      }
      point[trial] = point[trial] + 1;
      Blynk.virtualWrite(trialPinArray[trial], point[trial]);
      Blynk.virtualWrite(virtualLEDPinArray[i], 1);
      state = false;
      delay(1500);
    }
    resetLED();

    for (int i = 0; i < 5; i++) {
      timer = 0;
      while (state == false) {
        state = measureSensor(sensorArray[i], trigArray[i], minDistanceArray[i], maxDistanceArray[i]);
        timer = timer + 1;
        if (timer > 160) {
          goto bailout;
        }
      }
      Blynk.virtualWrite(virtualLEDPinArray[i], 1);
      state = false;
      delay(1500);
    }
    point[trial] = point[trial] + 3;
    Blynk.virtualWrite(trialPinArray[trial], point[trial]);
    resetLED();
  }
bailout:
  resetLED();
  Blynk.virtualWrite(V6, 0);
}

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1) {
    Blynk.virtualWrite(trialPinArray[0], 0);
    delay(100);
    Blynk.virtualWrite(V3, 0);
  }
}

BLYNK_WRITE(V4)
{
  if (param.asInt() == 1) {
    Blynk.virtualWrite(trialPinArray[1], 0);
    delay(100);
    Blynk.virtualWrite(V4, 0);
  }
}

BLYNK_WRITE(V5)
{
  if (param.asInt() == 1) {
    Blynk.virtualWrite(trialPinArray[2], 0);
    delay(100);
    Blynk.virtualWrite(V5, 0);
  }
}

BLYNK_WRITE(V24)
{
  LEDTrialPoint[0] = param.asInt();
  Blynk.virtualWrite(trialPinArray[0], point[trial]+LEDTrialPoint[0]+soundTrialPoint[0]);
}

BLYNK_WRITE(V25)
{
  LEDTrialPoint[1] = param.asInt();
  Blynk.virtualWrite(trialPinArray[1], point[trial]+LEDTrialPoint[1]+soundTrialPoint[1]);
}

BLYNK_WRITE(V26)
{
  LEDTrialPoint[2] = param.asInt();
  Blynk.virtualWrite(trialPinArray[2], point[trial]+LEDTrialPoint[2]+soundTrialPoint[2]);
}

BLYNK_WRITE(V27)
{
  soundTrialPoint[0] = param.asInt()*2;
  Blynk.virtualWrite(trialPinArray[0], point[trial]+LEDTrialPoint[0]+soundTrialPoint[0]);
}

BLYNK_WRITE(V28)
{
  soundTrialPoint[1] = param.asInt()*2;
  Blynk.virtualWrite(trialPinArray[1], point[trial]+LEDTrialPoint[1]+soundTrialPoint[1]);
}

BLYNK_WRITE(V29)
{
  soundTrialPoint[2] = param.asInt()*2;
  Blynk.virtualWrite(trialPinArray[2], point[trial]+LEDTrialPoint[2]+soundTrialPoint[2]);
}

void resetLED() {
  for (int i = 0; i < 5; i++) {
    Blynk.virtualWrite(virtualLEDPinArray[i], 0);
    delay(5);
  }
}

boolean measureSensor(int sensor_name, int trig_name, int min_distance, int max_distance) {
  long duration;
  float distanceCm;
  int avg_value = 0;
  int filterArray[20];
  digitalWrite(trig_name, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_name, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_name, LOW);
  duration = pulseIn(sensor_name, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  for (int sample = 0; sample < 20; sample++) {
    filterArray[sample] = distanceCm;
    delay(5);
  }
  for (int i = 0; i < 19; i++) {
    for (int j = i + 1; j < 20; j++) {
      if (filterArray[i] > filterArray[j]) {
        int swap = filterArray[i];
        filterArray[i] = filterArray[j];
        filterArray[j] = swap;
      }
    }
  }
  long sum = 0;
  for (int sample = 6; sample < 14; sample++) {
    sum += filterArray[sample];
  }
  avg_value = sum / 7;
  if (avg_value > 80) {
    avg_value = 99;
  }
  Serial.println(sensor_name);
  Serial.println(avg_value);
  if (min_distance < avg_value && avg_value < max_distance) {
    return true;
  } else {
    return false;
  }
}
