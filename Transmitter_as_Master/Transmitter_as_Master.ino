//YouTube: Aslam Hossain, https://www.youtube.com/channel/UCdaQfJEKvpoAPIzQNwFGlHw
//Facebook Page: https://www.facebook.com/aslamhossain3852
//For Any Help, Feel Free to knock me through what's App: 01877543825


#include<SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX

struct PacketData 
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
};

int value;

PacketData data;

void setup()
{
  btSerial.begin(38400); 
  Serial.begin(9600);  
}

//This function is used to map 0-1023 joystick value to 0-254. hence 127 is the center value which we send.
//It also adjust the deadband in joystick.
//Jotstick values range from 0-1023. But its center value is not always 511. It is little different.
//So we need to add some deadband to center value. in our case 500-530. Any value in this deadband range is mapped to center 127.
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  //Serial.println(value);
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
  data.lxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A3), false);
  data.lyAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A4), false);
  
  data.rxAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A5), false);
  data.ryAxisValue    = mapAndAdjustJoystickDeadBandValues(analogRead(A6), false);



  String dataString;
  dataString = dataString 
               + data.lxAxisValue + "," 
               + data.lyAxisValue + ","  
               + data.rxAxisValue + ","  
               + data.ryAxisValue + "\n";
  
  btSerial.print(dataString);
  delay(10);
}
