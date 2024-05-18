#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (4, 5);
byte addresses[][6] = {"0"};

uint16_t rc[16];

typedef struct __telem_t {
  uint8_t type;
  float ground_speed;
  float pitch;
  float yaw;
  float roll;
  int32_t lat;
  int32_t lon;
  int32_t alt;
} Telem;

Telem tel;

void setup() {
  setCpuFrequencyMhz(240);
  delay(1000);
  Serial.begin(460800);
  myRadio.begin();  
  myRadio.setPayloadSize(255);
  myRadio.setChannel(115); 
  myRadio.disableAckPayload();
  myRadio.setRetries(0, 0);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {
  if ( myRadio.available()) {
    myRadio.read(&tel, sizeof(tel));
    Serial.write((uint8_t *)&tel, sizeof(tel));
  }

  if (Serial.available()) {
    while (Serial.available()){
      Serial.readBytes((uint8_t *)rc, 32);
    }
  }

  delay(5);

  myRadio.stopListening();
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(rc, sizeof(rc));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}
