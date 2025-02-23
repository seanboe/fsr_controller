#include "AD5143.h"


AD5143::AD5143(){};

bool AD5143::init(uint8_t address) {
  this->address = address;
  Wire.begin();
  return true;
}

void AD5143::writePacket(Packet data) {
  Wire.beginTransmission(this->address);
  Wire.write((uint8_t)(data.tot >> 8));
  Wire.write((uint8_t)(data.tot & 0xFF));
  Wire.endTransmission();
}

uint8_t AD5143::read(Packet readData) {
  this->writePacket(readData);

  Wire.requestFrom(this->address, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

bool AD5143::setChannelResistance(uint8_t channel, uint8_t taps) {

  if (taps > 255 || channel > 8)
    return false;

  Packet packet;
  packet.control = WRITE_RDAC;
  packet.address = channel;
  packet.data = taps;

  this->writePacket(packet);
  
  return true;
}

bool AD5143::setEEPROM(uint8_t channel) {

  if (channel > 8)
    return false;

  Packet packet;
  packet.control = COPY_RDAC_EEPROM;
  packet.address = channel;
  packet.data = 1;

  writePacket(packet);

  return true;
}