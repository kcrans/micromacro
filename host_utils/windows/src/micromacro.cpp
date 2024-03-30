#define UNICODE
#define _UNICODE
#include "..\..\..\mm_esp32_firmware\config.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "User32.lib")

WORD keymap[NUM_COLS * NUM_ROWS] = { 
    VK_NUMPAD7, VK_NUMPAD8,    VK_NUMPAD9,
    VK_NUMPAD4, VK_NUMPAD5,    VK_NUMPAD6,
    VK_NUMPAD1, VK_NUMPAD2,    VK_NUMPAD3,
    VK_NUMPAD0, VK_OEM_PERIOD, VK_RETURN
    };

// Setup serial connection
HANDLE setup_serial(LPCWSTR port_name, DWORD baudrate) {
    HANDLE serial_handle = CreateFile(port_name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (serial_handle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port: invlaid handle value" << std::endl;
        return nullptr;
    }

    DCB dcb_serial_params = {0};
    dcb_serial_params.DCBlength = sizeof(dcb_serial_params);

    if (!GetCommState(serial_handle, &dcb_serial_params)) {
        std::cerr << "Error retreiving serial port state" << std::endl;
        CloseHandle(serial_handle);
        return nullptr;
    }

    dcb_serial_params.BaudRate = baudrate;
    dcb_serial_params.ByteSize = 8;
    dcb_serial_params.StopBits = ONESTOPBIT;
    dcb_serial_params.Parity = NOPARITY;

    if (!SetCommState(serial_handle, &dcb_serial_params)) {
        std::cerr << "Error setting the serial port state" << std::endl;
        CloseHandle(serial_handle);
        return nullptr;
    }

    // Connection established, return pointer to serial connection
    // But first we need to clear the buffer:
    PurgeComm(serial_handle, PURGE_RXCLEAR);
    return serial_handle;
}

// Function to read in whatever is in the serial connection's input buffer
bool read_serial(HANDLE serial_handle, char* buffer, DWORD buffer_size) {
    DWORD bytes_read; // number of bytes actually read
    DWORD errors;     // bit mask specifying errors (if any)
    COMSTAT status;   // current status of serial device

    //  Clear device's error flag in order to perform I/O operations        
    ClearCommError(serial_handle, &errors, &status);
    // See how many bytes are avaible in the input buffer
    DWORD bytes_to_read = status.cbInQue;
    
    if (bytes_to_read > 0) {
        if (errors == 0) {
            return ReadFile(serial_handle, buffer, buffer_size, &bytes_read, NULL);
        }
        else if (errors == CE_BREAK) {
            std::cerr << "Error: break condition detected" << std::endl;
        }
        else if (errors == CE_FRAME) {
            std::cerr << "Error: framing error detected" << std::endl;
        }
        else if (errors == CE_OVERRUN) {
            std::cerr << "Error: character-buffer overrun" << std::endl; 
        }
        else if (errors == CE_RXOVER) {
            std::cerr << "Error: input buffer overlow" << std::endl;
        }
        else if (errors == CE_RXPARITY) {
            std::cerr << "Error: parity error" << std::endl;
        }
        return false;
    }
    return false;
}

void key_event(char key_code) {
    WORD keypress = keymap[key_code];
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keypress;
    INPUT inputs[] = {input}; 

    SendInput(1, inputs, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    UINT events_sent = SendInput(1, inputs, sizeof(INPUT));
    if (events_sent == 0) {
        std::cerr << "Error: Keypress not registered" << std::endl;
    }
}

int wmain(int argc, LPWSTR argv[]) {
    
    LPWSTR port_name = L"COM6"; // Default name for serial port
    if (argc == 2) { // Else use the command line arg for the port's name
        port_name = argv[1];
    }

    DWORD baudrate = BAUDRATE;
    HANDLE serial_handle = setup_serial(port_name, baudrate);

    if (serial_handle == nullptr) {
        return 1;
    }

    char buffer[1024];

    while (true) {
        if (read_serial(serial_handle, buffer, 1)) {
            key_event(buffer[0]);
            // *remove for debug info* std::cout << static_cast<int>(buffer[0]) << std::endl;
        }
    }
    return 0;
}
