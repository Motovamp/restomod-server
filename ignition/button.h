#define pressDebounce 30ul

#define BUTTON_WAIT       0
#define BUTTON_PRESSED    1
#define BUTTON_RELEASED   2

class Button {
  public:
    Button() {}
    Button(byte pin) {
      _pin = pin;
      pinMode(_pin, INPUT_PULLUP);
    }

    Button(byte pin, bool allways) {
      _pin = pin;
      _allways = allways;
      pinMode(_pin, INPUT_PULLUP);
    }
    
    Button(byte pin, byte pinmode) {
      _pin = pin;
      _pinmode = pinmode;
      pinMode(_pin, _pinmode);
    }
    ~Button() {}

    void init(byte pin) {
      _pin = pin;
      _pinmode = INPUT_PULLUP;
      pinMode(_pin, _pinmode);
    }

    unsigned long duration() {
      return _duration;
    }

    void run() {
      _s = digitalRead(_pin);
      _tm = millis();
      
      _before = _pressed;
      if(_pinmode == INPUT_PULLUP) {
        _pressed = !_s;
      } else {
        _pressed = _s;
      }

      if(_pressed && !_before) {
        _requested = false;
      }
      
//      if(_requested) {
//        _pressed = false;
//        _before = false;
//      }

      
    }

    void req() {
      _requested = true;
    }
 
    byte pressedState() {
      _duration = _tm - _start;
      if(_pressed && !_before) {
        _start = _tm;
      } else if(_pressed && _before && _duration > pressDebounce) {
        if(_requested) return BUTTON_WAIT;
        return BUTTON_PRESSED;
      } else if(!_pressed && _before && _duration > pressDebounce) {
        _start = _tm;
        return BUTTON_RELEASED;
      }

      return BUTTON_WAIT;
    }
    
  private:
    
    
    byte
      _pin, _pinmode;
      
    bool
      _pressed = false, _before = false, _s, _requested = false, _allways = false;
      
    unsigned long
      _tm, _duration, _start;

   

    
};
