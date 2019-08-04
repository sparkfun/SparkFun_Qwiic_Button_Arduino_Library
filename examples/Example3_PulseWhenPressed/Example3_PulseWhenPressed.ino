/******************************************************************************
Checks whether the button is pressed, and light it up if it is! Also prints
status to the serial monitor.

Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019

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
uint8_t brightness = 170;   //The maximum brightness of the pulsing LED. Can be between 0 (min) and 255 (max)
uint16_t cycleTime = 500;   //The total time for the pulse to take. Set to a bigger number for a slower pulse, or a smaller number for a faster pulse
uint16_t offTime = 500;     //The total time to stay off between pulses. Set to 0 to be pulsing continuously.


void setup(){
    Serial.begin(115200);
    Wire.begin(); //Join I2C bus
    Wire.setClock(400000); //Set I2C clock to 400kHz
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

    button.LEDoff();
}

void loop(){
    //check if button is pressed, and tell us if it is!
    if(button.isPressed()){
        Serial.println("The button is pressed!");
        button.LEDconfig(brightness, 1, cycleTime, offTime);
    }

    else {
        Serial.println("The button is not pressed.");
        button.LEDoff();
    }
    
    delay(20); //let's not hammer too hard on the I2C bus
}