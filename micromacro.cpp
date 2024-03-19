#include <Windows.h>
#include <iostream>

WORD keymap[12] = {VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,
          VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6,
          VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3,
          VK_NUMPAD0, VK_OEM_PERIOD, VK_RETURN};

// Setup serial connection
HANDLE setup_serial(LPCSTR port_name, DWORD baudrate) {
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
    
    if (errors == 0 && bytes_to_read > 0) {
        if (ReadFile(serial_handle, buffer, buffer_size, &bytes_read, NULL)) {
            return true;
        }
    }
    return false;
}

void key_event(char key_code) {
    WORD keypress = keymap[key_code];
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keypress;

    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

int main(void) {
    LPCSTR port_name = "COM6";
    DWORD baudrate = CBR_9600;
    HANDLE serial_handle = setup_serial(port_name, baudrate);

    if (serial_handle == nullptr) {
        return 1;
    }

    char buffer[1024];

    while (true) {
        if (read_serial(serial_handle, buffer, 1)) {
            key_event(buffer[0]);
            //std::cout << static_cast<int>(buffer[0]) << std::endl;
        }
    }

    return 0;
}