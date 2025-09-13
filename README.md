# SmartHeater

Smart electric heater controller for dorm/room use. Reads temperature, applies schedules/limits, and drives a relay or SSR to keep a target range with safety cutoffs.

## Features
- Target temperature with simple hysteresis control
- Safety limits (max runtime, max temperature)
- Scheduled “eco” / “boost” presets
- Serial telemetry for debugging

## Hardware
- MCU: Arduino Nano/Uno
- Temp sensor: DS18B20 / NTC (with proper circuit) or DHT22/BME280
- Output: Relay module or SSR (observe mains safety)
- Power: As required by heater controller hardware

## Software / Stack
- Written in C++ for the Arduino platform.
- Uses the [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library to format sensor data.
- Includes the [Firebase Arduino](https://github.com/FirebaseExtended/firebase-arduino) library for optional cloud data upload.
- The `Pechka.ino` sketch contains the main control logic.

## Build & Run
### Using the Arduino IDE
1. Copy the contents of the `library/` folder into your Arduino libraries directory (typically `~/Documents/Arduino/libraries`).
2. Open `Pechka.ino` in the Arduino IDE.
3. Select the correct board and COM port.
4. Click **Upload** to compile and upload the firmware to your microcontroller.

### Using Arduino CLI
```bash
# Install required libraries
arduino-cli lib install ArduinoJson
# Compile for an Arduino Uno (adjust the FQBN as needed)
arduino-cli compile --fqbn arduino:avr:uno Pechka.ino
# Upload to the connected device
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno Pechka.ino
```

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
