#include <Servo.h>

const int TRIG_PIN = 7;
const int ECHO_PIN = 6;
const int SERVO_PIN = 10;
const int LED_PIN = 9;

const float EMA_ALPHA = 0.3;

const int MIN_DIST = 18;
const int MAX_DIST = 36;

Servo myServo;
float filteredDistance = 0.0;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);

  filteredDistance = getRawDistance();
}

void loop() {
  float rawDistance = getRawDistance();

  filteredDistance = (EMA_ALPHA * rawDistance) + (1.0 - EMA_ALPHA) * filteredDistance;

  if (filteredDistance >= MIN_DIST && filteredDistance <= MAX_DIST) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  int angle = 0;

  if (filteredDistance <= MIN_DIST) {
    angle = 0;
  } else if (filteredDistance >= MAX_DIST) {
    angle = 180;
  } else {
    angle = map(filteredDistance, MIN_DIST, MAX_DIST, 0, 180);
  }

  myServo.write(angle);

  Serial.print("Raw: ");
  Serial.print(rawDistance);
  Serial.print("cm, Filtered: ");
  Serial.print(filteredDistance);
  Serial.print("cm, Angle: ");
  Serial.println(angle);

  delay(50);
}

float getRawDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.034) / 2.0;

  return distance;
}
