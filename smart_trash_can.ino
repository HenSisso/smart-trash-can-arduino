// Smart Trash Can - Arduino Uno project
// Measures bin fill level with HC-SR04 and shows status via LCD + RGB LED
// Full project description: see README.md

#include <LiquidCrystal.h>

// HC-SR04 pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// RGB LED pins
#define RED_PIN 13
#define GREEN_PIN 5
#define BLUE_PIN 6

// I measured the bin depth with a ruler, it is about 40 cm from the sensor to the bottom
#define BIN_DEPTH 40

LiquidCrystal lcd(12, 11, 7, 4, 3, 2);

void setup() {
  Serial.begin(9600); // I use this for debugging while testing the sensor

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Smart Trash Can");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  // Serial.print("duration: "); Serial.println(duration); // I used this while testing

  int distance = duration * 0.034 / 2; // 0.034 is the speed of sound in cm per microsecond

  // Sometimes the sensor gives strange values, so I keep the distance inside the bin range
  if (distance > BIN_DEPTH) {
    distance = BIN_DEPTH;
  }
  if (distance < 0) {
    distance = 0;
  }

  int fullPercentage = ((BIN_DEPTH - distance) * 100) / BIN_DEPTH;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Trash Full: ");
  Serial.print(fullPercentage);
  Serial.println("%");

  lcd.setCursor(0, 0);
  lcd.print("Status: Running ");

  lcd.setCursor(0, 1);
  lcd.print("Full: ");
  lcd.print(fullPercentage);
  lcd.print("%   "); // I added spaces here to remove old numbers from the screen

  // I use green when the bin is empty, blue when it is filling, and red when it is full
  if (fullPercentage < 50) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  }
  else if (fullPercentage <= 85) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  }
  else {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }

  // TODO: Maybe I will add a buzzer when the bin goes above 85%, I need to check if I have a free pin
  delay(500);
}
