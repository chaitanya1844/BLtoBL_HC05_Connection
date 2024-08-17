#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
  bool button1State;
  bool button2State;
};

PacketData data;

// Right motor
int rightMotorPin1 = 12;
int rightMotorPin2 = 13;

// Left motor
int leftMotorPin1 = 10;
int leftMotorPin2 = 11;

unsigned long lastRecvTime = 0;

void setup() {
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(7,OUTPUT);
    Serial.begin(9600);
  btSerial.begin(38400);
}

void loop() {
  String dataString;
  if (btSerial.available()) {
    dataString = btSerial.readStringUntil('\n');
    sscanf(dataString.c_str(), "%d,%d,%d,%d,%d,%d", &data.lxAxisValue, &data.lyAxisValue, &data.rxAxisValue, &data.ryAxisValue, &data.button1State, &data.button2State);
    
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

    // Commented out PWM control code
    // analogWrite(enableRightMotor, abs(rightMotorSpeed));
    // analogWrite(enableLeftMotor, abs(leftMotorSpeed));

    // Handling button inputs
    if (data.button1State) {
      // Action when button 1 is pressed (e.g., additional control, LED on, etc.)
      digitalWrite(7, HIGH);
      delay(5000);
      digitalWrite(7, LOW);

    } else {
      // Action when button 1 is not pressed
      // digitalWrite(LED_BUILTIN, LOW);
    }

    if (data.button2State) {
      // Action when button 2 is pressed (e.g., another control, different mode, etc.)
      // Perform desired action for button 2
    }

    lastRecvTime = millis();
  } /* else {
    unsigned long now = millis();
    if (now - lastRecvTime > 1000) {
      // Signal lost after 1 second. Stop the motors
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, LOW);
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, LOW);
    } 
  } */
}
