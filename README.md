![tiny_keyboard_pic](mm_ai_pic_1.png)
# micromacro
minimal, simple firmware for macropads

## Features:
* n-key rollover
* [debouncing](https://my.eng.utah.edu/~cs5780/debouncing.pdf)
* arbitrary hotkeys and macros
* up to 256 keys
* fast

## Desktop Client Requirements (building from source):

### Python Client:
* Python 3.8+
* pynput
* pySerial

### C++ Windows Client:
* I've tested and built this app with the Microsoft C++ toolset (so the MSVC compiler) and vscode.
* Look [here](https://code.visualstudio.com/docs/cpp/config-msvc) for a tutorial on getting Microsoft Visual C++ setup in vscode.
* Then all you need to do is open a Developer Command Prompt for Visual Studio, go to the root folder for this repo and run code .\host_utils\windows.

## Firmware Requirements (building from source):
* Arduino IDE with Espressif's ESP32 package installed
* Look at [this](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) guide for more details on getting the dev enviroment setup


## Misc. Notes
* On MacOS you will have to give permission to terminal (or whatever you use to run the python script) to "control your computer"  
