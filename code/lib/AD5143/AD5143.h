#ifndef AD5143_H
#define AD5143_H

#include <Arduino.h>
#include <Wire.h>

typedef union {
  struct {
    uint8_t data : 8;
    uint8_t address : 4;
    uint8_t control : 4;
  };
  uint16_t tot;
} Packet;


#define WRITE_RDAC 0b0001
#define WRITE_REG 0b0010
#define READ 0b0011
#define COPY_RDAC_EEPROM 0b0111
#define RESET 0b1011
#define SHUTDOWN 0b1011

class AD5143 {

  public: 
    AD5143();

    bool init(uint8_t address);
    bool setChannelResistance(uint8_t channel, uint8_t taps);
    bool setEEPROM(uint8_t channel=8);

  private:
    uint8_t read(Packet readData);
    void writePacket(Packet data);

    uint8_t address;

};

#endif