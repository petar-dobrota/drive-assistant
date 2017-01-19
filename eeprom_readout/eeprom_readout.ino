#include <EEPROM.h>

struct RpmThrottle {
  int rpm;
  unsigned char throttlePos;
};

void setup() {
  Serial.begin(57600);
  Serial.println("hello");

  for(int addr = 0; addr < EEPROM.length(); addr += sizeof(RpmThrottle)) {
    
    RpmThrottle point;
    EEPROM.get(addr, point);

    Serial.print((int) point.throttlePos);
    Serial.print(' ');
    Serial.println(point.rpm);
  }
  
}

void loop() {
  while(1);
}
