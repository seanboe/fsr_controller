#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

#define ADS_1_ADDRESS 0x4B
#define ADS_2_ADDRESS 0x4A

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

#define CHANNEL_COUNT 8

// Array that holds the voltage read on each channel (not in order)
float pressure_data[CHANNEL_COUNT];

// the index of the map holds the index of that index in the pressure_data array
uint8_t pressure_data_map[] = {6, 5, 7, 4, 3, 2, 1, 0};

// Max to linearize to for upper and lower stimulation
#define UPPER_LIN_RISE 2.3
#define LOWER_LIN_RISE 1.6

unsigned long prevtime;
// In microseconds
#define UPDATE_DELAY    20000

float get_fsr_channel(uint8_t fsr_channel);
void setup(void)
{
  Serial.begin(115200);

  if (!ads1.begin(ADS_1_ADDRESS, &Wire)) {
    Serial.println("Failed to initialize ADS1. ");
    while (1);
  }

  if (!ads2.begin(ADS_2_ADDRESS, &Wire)) {
    Serial.println("Failed to initialize ADS2. ");
    while (1);
  }

  prevtime = micros();

}

void loop(void)
{

  for (int x = 0; x < CHANNEL_COUNT; x++) {
    if (x < 4)
      pressure_data[x] = ads1.computeVolts(ads1.readADC_SingleEnded(x % 4));
    else
      pressure_data[x] = ads2.computeVolts(ads2.readADC_SingleEnded(x % 4));
    Serial.print("CHAN" + String(x) + ": " + pressure_data[x]);
  }
  Serial.println();

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
float get_fsr_channel(uint8_t fsr_channel) {
  return pressure_data[pressure_data_map[fsr_channel - 1]];
}




// #include <Arduino.h>

// #include <SPI.h>
// #include <Adafruit_ADS1X15.h>
// #include <Wire.h>

// #define ADS_1_ADDRESS 0x4B
// #define ADS_2_ADDRESS 0x4A

// Adafruit_ADS1115 ads1;
// Adafruit_ADS1115 ads2;

// #define CHANNEL_COUNT 8
// float pressure_data[CHANNEL_COUNT]; // Array to hold pressure data
// #define SLAVE_ADDRESS 0x04 // Define the Arduino's I2C slave address

// void setup() {
//   Serial.begin(115200); // Start the serial communication for debugging
//   Wire.begin(); // Start I2C as a slave
//   // Wire.onRequest(requestEvent); // Register event for master request
//   while(!Serial)
//   {

//   }
//   // Initialize ADS1115 devices
//   if (!ads1.begin()) {
//     Serial.println("Failed to initialize ADS1.");
//     while (1); // Halt if fail to initialize
//   }

//   if (!ads2.begin()) {
//     Serial.println("Failed to initialize ADS2.");
//     while (1); // Halt if fail to initialize
//   }  
  
  
  
// }

// void loop() {
//   // Read from both ADS1115 devices

//   for (int i = 0; i < CHANNEL_COUNT; i++) {
//     if (i < 4) {
//       // Read first four channels from the first ADS1115
//       // pressure_data[i] = ads1.computeVolts(ads1.readADC_SingleEnded(i));
//       pressure_data[i] = 250.0;
//     } else {
//       // Read next four channels from the second ADS1115
//       pressure_data[i] = 250.0;
//     }
//   }
//   Serial.print("help");
//   // Wire.beginTransmission(SLAVE_ADDRESS);
//   // Wire.write((byte*)pressure_data, sizeof(pressure_data));
//   // Wire.endTransmission();
//   delay(20); // Delay for stability
// }

// // This function triggers when the I2C master requests data
// // void requestEvent() {
// //   // Send all pressure data as bytes to the master
// //   Wire.write((byte*)pressure_data, sizeof(pressure_data));
// // }
