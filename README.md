# Irrigation System Controller

### Overview
This project involves programming an Arduino Uno to act as an irrigation system controller. The source code allows for 18 programmable times with 8 independent zones, 
as well as an automatic mode and an off mode. Also, an Android application is in development to allow for mobile capability using a WiFi module. Currently, the frontend
of the application is under development. 

### Required Software
1. Arduino IDE
2. Android Studio

### Required Hardware
1. Arudino Uno
2. 4 x 4 Keypad
3. 16 x 2 LCD Display
4. Real Time Clock
5. WiFi Serial Transceiver Module - ESP8266 (Not currently integrated)

### Running the Project - Android Application
1. Clone the repository.
2. Open Android Studio.
3. Click on "Open An Existing Android Studio Project".
4. Click on the "SprinklerSystem" Folder, then click OK.
5. You can view each page by preview, or choose to download the application to an android device. 

NOTICE: To download the application to an android device, the device must have developer mode enabled, as well as USB debugging. 

### Running the Project - Arduino Source Code
1. Clone the repository.
2. Open Arudino IDE.
3. Open the file in the "OnboardCode" folder called "firstTest.ino".
4. Make sure the Arduino Uno is connected via USB to flash the program to the device. 

The following shows the pinout of the hardware devices used:
