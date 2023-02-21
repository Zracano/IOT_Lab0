#include <Arduino.h>
#include <ESP32Servo.h>
#include <TFT_eSPI.h>

const int servoPin = 32;
const int lightsensor = 15;
const int ledPin = 17;
bool ledOn = false;
TFT_eSPI tft = TFT_eSPI();
Servo servo;

int minLight = -1, maxLight = -1;

bool calibrating = true;

void setup() {
  pinMode(lightsensor, INPUT);
  pinMode(ledPin, OUTPUT);
  servo.attach(servoPin);

  tft.init(); // ini
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK); 
}

void loop() {
  if(calibrating) {
    int light = analogRead(lightsensor);

    if(minLight == -1) {
      minLight = light;
      maxLight = light;
    } else {
      if(light < minLight) minLight = light;
      if(light > maxLight) maxLight = light;
    }

    digitalWrite(ledPin, (millis() / 500) % 2);

    if(millis() >= 10000) calibrating = false;
  } else {
    int light = analogRead(lightsensor);
    int output = 179 * (light - minLight) / (maxLight - minLight);
    servo.write(output);

    tft.setCursor(0, 0);
    tft.setTextSize(2); 
    tft.setTextColor(TFT_GREEN); 
    tft.fillScreen(TFT_BLACK); 
    tft.println("Light: " + String(light)); 
    tft.println("Output: " + String(output));
  }
}