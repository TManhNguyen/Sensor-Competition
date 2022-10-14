#define BLYNK_TEMPLATE_ID "TMPLdP-Z23DH"
#define BLYNK_DEVICE_NAME "Competition"
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
int minDistanceArray[] = {2, 29, 4, 28, 1};
int maxDistanceArray[] = {10, 37, 12, 36, 9};
//change this for each team
int virtualLEDPinArray[] = {V16, V17, V18, V19, V20};
int trialPinArray[] = {V13, V14, V15};
boolean state = false;
int point = 0;
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
BLYNK_WRITE(V7)
{
  if (param.asInt() == 1) {
    delay(100);
    state = false;
    trial = trial + 1;
    point = 0;
    for (int i = 0; i < 5; i++) {
      timer = 0;
      while (state == false) {
        state = measureSensor(sensorArray[i], trigArray[i], minDistanceArray[i], maxDistanceArray[i]);
        timer = timer + 1;
        if (timer > 140) {
          goto bailout;
        }
      }
      point = point + 1;
      Blynk.virtualWrite(trialPinArray[trial], point);
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
        if (timer > 140) {
          goto bailout;
        }
      }
      Blynk.virtualWrite(virtualLEDPinArray[i], 1);
      state = false;
      delay(1500);
    }
    point = point + 3;
    Blynk.virtualWrite(trialPinArray[trial], point);
    resetLED();
  }
bailout:
  resetLED();
  point = 0;
  Blynk.virtualWrite(V7, 0);
}

BLYNK_WRITE(V21)
{
  if (param.asInt() == 1) {
    Blynk.virtualWrite(trialPinArray[0], 0);
    delay(100);
    Blynk.virtualWrite(V21, 0);
  }
}

BLYNK_WRITE(V22)
{
  if (param.asInt() == 1) {
    Blynk.virtualWrite(trialPinArray[1], 0);
    delay(100);
    Blynk.virtualWrite(V22, 0);
  }
}

BLYNK_WRITE(V23)
{
  if (param.asInt() == 1) {
    Blynk.virtualWrite(trialPinArray[2], 0);
    delay(100);
    Blynk.virtualWrite(V23, 0);
  }
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
