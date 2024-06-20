#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

#include <AD5143.h>

#define AD_1_ADDR 0x28
#define AD_2_ADDR 0x2B

AD5143 ad1;
AD5143 ad2;

#define ADS_1_ADDRESS 0x4B
#define ADS_2_ADDRESS 0x4A

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

#define CHANNEL_COUNT 8

// Array that holds the voltage read on each channel (not in order)
float pressure_data[CHANNEL_COUNT];

// the index of the map holds the index of that index in the pressure_data array
uint8_t pressure_data_map[] = {6, 5, 7, 4, 1, 0, 2, 3};

unsigned long prevtime;
// In microseconds
#define UPDATE_DELAY    20000

float getFsrChannel(uint8_t fsr_channel);
void setRheostats();
void setup(void)
{
  Serial.begin(115200);
  Wire.setClock(400000);

  ad1.init(AD_1_ADDR);
  ad2.init(AD_2_ADDR);

  if (!ads1.begin(ADS_1_ADDRESS, &Wire)) {
    Serial.println("Failed to initialize ADS1. ");
    while (1);
  }

  if (!ads2.begin(ADS_2_ADDRESS, &Wire)) {
    Serial.println("Failed to initialize ADS2. ");
    while (1);
  }

  setRheostats();
  prevtime = micros();

}

void loop(void) {

  if (prevtime - micros() > 50000) {

    for (int x = 0; x < CHANNEL_COUNT; x++) {
      if (x < 4)
        pressure_data[x] = ads1.computeVolts(ads1.readADC_SingleEnded(x % 4));
      else
        pressure_data[x] = ads2.computeVolts(ads2.readADC_SingleEnded(x % 4));
      // Serial.print("CHAN" + String(x) + ": " + pressure_data[x] + ", ");
    }

    for (int x = 0; x < 8; x++) {
      Serial.print(String(getFsrChannel(x)) + ",");
    }
    Serial.println();

    prevtime = micros();

  }

  // // Get upper stimulation
  // float upper_stim = get_fsr_channel(5) * 0.4 + get_fsr_channel(4) * 0.2 + get_fsr_channel(2) * 0.15 + get_fsr_channel(3) * 0.15 + get_fsr_channel(1) * 0.1;
  // upper_stim = (UPPER_LIN_RISE / 3.3) * upper_stim;

  // float lower_stim = get_fsr_channel(8) * 0.5 + get_fsr_channel(7) * 0.3 + get_fsr_channel(6) * 0.2;
  // lower_stim = (LOWER_LIN_RISE / 3.3) * lower_stim;

  // if (micros() - prevtime >= UPDATE_DELAY) {
  //   char buffer[30];
  //   sprintf(buffer, "<%s,%s>", String(upper_stim, 1).c_str(), String(lower_stim, 1).c_str());
  //   Serial.println(buffer);
  //   Serial.flush();
  //   prevtime = micros();
  // }

}

// Put in the number on the datasheet, don't zero index
float getFsrChannel(uint8_t fsr_channel) {
  return pressure_data[pressure_data_map[fsr_channel]];
}

void setRheostats() {
  ad1.setChannelResistance(8, 240);
  ad2.setChannelResistance(8, 240);
}
