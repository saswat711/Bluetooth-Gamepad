#In this project we are trying to build a Gamepad which is able to connect with other devices using Bluetooth. The Gamepad is an HID which is mostly use in playing video games in computers, phones, tablets, Xbox, e.t.c. But Gamepad are not just limited to playing video games. We can modify them to have act as a flight/ drone controller or a robot/machine controller. Opportunities are endless if we understand how to operate with Bluetooth especially as a HID.

#Design Constraints:
The gamepad should communicate wirelessly over Bluetooth.
The gamepad should be able to work properly with the solar system game.
It should have as low delay possible like real time system.

The gamepad should have 2 joysticks for directional controls: 
one for controlling player X and Y axis movement.     
one for controlling the player Z axis movements and rotation around it. 

The gamepad should have a left and one right trigger buttons.
The gamepad should have 2 function buttons start and select.
The gamepad should have a left shoulder button and one right shoulder buttons.
The gamepad should have a normal 4 front facing buttons for player actions.

#Hardware Used:   
ESP-32 Soc (Dev Kit): It allows us to communicate with other devices through Bluetooth. It also has high performance cores and uses ultra-low power to operate.
Analog Joysticks: For Player movement controls
Push Button switches: For trigger, shoulder, front faced, function buttons
Wires: For interconnections

# Software Used:   
Arduino IDE, Visual Studio Code
Online Libraries like Bounce, Nimble(Recommend we can also use ESP32 BLE library) for making ESP-32 as HID or minimizing input delay, E.T.C.
Link to Online Libraries Used:
Bounce Library, Nimble Library
![image](https://user-images.githubusercontent.com/79899982/183063474-766cd46b-6e01-4941-a041-665e7a841610.png)

The layout is as follow
![layout of gamepad](https://github.com/saswat711/Bluetooth-Gamepad/layout%20of%20gamepad.jpg?raw=true)

The Header folder contains the header_files/library [a relative link](Gamepad%20Headers) needed to be installed in the IDE. 
The Gamepad_Code [a relative link](GamePad_Code) folder contains the Code will be uploading to the ESP32 Dev-Kit. 

I have also included a schematic of the the circuit [a relative link](Gamepad%20Circuit%20Schematics.png) we need to make for the same.

Although I have used 10 push buttons. The numbers of buttons can be increased based on the need.

The same code can be used to convert the Gamepad to any other controller like for drone, e.t.c.

## Credits
Link to the refrence code inspired form the Learning Dev ESP32 Gamepad [Link to Refernce Code](https://github.com/lemmingDev/ESP32-BLE-Gamepad)

## Notes
This library allows you to make the ESP32 act as a Bluetooth Gamepad and control what it does.  
Relies on [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)

Basically, the mechanical part of buttons and switches vibrate slightly when closed or opened causing multiple undesired false states (similar to noise). This library filters out these undesired state changes. More about the library [Link to the Bounce Library](https://github.com/thomasfredericks/Bounce2)

