## This repo contains the code for making a ESP32 as a BLE Gamepad

I have used Arduino IDE while programming and uploading the code to the board and Visual studio for the header files development.
I am using a 30 GPIOs ESP32 DevKit and 2 * 2-axis Joystick and 10 Push Buttons.

The Header folder contains the header_files/library needed to be installed in the IDE.
The Gamepad_Code folder contains the Code will be uploading to the ESP32 Dev-Kit.

## Credits
Link to the refrence code inspired form the Learning Dev ESP32 Gamepad [Link to Refernce Code](https://github.com/lemmingDev/ESP32-BLE-Gamepad)

## Notes
This library allows you to make the ESP32 act as a Bluetooth Gamepad and control what it does.  
Relies on [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)

Basically, the mechanical part of buttons and switches vibrate slightly when closed or opened causing multiple undesired false states (similar to noise). This library filters out these undesired state changes. More about the library [Link to the Bounce Library](https://github.com/thomasfredericks/Bounce2)

