/******************************************************************************
Configures the button to raise an interrupt when pressed, and notifies us over
serial. Also allows us to enable/disable/reset the interrupt too!

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
}

void loop(){
    if(button.isPressedQueueEmpty() == false) {
        Serial.print(button.timeSinceLastPress()/1000.0);
        Serial.print("s since the button was last pressed   ");
        Serial.print(button.timeSinceFirstPress()/1000.0);
        Serial.print("s since the button was first pressed   ");
    }
    
    if(button.isPressedQueueEmpty() == true) Serial.print("ButtonPressed Queue is empty! ");

    if(button.isClickedQueueEmpty() == false){
        Serial.print(button.timeSinceLastClick()/1000.0);
        Serial.print("s since the button was last clicked   ");
        Serial.print(button.timeSinceFirstClick()/1000.0);
        Serial.print("s since the button was first clicked");
    }

    if(button.isPressedQueueEmpty() == true) Serial.print("  ButtonClicked Queue is empty!");
    Serial.println();

    if(Serial.available()) {
        
        uint8_t data = Serial.read();
        if(data == 'p' || data == 'P') {
            button.popPressedQueue();
            Serial.println("Popped PressedQueue!");
        }

        if(data == 'c' || data == 'C') {
            button.popClickedQueue();
            Serial.println("Popped ClickedQueue!");
        }
    }
    delay(20); //let's not hammer too hard on the I2C bus
}