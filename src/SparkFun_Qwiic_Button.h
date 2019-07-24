/******************************************************************************
SparkFun_Qwiic_Button.h
SparkFun Qwiic Button Header File
Fischer Moseley @ SparkFun Electronics
Original Creation Date: June 28, 2019
https://github.com/sparkfunX/

This file prototypes the QwiicSwitch class as implemented in SparkFunQwiicSwitch.cpp

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	Qwiic Button Version: 1.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __SparkFun_Qwiic_Button_H__
#define __SparkFun_Qwiic_Button_H__

#include <Wire.h>
#include <Arduino.h>

#define DEFAULT_DEVICE_ADDR 0x5F

//Qwiic Button Register Map
enum QwiicButton_Register {
  BUTTON_ID = 0x00,
  BUTTON_STATUS = 0x01,
  BUTTON_FIRMWARE = 0x02, //16-bit register where 0x02 is the MSB and 0x03 is the LSB
  BUTTON_INTERRUPT_ENABLE = 0x04,
  BUTTON_TIME_SINCE_OLDEST_PRESS = 0x05,
  BUTTON_TIME_SINCE_OLDEST_CLICK = 0x07,
  BUTTON_LED_BRIGHTNESS = 0x09,
  BUTTON_LED_PULSE_GRANULATITY = 0x0A,
  BUTTON_LED_PULSE_CYCLE_TIME = 0x0B,
  BUTTON_LED_PULSE_OFF_TIME = 0x0D,
  BUTTON_DEBOUNCE_TIME = 0x0F,
  BUTTON_I2C_ADDR = 0x10,
};

//Locations of Various Bits in the Status Register
const byte statusButtonPressedBufferEmptyBit = 5;
const byte statusButtonPressedBufferFullBit = 4;
const byte statusButtonClickedBufferEmptyBit = 3;
const byte statusButtonClickedBufferFullBit = 2;
const byte statusButtonClickedBit = 1;
const byte statusButtonPressedBit = 0;

const byte enableInterruptButtonClickedBit = 1;
const byte enableInterruptButtonPressedBit = 0;

class QwiicButton {

public: 
    //Status and Configuration functions
    bool begin(uint8_t address = DEFAULT_DEVICE_ADDR, 
        TwoWire &wirePort = Wire);
    uint8_t getAddress();                                   //Returns the device's I2C address
    uint8_t setAddress();                                   //NEED TO IMPLEMENT!!
    bool isConnected();                                     //Returns true if the device will acknowledge over I2C
    uint8_t getDeviceID();                                  //Returns the button's Device IDE
    uint16_t getFirmwareVersion();                          //Returns the version of firmware running on the button
    uint8_t getDebounceTime();                              //Returns the value of the time that the button wait for debouncing (in ms)
    uint8_t setDebounceTime(uint8_t time);                  //Sets how long to wait for debouncing (in ms)
    uint8_t configureLED(uint8_t brightness, uint8_t 
        granularity, uint16_t cycleTime, uint16_t offTime); //Configures the LED built into the button
    bool isPressed();                                       //Returns true if the button is pressed, false otherwise
    bool isClicked();                                       //Returns true if the button is clicked, false otherwise
    
    bool isPressedBufferEmpty();                            //Returns true if the ButtonPressed buffer is empty, false otherwise
    bool isPressedBufferFull();                             //Returns true if the ButtonPressed buffer is full, false otherwise 
    bool isClickedBufferEmpty();                            //Returns true if the ButtonClicked buffer is empty, false otherwise
    bool isClickedBufferFull();                             //Returns true if the ButtonClicked buffer is full, false otherwise

    //Queue Manipulation functions
    uint16_t getOldestButtonPress();                        //Returns the amount of time that has passed since the oldest button press (in ms)
    uint16_t getOldestButtonClick();                        //Returns the amount of time that has passed since the oldest button click (in ms)
    uint8_t popButtonPressedQueue();                        //Clears the oldest entry from the queue of button press event times
    uint8_t popButtonClickedQueue();                        //Clears the oldest entry from the queue of button click event times
    void wipeButtonPressedQueue();                          //Clears the queue of button press event times 
    void wipeButtonClickedQueue();                          //Clears the queue of button click event times



private: 
    TwoWire *_i2cPort = NULL; //Generic connection to user's chosen I2C Hardware
    uint8_t _deviceAddress;

    //Direct Register Read/Write functions
    uint8_t readsingleRegister(QwiicButton_Register reg);   //Reads a 8-bit register from the Qwiic Button
    uint16_t readDoubleRegister(QwiicButton_Register reg);  //Reads a 16-bit register from the Qwiic Button (little endian)
    uint8_t writeSingleRegister(QwiicButton_Register reg);  //Write a byte to a single 8-bit register
    uint8_t writeDoubleRegister(QwiicButton_Register reg);  //Write two bytes to a 16-bit register (little endian)
}