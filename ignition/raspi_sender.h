#include <Wire.h>

// отправка команды в raspberry
void sendData(String data)
{
  Serial.flush();
  Serial.println();
  Serial.println(data);
  Serial.flush();
  
    Wire.beginTransmission(0x04);
//    Serial.println("transmission started");
    Wire.write(data[0]);
//    Serial.println("transmission complete");
    Wire.endTransmission();
//    Serial.println("transmission end");

    Wire.requestFrom(0x04, 1);    // request potentiometer position from slave 0x04
    if(Wire.available()) {        // read response from slave 0x04
      byte i2c_rcv = Wire.read();
    }
}

void sendData(char data)
{
    Wire.beginTransmission(4);     // transmit to device #4
    Wire.write(data);              // sends one byte 
    Wire.endTransmission();
}

void sendData(byte data)
{
    Wire.beginTransmission(4);     // transmit to device #4
    Wire.write(data);              // sends one byte 
    Wire.endTransmission();
}
