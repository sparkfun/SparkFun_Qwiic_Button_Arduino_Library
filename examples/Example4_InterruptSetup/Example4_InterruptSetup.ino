/******************************************************************************
Configures the button to raise an interrupt when pressed, and notifies us over
serial.

Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 29, 2019

This code is beerware; if you see me (or any other SparkFun employee) at the
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
    Wire.setClock(400000);
    button.begin();

    //check if button will acknowledge over I2C
    if(button.isConnected()){
        Serial.println("Device will acknowledge!");
    }

    else {
        Serial.println("Device did not acknowledge! Freezing.");
        while(1);
    }
    button.resetInterruptConfig();
    button.enableClickedInterrupt();
    button.LEDon();
    delay(200);
    button.LEDoff();
}

void loop(){
    if(button.interruptTriggered()){
        Serial.println("Interrupt Triggered!");
    }

    else {
        Serial.println("interrupt not triggered");
    }

    if(Serial.available()){
        uint8_t recieved = Serial.read();
        if(recieved == 'r' || recieved == 'R') {
            Serial.println("resetting interrupt");
            button.clearInterrupt();
        }
        if(recieved == 'd' || recieved == 'D') {
            Serial.println("disabling interrupt");
            button.disableClickedInterrupt();
        }

        if(recieved == 'e' || recieved == 'E') {
            Serial.println("enabling interrupt");
            button.enableClickedInterrupt();
        }
    }
    delay(20); //let's not hammer too hard on the I2C bus
}