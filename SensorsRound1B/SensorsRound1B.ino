#define BLYNK_TEMPLATE_ID "TMPLhZaDl7eX"
#define BLYNK_DEVICE_NAME "Competition 2"
#define BLYNK_AUTH_TOKEN "DuCE1ZctsnE5lWQ6mQmJk0Ucn-HIcutS"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Hotspotify";
char pass[] = "huhuhuhu";
int scoreTeam1[] = {0,0,0,0,0};
int scoreTeam2[] = {0,0,0,0,0};
int trialTeam1 = 1;
int trialArray1[] = {V99, V11, V12, V13};
int trialTeam2 = 1;
int trialArray2[] = {V99, V14, V15, V16};


// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V20)
{
  if (param.asInt() == 1) {
    int score = 0;
    for(int i = 0; i<5; i++){
      score = score + scoreTeam1[i];
    }
    Blynk.virtualWrite(trialArray1[trialTeam1], score*2);
  }
}

BLYNK_WRITE(V19)
{
  if (param.asInt() == 1) {
    int score = 0;
    for(int i = 0; i<5; i++){
      score = score + scoreTeam2[i];
    }
    Blynk.virtualWrite(trialArray2[trialTeam2], score*2);
  }
}

BLYNK_WRITE(V0)
{
  scoreTeam1[0] = param.asInt();
}
BLYNK_WRITE(V1)
{
  scoreTeam1[1] = param.asInt();
}
BLYNK_WRITE(V2)
{
  scoreTeam1[2] = param.asInt();
}
BLYNK_WRITE(V3)
{
  scoreTeam1[3] = param.asInt();
}
BLYNK_WRITE(V4)
{
  scoreTeam1[4] = param.asInt();
}
BLYNK_WRITE(V17)
{
  trialTeam1 = param.asInt();
}

BLYNK_WRITE(V5)
{
  scoreTeam2[0] = param.asInt();
}
BLYNK_WRITE(V6)
{
  scoreTeam2[1] = param.asInt();
}
BLYNK_WRITE(V7)
{
  scoreTeam2[2] = param.asInt();
}
BLYNK_WRITE(V8)
{
  scoreTeam2[3] = param.asInt();
}
BLYNK_WRITE(V9)
{
  scoreTeam2[4] = param.asInt();
}
BLYNK_WRITE(V18)
{
  trialTeam2 = param.asInt();
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
