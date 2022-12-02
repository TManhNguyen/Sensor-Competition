#define BLYNK_TEMPLATE_ID "TMPL-PIX42Lm"
#define BLYNK_DEVICE_NAME "Competition Round 3 Team 2"
#define BLYNK_AUTH_TOKEN "3ydFHbPGLl1UB7CtGETzUFWgaYU0idiy"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hotspotify";
char pass[] = "huhuhuhu";


int point = 0;
int trial = 1;
int trialPinArray[] = {V99,V8,V6,V7};
int A = 0;
int B = 0;
int C = 0;
int ultrasonic = 0;
int conditional = 0;
int colour = 0;
int AR = 0; 

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V5)
{
  if(param.asInt() == 1){
    A = 2;
  }else{
    A = 0;
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}

BLYNK_WRITE(V9)
{
  if(param.asInt() == 1){
    B = 2;
  }else{
    B = 0;
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}

BLYNK_WRITE(V10)
{
  if(param.asInt() == 1){
    C = 2;
  }else {
    C = 0;
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}
BLYNK_WRITE(V4)
{
  trial = param.asInt();
}

BLYNK_WRITE(V0)
{
  if(param.asInt() == 1){
    ultrasonic = 4;
  }else{
    ultrasonic = 0;
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}

BLYNK_WRITE(V1)
{
  if(param.asInt() == 1){
    conditional = 4;
  }else{
    conditional = 0;
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}

BLYNK_WRITE(V2)
{
  if(param.asInt() == 1){
    colour = 4;
  }else{
    colour = 0;
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}

BLYNK_WRITE(V3)
{
  if(param.asInt() == 1){
    AR = 2;
  }else{
    AR = 0; 
  }
  Blynk.virtualWrite(trialPinArray[trial], A + B + C + ultrasonic + conditional + colour + AR);
}
