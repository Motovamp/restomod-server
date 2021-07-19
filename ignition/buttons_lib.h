#include "ignition.h"

#define MeasureError 8
#define BDebounce 12ul
#define BClick 150ul
#define BLong 1000ul
#define BIdle 1500ul

class Buttons {
  public:
    Buttons(byte pin) {
       _pin = pin;
       _prgbtn.init(5);
    }
    
    void storeInit(int block[6], Gearbox *gearbox) {
       for(int i = 0; i < 6; i++) {
        _block[i] = block[i];
       }

       _gearbox = *gearbox;
    }
    
    void iteration(int *mode, int *omode) {
        tm = millis();
        int current = ButtonIndex(analogRead(_pin), _block, MeasureError);
      
        if(current == 100) // если ничего не нажато
        {
           
          if(prev > -1 && prev < 5 && bdur > BDebounce) // а на предыдущей итерации было нажато
          {
            if(!(prev == 1 && bdur > BIdle))
              if(_menu && prev == 1) {
                _menu = false;
              } else {
                sendData(_bCommand[prev]);
              }
              
              if(prev == 1 && *mode == PROGRAMMING) {
                progIterator ++;
                _gearbox.dataSet(progIterator);
                if(progIterator == 4)
                {
                  *mode = *omode;
                }
              }     
          }
          bdur = 0;
        }
        else   // если что-то нажато
        { 
          if(current != prev)   // и нажатие новое
          {
            bstart = tm;     // обнуляем счетчик времени нажатия
          }
          else
          {
            bdur = tm - bstart;  // считаем длительность нажатия
            if(current == 1 && bdur > BIdle  /*&& *mode != MENU */)  // кнопка ок нажата больше полутора сенунд
            {
              sendData("M");  // вход в меню
              _menu = true;
              bstart = tm;
            }
          }
        } 
        prev = current;

        /// юстировка коробаса
        _prgbtn.run();
        if(_prgbtn.pressedState() == BUTTON_PRESSED) {
          _prgbtn.req();
          *omode = *mode;
          *mode = PROGRAMMING;
          progIterator = -1;
        }
    }
  private:
    byte _pin;
    bool _menu = false;
    Button _prgbtn;
    Gearbox _gearbox;
    String _bCommand[5] = {"A", "B", "C", "D", "E"};
    int _block[6], prev = 100, progIterator = -1;
    unsigned long tm, bstart, bdur;
};
