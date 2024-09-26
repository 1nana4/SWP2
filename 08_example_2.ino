// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 10.0    // minimum distance to be measured (unit: cm)
#define _DIST_MAX 30.0    // maximum distance to be measured (unit: cm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficient to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() { 
  float distance;

  // wait until next sampling time
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  // LED 밝기 제어
  int ledBrightness = calculateBrightness(distance);

  // LED 밝기 설정
  analogWrite(PIN_LED, ledBrightness);

  // output the distance and LED brightness to the serial port
  Serial.print("Distance: "); Serial.print(distance);
  Serial.print(" cm, LED Brightness: "); Serial.println(ledBrightness);

  // update last sampling time
  last_sampling_time += INTERVAL;
}

// 거리 기반 LED 밝기 계산 함수
int calculateBrightness(float distance) {
  if (distance <= 10.0) { // 1 cm 이하
    return 255; // 꺼짐
  } else if (distance > 10.0 && distance < 15.0) { // 1cm ~ 1.5cm
    return map(distance, 10, 15, 255, 128); // 50% 밝기
  } else if (distance >= 15.0 && distance <= 20.0) { // 1.5cm ~ 2.0cm
    return 0; // 최대 밝기
  } else if (distance > 20.0 && distance < 25.0) { // 2.0cm ~ 2.5cm
    return 128; // 50% 밝기
  } else if (distance >= 25.0 && distance <= 30.0) { // 2.5cm ~ 3.0cm
    return map(distance, 25, 30, 128, 255); // 점차 꺼짐
  } else {
    return 255; // 꺼짐
  }
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
