#define SOUND_SPEED 0.034

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long duration;
  float distanceCm;
  int avg_value = 0;
  int filterArray[20];
  digitalWrite(D6, LOW);
  delayMicroseconds(2);
  digitalWrite(D6, HIGH);
  delayMicroseconds(10);
  digitalWrite(D6, LOW);
  duration = pulseIn(D5, HIGH);
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
  Serial.print("D5: ");
  Serial.println(avg_value);
}
