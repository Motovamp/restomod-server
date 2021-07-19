#include <EEPROM.h>

void dataLoad(int address, int storage[], int slength)
{
  for(int i = 0; i < slength; i++)
  {
    EEPROM.get(address, storage[i]);
    address += sizeof(int);
  }
}

void dataInit(int address, int storage[], int slength)
{
  for(int i = 0; i < slength; i++)
  {
    EEPROM.put(address, storage[i]);
    address += sizeof(int);
  }
}

void edataSet(int address, int data)
{
  EEPROM.put(address, data);
}
