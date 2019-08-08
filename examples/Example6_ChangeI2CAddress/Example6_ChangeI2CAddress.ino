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
    Wire.setClock(400000); //Set I2C clock speed to 400kHz

    //check if button will acknowledge over I2C
    if(device.isConnected()){
        Serial.println("Device will acknowledge!");
    }

    else {
        Serial.println("Device did not acknowledge! Freezing.");
        while(1);
    }
}

void loop() {
    //print an introduction
    Serial.println("Howdy stranger! This configurator will help you change");
    Serial.println("The I2C address of your attached Qwiic Button/Switch.");
    Serial.println();
    Serial.println("Also, make sure that your Line Ending in the Serial Monitor");
    Serial.println("is set to 'Both NL & CR'");
    Serial.println();
    Serial.println("To begin, let's scan for a device. Disconnect all other Qwiic or");
    Serial.println("I2C devices from your microcontroller, and then send any character");
    Serial.println("to begin the scan.");

    while(!Serial.available()); //wait for the user to send a character
    while(Serial.available()) Serial.read(); //flush the readbuffer

    Serial.println("Beginning scan...");
    uint8_t address = scanForDevices();
    
    if(address == -1) { //if the function returned with error, freeze
        Serial.println("No devices found! Freezing.");
        while(1);
    }
    
    //if we got to here, it means that we haven't frozen, so we print that we found a device
    Serial.print("Device found at address: 0x");
    Serial.println(address, HEX);

    device.begin(address);    

    //Inform the user that they'll have to pick out a new address
    Serial.println();
    Serial.println("Enter a new I2C address for the Qwiic Button to use (in hex)!");
    Serial.println("Don't use the 0x prefix. For instance, if you wanted to");
    Serial.println("change the address to 0x5B, you would enter 5B and press enter.");

    while(!Serial.available()); //wait until the user sends some characters

    //Read the buffer and parse it for a valid hex address
    String stringBuffer = Serial.readStringUntil('\r');
    char charBuffer[10];
    stringBuffer.toCharArray(charBuffer, 10);
    uint8_t newAddress = 0;
    uint8_t success = sscanf(charBuffer, "%x", &newAddress);

    //if precisely 1 valid hex number was found in the string, begin setting address
    if(success == 1) {
        //check that the address is valid
        if(newAddress > 0x08 && newAddress < 0x77) {
            Serial.println("Character recieved, and device address is valid!");
            Serial.print("Attempting to set device address to: ");
            Serial.println(newAddress, HEX);
            
            device.setI2Caddress(newAddress);
            delay(10); //give the button/switch some time to restart its I2C hardware

            if(device.isConnected()) {
                //Job is done, print that we're finished
                Serial.print("Address successfully changed! Device will respond at: ");
                Serial.println(device.getI2Caddress(), HEX);

                Serial.println();
                Serial.println("Job done, freezing.");
                while(1);
            }

            else {
                Serial.print("Address change was not successfull! Reset and try again.");
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

//returns the I2C address of the connected device, or -1 if there's no device found
int16_t scanForDevices() {
    for(uint8_t addr = 0; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if(Wire.endTransmission() == 0) {
            return addr;
        }
    }
    return -1;
}