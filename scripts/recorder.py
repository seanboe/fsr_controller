import os
import serial
import serial.tools.list_ports

available_ports = list(serial.tools.list_ports.comports())
for port in available_ports:
  if "Adafruit" in port.description:
    device = serial.Serial(port=port.device, baudrate=115200, timeout=0.1)

print("Got port")

with open('recording2.txt', 'w') as recording:
  try:
    while True:
      input = device.readline().decode('ascii')
      if len(input) > 0:
        recording.write(input)
  except KeyboardInterrupt:
    pass