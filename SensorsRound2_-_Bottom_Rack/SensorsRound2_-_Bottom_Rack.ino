#define BLYNK_TEMPLATE_ID "TMPL1MIagbWT"
#define BLYNK_DEVICE_NAME "Competition2 Team 2"
#define BLYNK_AUTH_TOKEN "2OhnpOyDyQT0J85hTIoJWpfRY_NIoMNg"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hotspotify";
char pass[] = "huhuhuhu";

int echoPin0 = D0;
int echoPin1 = D1;
int echoPin2 = D2;
int echoPin3 = D3;
int echoPin4 = D4;
int trigPin0 = D5;
int trigPin1 = D6;
int trigPin2 = D7;
int trigPin3 = D8;

int sensorArray[] = {D0, D1, D2, D3, D4};
int trigArray[] = {D5, D5, D6, D7, D8};
int minDistanceArray[] = {8, 3, 0, 0, 10}; //Update distance
int maxDistanceArray[] = {18, 13, 10, 10, 18}; //Update distance
//change this for each team
int virtualLEDPinArray[] = {V0, V1, V2, V3, V4};
int trialPinArray[] = {V99, V5, V6, V7};
boolean state = false;
int conditionalPoint = 0;
int soundPoint = 0;
int sendPoint = 0;
int receivePoint = 0;
int point[] = {0, 0 , 0};
int trialBonus = 1;
int trial = 1;
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
  pinMode(trigPin3, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}

//Change this for each team
BLYNK_WRITE(V13)
{
  if (param.asInt() == 1) {
    resetLED();
    delay(100);
    state = false;
    //Measure 1+2
    for (int i = 0; i < 2; i++) {
      timer = 0;
      while (state == false) {
        state = measureSensor(sensorArray[i], trigArray[i], minDistanceArray[i], maxDistanceArray[i]);
        timer = timer + 1;
        if (timer > 200) {
          goto bailout;
        }
      }
      point[trial] = point[trial] + 2;
      Blynk.virtualWrite(virtualLEDPinArray[i], 1);
      Blynk.virtualWrite(trialPinArray[trial], point[trial]);
      state = false;
      delay(1000);
    }

    //Measure 3/4
    state = false;
    timer = 0;
    while (state == false) {
      boolean state2 = measureSensor(sensorArray[2], trigArray[2], minDistanceArray[2], maxDistanceArray[2]);
      state = (state == true || state2 == true);
      if (state2 == true)
      {
        Blynk.virtualWrite(virtualLEDPinArray[2], 1);
      }
      boolean state3 = measureSensor(sensorArray[3], trigArray[3], minDistanceArray[3], maxDistanceArray[3]);
      state = (state == true || state3 == true);
      if (state3 == true)
      {
        Blynk.virtualWrite(virtualLEDPinArray[3], 1);
      }
      timer = timer + 1;
      if (timer > 250) {
        goto bailout;
      }
    }
    point[trial] = point[trial] + 2;
    Blynk.virtualWrite(trialPinArray[trial], point[trial]);

    //Measure 5
    state = false;
    timer = 0;
    while (state == false) {
      state = measureSensor(sensorArray[4], trigArray[4], minDistanceArray[4], maxDistanceArray[4]);
      timer = timer + 1;
      if (timer > 200) {
        goto bailout;
      }
    }
    point[trial] = point[trial] + 2;
    Blynk.virtualWrite(virtualLEDPinArray[4], 1);
    state = false;
    delay(1000);
    Blynk.virtualWrite(trialPinArray[trial], point[trial]);
  }
bailout:
  trial = trial + 1;
  Blynk.virtualWrite(V13, 0);
  Blynk.virtualWrite(V14, trial);
}

BLYNK_WRITE(V8)
{
  conditionalPoint = param.asInt();
  Blynk.virtualWrite(trialPinArray[trialBonus], point[trialBonus] + conditionalPoint + soundPoint + sendPoint + receivePoint);
}

BLYNK_WRITE(V9)
{
  soundPoint = param.asInt();
  Blynk.virtualWrite(trialPinArray[trialBonus], point[trialBonus] + conditionalPoint + soundPoint + sendPoint + receivePoint);
}

BLYNK_WRITE(V10)
{
  sendPoint = param.asInt();
  Blynk.virtualWrite(trialPinArray[trialBonus], point[trialBonus] + conditionalPoint + soundPoint + sendPoint + receivePoint);
}

BLYNK_WRITE(V11)
{
  receivePoint = param.asInt();
  Blynk.virtualWrite(trialPinArray[trialBonus], point[trialBonus] + conditionalPoint + soundPoint + sendPoint + receivePoint);
}

BLYNK_WRITE(V12)
{
  trialBonus = param.asInt();
}

BLYNK_WRITE(V14)
{
  trial = param.asInt();
}

BLYNK_WRITE(V15)
{
  if (param.asInt() == 1) {
    point[0] = 0;
    Blynk.virtualWrite(trialPinArray[1], 0);
    delay(100);
    Blynk.virtualWrite(V15, 0);
  }
}

BLYNK_WRITE(V16)
{
  if (param.asInt() == 1) {
    point[1] = 0;
    Blynk.virtualWrite(trialPinArray[2], 0);
    delay(100);
    Blynk.virtualWrite(V16, 0);
  }
}

BLYNK_WRITE(V17)
{
  if (param.asInt() == 1) {
    point[2] = 0;
    Blynk.virtualWrite(trialPinArray[3], 0);
    delay(100);
    Blynk.virtualWrite(V17, 0);
  }
}

void resetLED() {
  for (int i = 0; i < 5; i++) {
    Blynk.virtualWrite(virtualLEDPinArray[i], 0);
    delay(10);
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
