String commands[] = {"A", "B", "C", "D", "E", "Q", "R", "S", "T", "U", "P", "M", "I", "O", "W"};

// отправка команды в raspberry
void sendData(String data)
{
  Serial.flush();
  Serial.println();
  Serial.println(data);
  Serial.flush();
}

void sendData(char data)
{
  Serial.flush();
  Serial.println();
  Serial.println(data);
  Serial.flush();
}

void sendData(byte data)
{
  Serial.flush();
  Serial.write(data);
  Serial.flush();
}
