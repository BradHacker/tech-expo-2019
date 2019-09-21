#include <Servo.h>

#define MOVEMENT_SPEED 1
#define SPEAKER_1 8
#define SPEAKER_2 9
#define SPEAKER_3 10
#define SPEAKER_4 11

#define OVERLAP 20

char faceState = '0';
int angle = 90;
unsigned long lastFaceTime = 0;

Servo servo;

void setup() {
  // Init servo and set to 90 degrees
  servo.attach(3);
  pinMode(SPEAKER_1, OUTPUT);
  pinMode(SPEAKER_2, OUTPUT);
  pinMode(SPEAKER_3, OUTPUT);
  pinMode(SPEAKER_4, OUTPUT);
  digitalWrite(SPEAKER_1, HIGH);
  digitalWrite(SPEAKER_2, HIGH);
  digitalWrite(SPEAKER_3, HIGH);
  digitalWrite(SPEAKER_4, HIGH);
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
  }
  if(millisOnLoop - lastFaceTime > 6000UL) {
      angle = 90;
    }
    // Set servo angle
    servo.write(angle);
    // Turn on appropriate speakers
    //char[] buf;
    //Serial.readBytesUntil('|', buf, 3);
    activateSpeakers(angle);
    // Send back current angle to Processing
    Serial.println(angle);
}

void activateSpeakers(int a) {
  if (a < 180 && a > (135 - (OVERLAP / 2))) digitalWrite(SPEAKER_1, LOW);
  else digitalWrite(SPEAKER_1, HIGH);

  if (a < (135 + (OVERLAP / 2)) && a > (90 - (OVERLAP / 2))) digitalWrite(SPEAKER_2, LOW);
  else digitalWrite(SPEAKER_2, HIGH);

  if (a < (90 + (OVERLAP / 2)) && a > (45 - (OVERLAP / 2))) digitalWrite(SPEAKER_3, LOW);
  else digitalWrite(SPEAKER_3, HIGH);

  if (a < (45 + (OVERLAP / 2)) && a > 0) digitalWrite(SPEAKER_4, LOW);
  else digitalWrite(SPEAKER_4, HIGH);
}
