/******************************************************************************
Prints button status to the serial monitor, using a button attached over Qwiic

Fischer Moseley @ SparkFun Electronics
Original Creation Date: June 28, 2019

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Hardware Connections:
Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
Plug the button into the shield
Print it to the serial monitor at 9600 baud.

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h>
#include <SparkFun_Qwiic_Button.h>

QwiicButton Button;

void setup(){
    Serial.begin(9600);
    Serial.println("SparkFun Qwiic Button Example 1");

    Wire.begin(); //Join i2c bus

    if(Button.isConnected() == false){
        Serial.println("Device not found! Check wiring and try again.");
        while(1);
    }

    else {
        Serial.println("Device found!");
    }
}

void loop(){
    bool state = Button.isPressed();
    if(state == true){
        Serial.println("The button is pressed!");
    }

    else {
        Serial.println("The button is not pressed.");
    }
    
    delay(20);
}