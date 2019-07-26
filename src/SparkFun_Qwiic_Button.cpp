/******************************************************************************
SparkFun_Qwiic_Button.cpp
SparkFun Qwiic Button/Switch Library Source File
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019
https://github.com/sparkfunX/

This file implements the QwiicButton class, prototyped in SparkFun_Qwiic_Button.h

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic Button Version: 1.0.0
    Qwiic Switch Version: 1.0.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h>
#include <SparkFun_Qwiic_Button.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*-------------------------------- Device Status ------------------------*/

bool QwiicButton::begin(uint8_t address, TwoWire &wirePort){
    _deviceAddress = address; //grab the address that the sensor is on
    _i2cPort = &wirePort; //grab the port that the user wants to use
    
    //return true if the device is connected and the device ID is what we expect
    bool success = isConnected();
    success &= checkDeviceID();
    return success;
}

bool QwiicButton::isConnected(){
    _i2cPort->beginTransmission(_deviceAddress);
    return (_i2cPort->endTransmission() == 0);
}

uint8_t QwiicButton::deviceID(){
    return readSingleRegister(ID); //read and return the value in the ID register
}

bool QwiicButton::checkDeviceID(){
    return ( (deviceID() == DEV_ID_SW) || (deviceID() == DEV_ID_BTN) ); //Return true if the device ID matches either the button or the switch
}

uint8_t QwiicButton::getDeviceType(){
    if(isConnected()){ //only try to get the device ID if the device will acknowledge
        uint8_t id = deviceID();
        if(id == DEV_ID_BTN) return 1; //
        if(id == DEV_ID_SW) return 2;
    }
    return 0;
}

uint16_t QwiicButton::getFirmwareVersion(){
    uint16_t version = (readSingleRegister(FIRMWARE_MAJOR)) << 8;
    version |= readSingleRegister(FIRMWARE_MINOR);
    return version;
}


/*------------------------------ Button Status ---------------------- */
bool QwiicButton::isPressed(){
    statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(BUTTON_STATUS);
    return statusRegister.isPressed;
}

bool QwiicButton::hasBeenClicked(){
    statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(BUTTON_STATUS);
    return statusRegister.hasBeenClicked;
}


/*---------------------------- LED Configuration -------------------- */
bool QwiicButton::LEDconfig(uint8_t brightness, uint8_t granularity, uint16_t cycleTime, uint16_t offTime){
    bool success = writeSingleRegister(LED_BRIGHTNESS, brightness);
    success &= writeSingleRegister(LED_PULSE_GRANULARITY, granularity);
    success &= writeDoubleRegister(LED_PULSE_CYCLE_TIME, cycleTime);
    success &= writeDoubleRegister(LED_PULSE_OFF_TIME, offTime);
    return success;
}

bool QwiicButton::LEDoff(){
    return LEDconfig(0, 1, 0, 0);
}

bool QwiicButton::LEDon(uint8_t brightness){
    return LEDconfig(brightness, 1, 0, 0);
}


/*------------------------- Internal I2C Abstraction ---------------- */

uint8_t QwiicButton::readSingleRegister(Qwiic_Button_Register reg){
    _i2cPort->beginTransmission(DEV_ADDR);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();
    if(_i2cPort->requestFrom(DEV_ADDR, 1) != 0){
    return _i2cPort->read();     
    }
}

uint16_t QwiicButton::readDoubleRegister(Qwiic_Button_Register reg){
    _i2cPort->beginTransmission(DEV_ADDR);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();

    if(_i2cPort->requestFrom(DEV_ADDR, 2) != 0){
    uint16_t data = _i2cPort->read() << 8;
    data |= _i2cPort->read();
    return data;
    }
}

bool QwiicButton::writeSingleRegister(Qwiic_Button_Register reg, uint8_t data){
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->write(data);
    return (_i2cPort->endTransmission() != 0);
}

bool QwiicButton::writeDoubleRegister(Qwiic_Button_Register reg, uint16_t data){
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->write(lowByte(data));
    _i2cPort->write(highByte(data));
    return (_i2cPort->endTransmission() != 0);
}