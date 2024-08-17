#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData 
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
  bool button1State;
  bool button2State;
};

PacketData data;

void setup()
{
  btSerial.begin(38400); 
  Serial.begin(9600);  

  pinMode(4, INPUT_PULLUP); // Button 1 connected to pin 4 with internal pull-up resistor
  pinMode(5, INPUT_PULLUP); // Button 2 connected to pin 5 with internal pull-up resistor
}

// This function is used to map 0-1023 joystick value to 0-254.
// The function also adjusts the deadband in the joystick.
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 550)
  {
    value = map(value, 550, 1023, 127, 254);
  }
  else if (value <= 480)
  {
    value = map(value, 480, 0, 127, 0);  
  }
  else
  {
    value = 127;
  }

  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

void loop()
{
  data.lxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A1), false);
  data.lyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A2), false);
  
  data.rxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A6), false);
  data.ryAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A7), false);

  data.button1State   = !digitalRead(4); // Read the state of button 1 (inverted due to pull-up)
  data.button2State   = !digitalRead(5); // Read the state of button 2 (inverted due to pull-up)

  Serial.print(data.lxAxisValue);
  Serial.print(" ");
  Serial.print(data.lyAxisValue);
  Serial.print(" ");
  Serial.print(data.rxAxisValue);
  Serial.print(" ");
  Serial.print(data.ryAxisValue);
  Serial.print(" ");
  Serial.print(data.button1State);
  Serial.print(" ");
  Serial.println(data.button2State);

  String dataString;
  dataString = dataString 
               + data.lxAxisValue + "," 
               + data.lyAxisValue + ","  
               + data.rxAxisValue + ","  
               + data.ryAxisValue + ","
               + data.button1State + ","
               + data.button2State + "\n";
  
  btSerial.print(dataString);
  delay(10);
}
