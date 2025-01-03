# Smart Home IR Control with ESP32

Control your IR home devices using Alexa and Telegram, powered by an ESP32 microcontroller.

## Description

This project allows you to control various IR home devices such as fans, lights, TVs, and LEDs using voice commands through Alexa and text commands via a Telegram bot. 
The ESP32 acts as virtual devices to the alexa network, and sends commands using IR to the real devices.

## Features

- Voice control using Alexa
- Remote control via Telegram bot
- Both Alexa and Telegram allow scheduling commands
- Support for multiple devices (fan, light, TV, LED)
- Easy setup and configuration

## How to Use

1. **Alexa Voice Commands:**
   - "Alexa, turn on the living room light"
   - "Alexa, set the fan speed to medium"
   - "Alexa, turn off the TV in 30 minutes"

2. **Telegram Bot Commands:**
   - `/off` - Turn off Fan
   - `/low` - Set the fan speed to low
   - `/med` - Set the fan speed to medium
   - `/high` - Set the fan speed to high
   - `/light` - Turn on/off light
   - `/2h` - Turn off Fan after 2 hours
   - `/4h` - Turn off Fan after 4 hours
   - `/8h` - Turn off Fan after 8 hours
   - `/offall` - Turn off the fan and the light
   - `/help` - Get bot usage help
   - `/status` - Get current status of all devices
   - `/restart` - Restart the ESP32-HUB

## Setup Instructions 🛠

1. **Hardware Setup:**
   - Connect your ESP32 to your home network
   - Wire the Infrared LEDs
   - Wire the 2N222 transistor 
   -  resistor to the ESP32

2. **Software Setup:**
   - Clone this repository
   - Open the project in Arduino IDE
   - Install required libraries:
     - ArduinoJson
     - UniversalTelegramBot
     - fauxmo
   - Update `arduino_secrets.h` with your Wi-Fi credentials, Telegram bot token, and Alexa device names

3. **Telegram Bot Setup:**
   - Create a new bot using BotFather on Telegram
   - Copy the bot token and add it to `arduino_secrets.h`
   - Start a chat with your bot and send the `/start` command

4. **Upload the Code:**
   - Downgrade ESP boards to version 2.0.17 (downgrade) due to https://github.com/crankyoldgit/IRremoteESP8266/issues/2039
   - Compile and upload the code to your ESP32 (mine is ESP32 DEV MODULE)
   - Monitor the serial output for debugging information

5. **Alexa Setup:**
   - Open the Alexa app on your smartphone
   - Go to "Add Device" and select "Other"
   - Follow the prompts to discover your ESP32-controlled devices

## To-Do List 📝

- [x] Save multiple Wifi networks and connect to the strongest
- [x] Add Telegram control with commands
- [x] Restrict bot to admin user(s)
- [ ] Add wire diagram and stl file
- [ ] clean code: unused imports/macros, translate comments
- [ ] Store TV IR commands in macros
- [ ] Revert Multi-Wifi to single network for simplicity
- [ ] Use F() to store string in program memory (flash storage) instead of dynamic work memory so there is more free memory left.
- [ ] Replace fauxmo with [Espalexa] (https://nocheprogramacion.com/series/arduino_esp/020_alexa.html#17:28) in order to add more devices? 
- [ ] Unify "fan" and "light" in the same device to overcome alexa's devices limitation (up to 3 devices sharing IP)
- [ ] Add TV brightness and LEDs as a single device?
- [ ] Implement OTA (Over-The-Air) updates
- [ ] Add Telegram debug mode
- [ ] Update Set Telegram commands from code https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/SetMyCommands/SetMyCommands.ino
- [ ] Add ignore case on Telegram commands

## Future

- [ ] Use bluetooth to turn on ceiling LEDs. Reverse engineering needed
- [ ] Add scheduling functionality for automated control

## Troubleshooting 🆘

If you encounter any issues, please check the following:

1. Ensure all wiring is correct and secure
2. Verify that your Wi-Fi credentials are entered correctly
3. Check that your Telegram bot token is valid
4. Make sure your Alexa device is on the same network as the ESP32
5. Check start-up Telegram message
6. Connect device to serial monitor and read logs
7. check ESP32 boards version. Should be version 2.0.17 due to https://github.com/crankyoldgit/IRremoteESP8266/issues/2039

For more detailed troubleshooting, refer to the `TROUBLESHOOTING.md` file in the `/docs` directory.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License 📃

This project is licensed under the MIT License - see the `LICENSE.md` file for details.


Happy smart home controlling! 🏠✨