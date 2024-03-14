import serial
from pynput.keyboard import Key, Controller

keyboard = Controller()

keymap = ['7', '8', '9',
          '4', '5', '6',
          '1', '2', '3',
          '0', '.', Key.enter]

get_key = {bytes(i) : key for i, key in enumerate(keymap)}

with serial.Serial('COM6', 9600) as ser:
    while True:
        if ser.in_waiting > 0:
            key_byte = ser.read(1)
            keypress = keymap[key_byte[0]]
            keyboard.press(keypress)
            keyboard.release(keypress)
            
