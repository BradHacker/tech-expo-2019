#include <Servo.h>

#define MOVEMENT_SPEED 1

char faceState = '0';
int angle = 90;
unsigned long lastFaceTime = 0;

Servo servo;

void setup() {
  // Init servo and set to 90 degrees
  servo.attach(3);
  Serial.begin(9600);
  servo.write(90);
  delay(1000);
}

void loop() {
  unsigned long millisOnLoop = millis();
  // If we are sending data from Processing
  if (Serial.available() > 0) {
    // Get movement state
    faceState = Serial.read() - '0';
    // If move clockwise
    if (faceState == 1 && angle < 180) {
      angle += MOVEMENT_SPEED;
      lastFaceTime = millisOnLoop;
    }
    // If move counter clockwise
    if (faceState == 2 && angle > 0) {
      angle -= MOVEMENT_SPEED;
      lastFaceTime = millisOnLoop;
    }
    // If shouldn't move
    if (faceState == 0) lastFaceTime = millisOnLoop;
    // If no face and it's been longer than 6 secs since a face
    if (faceState == 4 && millisOnLoop - lastFaceTime > 6000UL) {
      angle = 90;
    }
    // Set servo angle
    servo.write(angle);
    // Send back current angle to Processing
    Serial.println(angle);
  }
}
