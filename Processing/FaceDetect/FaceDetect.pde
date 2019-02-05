import processing.serial.*;

import processing.video.*;

import gab.opencv.*;
import java.awt.Rectangle;

OpenCV opencv;
Capture video;
Serial arduino;

int THRESHHOLD = 50;
int SMALLEST_FACE = 100;
int LARGEST_FACE = 200;

int angle = 90;

// Serial port /dev/cu.usbmodem14601
void setup() {
  size(1280, 960);
  println(Capture.list()[0]);
  video = new Capture(this, Capture.list()[0]);
  opencv = new OpenCV(this, width, height);
  opencv.loadCascade(OpenCV.CASCADE_FRONTALFACE);
  
  arduino = new Serial(this, "/dev/cu.usbmodem14601", 9600);
  
  THRESHHOLD = width / 25;
  SMALLEST_FACE = width / 7;
  LARGEST_FACE = width / 2;
  println("THRESHHOLD: " + THRESHHOLD);
  println("SMALLEST_FACE: " + SMALLEST_FACE);
  println("LARGEST_FACE: " + LARGEST_FACE);
  
  video.start();
}

void draw() {
  if (video.height > 0 && video.width > 0) {
    opencv.loadImage(video);
    //opencv.contrast(.2);
    // opencv.brightness(20);
    
    image(video, 0, 0);
    
    noFill();
    strokeWeight(3);
    stroke(0,255,0);
    line((width/2) - THRESHHOLD,0,(width/2) - THRESHHOLD,height);
    line((width/2) + THRESHHOLD,0,(width/2) + THRESHHOLD,height);
    
    serialEvent();
    stroke(255);
    rectMode(CENTER);
    textSize(18);
    textAlign(CENTER);
    text(angle, width/2, 20);
    
    stroke(255,0,0);
    strokeWeight(3);
    
    Rectangle[] faces = opencv.detect(2.0, 0, 5, SMALLEST_FACE, LARGEST_FACE);
    // println("Faces: " + faces.length);
    
    if (faces.length > 0) {
      rectMode(CORNER);
      rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height);
      rectMode(CENTER);
      stroke(0,0,255);
      rect(faces[0].x + faces[0].width / 2, faces[0].y + faces[0].height / 2, SMALLEST_FACE, SMALLEST_FACE);
      stroke(255,255,0);
      rect(faces[0].x + faces[0].width / 2, faces[0].y + faces[0].height / 2, LARGEST_FACE, LARGEST_FACE);
      int middleX = faces[0].x + (faces[0].width / 2);
      // 2 - move right
      // 1 - move left
      // 0 - don't move
      if (middleX > width / 2 + THRESHHOLD) {
        arduino.write('2');
        // println(2);
      }
      else if (middleX < width / 2 - THRESHHOLD) {
        arduino.write('1');
        // println(1);
      }
      else {
        arduino.write('0');
        // println(0);
      }
    }  else {
      arduino.write('4');
      // println(4);
    }
  } else {
    arduino.write('4');
    // println(4);
  }
}

void captureEvent(Capture c) {
  c.read();
}

void serialEvent() {
  try {
    if (arduino != null && arduino.active()) {
      String val = "";
      byte[] b = arduino.readBytes();
      printArray(b);
      for (int i = 0; i < b.length; i++) {
        val += (char) b[i];
      }
      println("val: " + val);
    
      val.trim();
      
      angle = Integer.parseInt(val);
    }
  }
  catch (RuntimeException e) {
    // println(e.getMessage());
  }
}
