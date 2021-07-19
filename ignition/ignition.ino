#include "buttons_lib.h"

const int
    // analog inputs
    buttonsPin = 1,   // Кнопки
    shiftPin = 2,     // Коробка
    Engine = 3,  // датчик давления масла  
    // raspberryPin = 5, // вход малинки

    // digital inputs
    Parking   = 2,
    Brake     = 3,
    Ibut      = 4,
    programmButton = 5,
    Alarm     = 6,
          
    // digital outputs
    RaspbPower= 8,
    Oled1     = 9,
    IGN       = 10,
    STR       = 11;

int 
    oldMode = WATING,       // Предыдущий режим
    curMode = WATING,         // Текущий режим
    scurrent = 100,   // index текущей передачи
    sprev = 100,     // индекс предыдущей передачи

    adjustments[6] = {848, 792, 728, 648, 561, 370}, // Значения АЦП блока кнопок
    sadjustments[6] = {200, 400, 600, 800, 1000, 0};          // Значения АЦП положения коробки

bool 
  raspberryWork = false, 
  raspberryWorkBefore = false;

Buttons bBlock(buttonsPin);
Gearbox gBox(shiftPin);
Ignition iBlock(Brake, Ibut, Alarm, Parking, Engine, Oled1, IGN, STR, RaspbPower);

void setup() {
  Serial.begin(9600);
  Wire.begin();
//  pinMode(4, INPUT_PULLUP);

//  for(;;) {
//    delay(100);
//    
//    Serial.println(analogRead(2));
//  }

  // первичная инициализация
//  dataInit(0, adjustments, 6);
//  dataInit(20, sadjustments, 6);

  // Загрузка данных из EEPROM
//  dataLoad(0, adjustments, 6);
  dataLoad(20, sadjustments, 6);

  gBox.storeInit(sadjustments);
  bBlock.storeInit(adjustments, &gBox);


}


void loop() {  
  
  // зажигание
  iBlock.run(&curMode, &oldMode);
  
  // малинка
//  raspberryWork = true; //digitalRead(transporterPin);
  Wire.requestFrom(0x04, 1);
  if(Wire.available()) {
    raspberryWork = Wire.read();
  }
  if(raspberryWork) {

    if(curMode != WATING) {
      
      // коробас
      gBox.iteration(curMode);
      // кнопки
      bBlock.iteration(&curMode, &oldMode);
    }
      
    
    if(!raspberryWorkBefore || curMode != oldMode) {
      gBox.dataSend();
      if(curMode == WATING) {
        iBlock.sending("O");
      } else if(curMode == WORKING) {
        iBlock.sending("W");
      } else if(curMode == PROGRAMMING) {
        iBlock.sending("P");
      } else {
        iBlock.sending("I");
      }
    }
  }
  raspberryWorkBefore = raspberryWork;  
}
