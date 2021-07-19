#include "i2c_slave.h"

byte working = false;

void setup() {
	Serial.begin(9600);
//	for(;;) {
//		sendData(commands[random(15)]);
//		delay(500);
//	}

//  pinMode(2, OUTPUT);

  Wire.begin(0x04);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(dataRequest);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void dataRequest() {
  Wire.write(working);
}

void receiveEvent(int bytes) {
//  String data;
  char data = Wire.read();
  
//  int c = 0;
//  Serial.println(Wire.available());
//   while (Wire.available()) { 
//    data[c] = Wire.read();     // принять байт как символ
//  }
  sendData(data);
}



void loop() {
  working = analogRead(0) > 200 ? 1 : 0;
  digitalWrite(8, !digitalRead(2));
  digitalWrite(9, !digitalRead(3));
}
