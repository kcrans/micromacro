![tiny_keyboard_pic](mm_ai_pic_1.png)
# micromacro
minimal, simple firmware for macropads

## Features:
* n-key rollover
* debouncing
* arbitrary hotkeys and macros
* up to 256 keys
* fast

## Desktop Client Requirements (building from source):
### Python Client:
* Python 3.8+
* pynput
* pySerial
### C++ Windows Client:
* A C++ compiler
* I've tested and built it with the Microsoft C++ toolset and vscode

## Firmware Requirements (building from source):
* Arduino IDE with Espressif's ESP32 package installed
* Look at [this](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) guide for more details on getting the dev enviroment setup


## Misc. Notes
* On MacOS you will have to give permission to terminal (or whatever you use to run the python script) to "control your computer"  
