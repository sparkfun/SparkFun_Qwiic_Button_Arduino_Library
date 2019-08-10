/******************************************************************************
A configurator for changing the I2C address on the Qwiic Button/Switch that walks
the user through finding the address of their button, and then changing it!

Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 30, 2019

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Hardware Connections:
Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
Plug the button into the shield
Print it to the serial monitor at 115200 baud.

Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Qwiic_Button.h>
QwiicButton device;

void setup(){
    Serial.begin(115200);
    Wire.begin(); //Join I2C bus
    Wire.setClock(400000);
    device.begin(DEFAULT_BUTTON_ADDRESS); // Initialize our button! Set to DEFAULT_SWITCH_ADDRESS if you're using a
                                          // switch, or whatever the I2C address of your device is

    //check if button will acknowledge over I2C
    if(device.isConnected()) {
        Serial.println("Device will acknowledge!");
    }

    else {
        Serial.println("Device did not acknowledge! Freezing.");
        while(1);
    }
    Serial.println();
    Serial.println("Enter a new I2C address for the Qwiic Button/Switch to use!");
    Serial.println("Don't use the 0x prefix. For instance, if you wanted to");
    Serial.println("change the address to 0x5B, you would enter 5B and press enter.");
    Serial.println();
    Serial.println("One more thing! Make sure your line ending is set to 'Both NL & CR'");
    Serial.println("in the Serial Monitor.");
    Serial.println();
}

void loop(){
    //check if button is pressed, and tell us if it is!
    if(Serial.available()) {
        uint8_t newAddress = 0;
        String stringBuffer = Serial.readStringUntil('\r');
        char charBuffer[10];
        stringBuffer.toCharArray(charBuffer, 10);
        uint8_t success = sscanf(charBuffer, "%x", &newAddress);

        if(success) {
            if(newAddress > 0x08 && newAddress < 0x77) {
                Serial.println("Character recieved, and device address is valid!");
                Serial.print("Attempting to set device address to 0x");
                Serial.println(newAddress, HEX);

                if(device.setI2Caddress(newAddress) == 0) {
                    Serial.println("Device address set succeeded!");
                }

                else {
                    Serial.println("Device address set failed!");
                }

                delay(100); //give the hardware time to do whatever configuration it needs to do\

                if(device.isConnected()) {
                    Serial.println("Device will acknowledge on new I2C address!");
                }

                else {
                    Serial.println("Device will not acknowledge on new I2C address.");
                }
            }

            else {
                Serial.println("Address out of range! Try an adress between 0x08 and 0x77");
            }
        }

        else {
            Serial.print("Invalid Text! Try again.");
        }
        
    }
}