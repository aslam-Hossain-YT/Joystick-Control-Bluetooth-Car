//YouTube: Aslam Hossain, https://www.youtube.com/channel/UCdaQfJEKvpoAPIzQNwFGlHw
//Facebook Page: https://www.facebook.com/aslamhossain3852
//For Any Help, Feel Free to knock me through what's App: 01877543825


#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;

  byte rxAxisValue;
  byte ryAxisValue;
};

PacketData data;

// Right motor
int enableRightMotor = 6;
int rightMotorPin1 = 12;
int rightMotorPin2 = 11;

// Left motor
int enableLeftMotor = 5;
int leftMotorPin1 = 10;
int leftMotorPin2 = 9;

unsigned long lastRecvTime = 0;

void setup() {
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  Serial.begin(9600);
  btSerial.begin(38400);
}

void loop() {
  String dataString;
  if (btSerial.available()) {
    dataString = btSerial.readStringUntil('\n');
    sscanf(dataString.c_str(), "%d,%d,%d,%d", &data.lxAxisValue, &data.lyAxisValue, &data.rxAxisValue, &data.ryAxisValue);

    int throttle = map(data.lyAxisValue, 254, 0, -255, 255); // Left stick - y axis - forward/backward car movement
    int steering = map(data.rxAxisValue, 0, 254, -255, 255); // Right stick - x axis - left/right car movement



    int rightMotorSpeed = 0, leftMotorSpeed = 0;

    if (throttle >= 0) {
      rightMotorSpeed = throttle - steering;
      leftMotorSpeed = throttle + steering;
    } else {
      rightMotorSpeed = throttle + steering;
      leftMotorSpeed = throttle - steering;
    }

    rightMotorSpeed = constrain(rightMotorSpeed, -250, 250);
    leftMotorSpeed = constrain(leftMotorSpeed, -250, 250);

    int rightMotorDirection = (rightMotorSpeed >= 0) ? 1 : -1;
    int leftMotorDirection = (leftMotorSpeed >= 0) ? 1 : -1;

    // Control right motor
    if (rightMotorSpeed > 0) {
      digitalWrite(rightMotorPin1, HIGH);
      digitalWrite(rightMotorPin2, LOW);
    } else if (rightMotorSpeed < 0) {
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, HIGH);
    } else {
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, LOW);
    }

    // Control left motor
    if (leftMotorSpeed > 0) {
      digitalWrite(leftMotorPin1, HIGH);
      digitalWrite(leftMotorPin2, LOW);
    } else if (leftMotorSpeed < 0) {
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, HIGH);
    } else {
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, LOW);
    }

    analogWrite(enableRightMotor, abs(rightMotorSpeed));
    analogWrite(enableLeftMotor, abs(leftMotorSpeed));

    lastRecvTime = millis();
  } else {
    unsigned long now = millis();
    if (now - lastRecvTime > 1000) {
      // Signal lost after 1 second. Stop the motors
      analogWrite(enableRightMotor, 0);
      analogWrite(enableLeftMotor, 0);
    }
  }
}
