/******************************************************************************
Checks whether the button is pressed, and then prints its status over serial!

Fischer Moseley @ SparkFun Electronics
Original Creation Date: June 28, 2019

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Hardware Connections:
Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
Plug the button into the shield
Print it to the serial monitor at 115200 baud.

Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Qwiic_Button.h>
QwiicButton button;

void setup(){
    Serial.begin(115200);
    Wire.begin(); //Join I2C bus
    Wire.setClock(400000); //Set I2C clock speed to 400kHz
    button.begin(DEFAULT_BUTTON_ADDRESS); // Initialize our button! Set to DEFAULT_SWITCH_ADDRESS if you're using a
                                          // switch, or whatever the I2C address of your device is

    //check if button will acknowledge over I2C
    if(button.isConnected()){
        Serial.println("Device will acknowledge!");
    }

    else {
        Serial.println("Device did not acknowledge! Freezing.");
        while(1);
    }
}

void loop(){
    //check if button is pressed, and tell us if it is!
    if(button.isPressed()){
        Serial.println("The button is pressed!");
    }

    else {
        Serial.println("The button is not pressed.");
    }
    
    delay(20); //Don't hammer too hard on the I2C bus
}