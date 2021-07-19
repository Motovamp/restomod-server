#include "eeprom_load.h"
#include "raspi_sender.h"
#include "mode_consts.h"

class Gearbox {
  public:
    Gearbox() {}
    Gearbox(byte pin) {
      _pin = pin;
    }
    void storeInit(int block[5]) {
       for(int i = 0; i < 5; i++) {
        _block[i] = block[i];
       }
    }
    void iteration(int mode) {
//      Serial.println(/analogRead(_pin));
      scurrent = ButtonIndex(analogRead(_pin), _block, 20);  // Получаем индекс положения коробки
      if(scurrent > -1 && scurrent < 5 && scurrent != prev && mode != PROGRAMMING)
      {
        sendData(_bCommand[scurrent]);   
        prev = scurrent;
      }
    }
    void dataSet(int index) {
      _block[index] = analogRead(_pin);
      edataSet(index * sizeof(int) + 20, _block[index]);
    }

    void dataSend() {
      sendData(_bCommand[scurrent]);
    }
  private:
    byte _pin;
    String _bCommand[5] = {"Q", "R", "S", "T", "U"};
    int _block[5], prev, scurrent;
};
