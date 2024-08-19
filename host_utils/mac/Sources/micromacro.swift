// import Config 
import Figlet
import ArgumentParser
import ORSSerial

class SerialPortManager: NSObject, ORSSerialPortDelegate {
    var serialPort: ORSSerialPort?
    let keyMap = [
    "7", "8", "9",
    "4", "5", "6",
    "1", "2", "3",
    "0", ".", "enter"]
    // Example keymap

    override init() {
        super.init()
        let portName = "/dev/cu.usbmodem101"  // Change this to your actual serial port name
        self.serialPort = ORSSerialPort(path: portName)
        self.serialPort?.baudRate = 921600
        self.serialPort?.delegate = self
        self.serialPort?.open()
    }
    
    func serialPort(_ serialPort: ORSSerialPort, didReceive data: Data) {
        data.forEach { byte in
            let index = Int(byte)
            if index >= 0 && index < keyMap.count {
                print("Key Pressed: \(keyMap[index])")
            } else {
                print("Received index out of keymap range: \(index)")
            }
        }
    } 

    func serialPortWasRemovedFromSystem(_ serialPort: ORSSerialPort) {
        print("Serial port was removed")
        self.serialPort = nil
    }

    func serialPort(_ serialPort: ORSSerialPort, didEncounterError error: Error) {
        print("Serial port (\(serialPort)) encountered error: \(error)")
    }
}

let manager = SerialPortManager()
RunLoop.main.run()
