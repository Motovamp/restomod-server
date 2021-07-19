#include "button.h"
#include "gearbox.h"

#define startLevel  200ul
#define starterTime 3000ul
#define pompeTime   2000ul

#define SLOW LOW
#define SHIGH HIGH

class Ignition {
  public:
    Ignition(byte brake, byte startb, byte alarm, byte parking, byte engine, byte butLed, byte ignition, byte starter, byte raspbpower) {
     
      iBrake.init(brake);
      
      iStartb.init(startb); 
      iAlarm.init(alarm);
      iParking.init(parking);
      
      _enginePin = engine;
       
      _raspbPowPin = raspbpower;
      _butLedPin = butLed;
      _ignitionPin = ignition;
      _starterPin = starter;

      pinMode(_butLedPin, OUTPUT);
      pinMode(_raspbPowPin, OUTPUT);
      pinMode(_ignitionPin, OUTPUT);
      pinMode(_starterPin, OUTPUT);

    }

    void run(int *mode, int *omode) {

      *omode = *mode;
      reading();
      tm = millis();
      sdur = sstart ? tm - sstart : 0;
      pdur = pstart ? tm - pstart : 0;
      if(_alarm == BUTTON_PRESSED) {
        *mode = WATING;
//         sending("fuck");    
        digitalWrite(_raspbPowPin, SLOW);
      } else {
//        Serial.println("gtest");
        digitalWrite(_raspbPowPin, SHIGH);
        if(*mode == WATING) {
//          sending("O");          
          if(_brake == BUTTON_WAIT && _startb == BUTTON_PRESSED && _parking == BUTTON_PRESSED) {
//            Serial.println("test");
            sstart = 0;
            *mode = FAST_START;
            iStartb.req();
          } else if(_startb == BUTTON_PRESSED) {
            *mode = PUMP_PAUSE;
            iStartb.req();
          }
        } else if(*mode == IGNITION_ON) {
//           Serial.println("ignition");          
          if(_startb == BUTTON_PRESSED) {
            if(_brake == BUTTON_WAIT && _parking == BUTTON_PRESSED) {
              Serial.println("starting");  
              *mode = STARTING;
              sstart = tm;
            } else if(_parking == BUTTON_WAIT && _brake == BUTTON_WAIT) { 
              // выводить инфо на дисплей коробас не в паркинге!!!!!!!!!!!!!!
                iStartb.req();
            } else {
              *mode = WATING;
              iStartb.req();
            }
          }        
        } else if(*mode == STARTING) {
          if(_engineState) {
            *mode = WORKING;
            sstart = 0;
          } else if(sdur > starterTime && _startb != BUTTON_PRESSED) {
            *mode = IGNITION_ON;
            sstart = 0;
          }
        } else if(*mode == WORKING) {
          if(!_engineState) {
            *mode = IGNITION_ON;
          }
          
          if(_parking == BUTTON_PRESSED && _startb == BUTTON_PRESSED) {
            *mode = WATING;
            iStartb.req();
          }
        } else if(*mode == PUMP_PAUSE) {
//          sending("I");          

          if(pstart == 0) {
            pstart = tm;
          }
          
          if(pdur > pompeTime) {
            if(_fs) {
              sstart = tm;
              *mode = STARTING;
            } else {
              sstart = 0;
              *mode = IGNITION_ON;
            }

            _fs = false;
            pstart = 0;
          }
        } else if(*mode == FAST_START) {
          _fs = true;
          *mode = PUMP_PAUSE;
        } else if(*mode == PROGRAMMING) {
          return;
        }
      }

      writing(*mode);
    }

    void sending(String val) {
      if (val != oldSend) {
        sendData(val);
        oldSend = val;
      }
      return;
      
//      if(!sendStart) {
//        sendData(val);
//        sendStart = tm;
//      }
//
//      unsigned long ld = tm - sendStart;
//      
//      if(ld > 1000) {
//        sendData(val);
//        sendStart = tm;
//      }
    }

  private:
    byte
      _brake, // тормоз
      _startb, // кнопка
      _enginePin,// датчик масла
      _parking, // датчик паркинга
      _alarm, // минус от синалки !!! инвертировать
      _raspbPowPin, // реле питания малинки
      _butLedPin, // светодиод кнопки
      _ignitionPin, // реле зажигания
      _starterPin;   // реле стартера
    bool
      _engineState, _fs = false;

    String oldSend = "Z";
      
    Button 
      iBrake,
      iStartb, 
      iAlarm,
      iParking; 
  
    unsigned long 
      tm, 
      sendStart = 0,
      sstart = 0,
      pstart = 0,  
      sdur,
      pdur;
      
    void reading() {
      _engineState = analogRead(_enginePin) > startLevel;
      
      iBrake.run();
      iStartb.run(); 
      iAlarm.run();
      iParking.run();

      _brake = iBrake.pressedState();
      _startb = iStartb.pressedState();
      _parking = iParking.pressedState();
      _alarm = iAlarm.pressedState();
      
    }

    void writing(byte _mode) {
      if(_mode == WATING) {
        digitalWrite(_butLedPin, SLOW);
        digitalWrite(_ignitionPin, SLOW);
        digitalWrite(_starterPin, SLOW);
//        sending("WATING");
      } else if(_mode == PUMP_PAUSE) {
        digitalWrite(_butLedPin, SLOW);
        digitalWrite(_ignitionPin, SHIGH);
        digitalWrite(_starterPin, SLOW);
//        sending("PUMP");
      } else if(_mode == IGNITION_ON) {
        digitalWrite(_butLedPin, SLOW);
        digitalWrite(_ignitionPin, SHIGH);
        digitalWrite(_starterPin, SLOW);
//        sending("IGNITION_ON");
      } else if(_mode == STARTING) {
        digitalWrite(_butLedPin, SLOW);
        digitalWrite(_ignitionPin, SHIGH);
        digitalWrite(_starterPin, SHIGH);
//        sending("STARTING");
      } else if(_mode == WORKING) {
        digitalWrite(_butLedPin, SHIGH);
        digitalWrite(_ignitionPin, SHIGH);
        digitalWrite(_starterPin, SLOW);
//        sending("WARKING");
      } else if(_mode == PROGRAMMING) {
        digitalWrite(_butLedPin, SLOW);
        digitalWrite(_ignitionPin, SHIGH);
        digitalWrite(_starterPin, SLOW);
//        sending("PROGRAMMING");
      } 
    }
};
