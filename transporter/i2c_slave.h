#include <Wire.h>
#include "raspi_sender.h"

class Receiver {
    public:
        Receiver() {}
        Receiver(byte addr) {
            _addr = addr;
        }
        ~Receiver() {}

    private:
        byte
            _addr;
        bool
            _connected = false;
};
