import pygame
import numpy as np
from PIL import Image  
import time
import serial
import serial.tools.list_ports

insole_image_path = 'images/insole.png'
recording_image_path = 'recording1.txt'

def lerp_between_colors(min, max, scale):
  new_r = int(scale * (max.r - min.r) + min.r)
  new_g = int(scale * (max.g - min.g) + min.g)
  new_b = int(scale * (max.b - min.b) + min.b)

  return np.array([new_r, new_g, new_b, 255]).clip(0, 255)

def get_scalemap(insole, fsr_position, fsr_weight):
  scale_map = np.zeros(insole.shape[0 : 2])
  scale_map = scale_map.astype('float32')

  (x, y) = np.ogrid[0 : insole.shape[0], 0 : insole.shape[1]]
  distances = np.sqrt((x - fsr_position[0]) ** 2 + (y - fsr_position[1]) ** 2)

  scale_map = np.where(distances < 150, fsr_weight * np.cos(distances / 100), 0)    
  np.clip(scale_map, 0, 255)

  return scale_map

def get_heatmap(insole_raw, fsr_positions, fsr_weights):
  heatmap = np.zeros(insole_raw.shape)
  for fsr_position, fsr_weight in zip(fsr_positions, fsr_weights):
    heatmap[:, :, 3] += get_scalemap(insole_raw, fsr_position, fsr_weight)
  heatmap[:, :, 3] = heatmap[:, :, 3] / np.max(heatmap[:, :, 3])

  min = pygame.Color(55, 52, 235)
  max = pygame.Color(235, 52, 52)

  heatmap[:, :, 0] = np.where(insole_raw[:, :, 0] < 50, heatmap[:, :, 3] * (max.r - min.r) + min.r, 255)
  heatmap[:, :, 1] = np.where(insole_raw[:, :, 0] < 50, heatmap[:, :, 3] * (max.g - min.g) + min.g, 255)
  heatmap[:, :, 2] = np.where(insole_raw[:, :, 0] < 50, heatmap[:, :, 3] * (max.b - min.b) + min.b, 255)
  heatmap[:, :, 3] = np.ones(heatmap.shape[0:2]) * 255

  return heatmap


def main():

  insole_raw = np.array(Image.open(insole_image_path).transpose(Image.TRANSPOSE))
  print(insole_raw.shape)

  fsr_positions = np.array([(255, 85), (168, 135), (304, 180), (220, 228), (130, 283), (152, 417), (274, 633), (195, 633)])

  # with open(recording_image_path, 'r') as recording:
  #   processed_recording = recording.read().splitlines()
  # for index, line in enumerate(processed_recording):
  #   processed_recording[index] = line.split(', ')[:-1]
  # processed_recording = np.array(processed_recording).astype(float)
  # processed_recording = processed_recording / np.max(processed_recording)

  available_ports = list(serial.tools.list_ports.comports())
  for port in available_ports:
    if "Adafruit" in port.description:
      device = serial.Serial(port=port.device, baudrate=115200, timeout=0.1)
      print("Got device")

  pygame.init()

  screen = pygame.display.set_mode(insole_raw.shape[0 : 2])

  # image = pygame.image.load(insole_image_path).convert()

  pygame.display.update()

  stopevents = pygame.QUIT, pygame.KEYDOWN

  frame = 200
  while(True):

    for e in pygame.event.get():
      if e.type in stopevents:
          return

      elif e.type == pygame.MOUSEBUTTONDOWN:
        position = pygame.mouse.get_pos()
        print(position)
        pygame.draw.circle(screen, (90, 170, 30), position, 20)
        pygame.display.update()

    device_input = device.readline().decode('ascii')
    print(device_input)
    device_input = np.array(device_input.split(',')[:-1]).astype('float32')
    print(device_input)

    screen.fill((255, 255, 255))
    initial = time.time() * 1000
    heatmap = get_heatmap(insole_raw, fsr_positions, device_input)
    surface = pygame.surfarray.make_surface(heatmap[:, :, : 3])
    while time.time() * 1000 - initial < 50:
      pass
    screen.blit(surface, (0, 0))
    pygame.display.update()


    frame += 1

if __name__ == "__main__":
  main()
  pygame.quit() 
