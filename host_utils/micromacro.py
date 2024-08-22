import sys
import time
import serial
import sqlite3
import threading
from pynput.keyboard import Key, Controller

keyboard = Controller()

keymap = ['7', '8', '9',
          '4', '5', '6',
          '1', '2', '3',
          '0', '.', Key.enter]

keycounts = {str(key): 0 for key in keymap}

get_key = {bytes(i) : key for i, key in enumerate(keymap)}

baudrate = 921600
if len(sys.argv) >= 2:
    port = sys.argv[1]
if len(sys.argv) == 3:
    baudrate = int(sys.argv[2])
else:
    port = "/dev/cu.usbmodem101" # MacOS-style port
    #port = "COM6"              # Windows-style port

def open_port(max_attempts=3):
    for i in range(max_attempts):
        try:
            ser = serial.Serial(port, baudrate, timeout=1)
            return ser
        
        except serial.SerialException as e:
            print(f'Error opening {port}: {e}')
            time.sleep(2**i) # Exponential backoff
        except ValueError as e:
            print(f'Value error: {e}')
            return None
        except Exception as e:
            print(f'Unexpected error: {e}')
            return None

    return None

def init_db():
    connection = sqlite3.connect('keypresses.db')
    cursor = connection.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS KeyPresses (
            key_id TEXT PRIMARY KEY,
            count INTEGER
            )
        ''')
    connection.commit()
    connection.close()

lock = threading.Lock()

def update_db():
    connection = sqlite3.connect('keypresses.db')
    cursor = connection.cursor()
    
    with lock:
        for key, count in keycounts.items():
            cursor.execute('''
                INSERT INTO KeyPresses (key_id, count) VALUES (?, ?)
                ON CONFLICT (key_id) DO UPDATE SET count = count + 
                EXCLUDED.count;
                ''', (key, count))
    connection.commit()
    connection.close()

init_db()

def auto_save(interval=600): # Save every 10 minutes
    while True:
        update_db()
        time.sleep(interval)

thread = threading.Thread(target=auto_save)
thread.daemon = True
thread.start()

serial_connection = open_port()

if serial_connection is not None:
    while True:
        if ser.in_waiting > 0:
            key_byte = ser.read(1)
            keypress = keymap[key_byte[0]]
            keyboard.press(keypress)
            keyboard.release(keypress)
            with lock:
                keycounts[keypress] += 1
        
