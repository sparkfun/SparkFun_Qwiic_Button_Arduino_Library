/******************************************************************************
SparkFun_Qwiic_Button.cpp
SparkFun Qwiic Button Source File
Fischer Moseley @ SparkFun Electronics
Original Creation Date: June 28, 2019
https://github.com/sparkfunX/

This file implements all functions of the QwiccButton class. Functions for reading
button status, manipulating the event queue, configuring interrupts, and controlling
the built-in LED are defined here.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	Qwiic Button Version: 1.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Qwiic_Button.h>


//Constructs a QwiicButton object with user-specified I2C address and port
QwiicButton::QwiicButton(TwoWire &wirePort, uint8_t addr){
    _i2cPort = wirePort; //use an arbitrary I2C port. Set to use Wire by default in the header file 
    _deviceAddress = addr; //I2C address of the button
}

//Returns the I2C address of the QwiicButton
uint8_t QwiicButton::getAddress(){
	return _deviceAddress;
}

//Sets the I2C address of the QwiicButton
uint8_t QwiicButton::setAddress(){
	//NEED TO IMPLEMENT!
}

//Returns true if the device will acknowledge over I2C
bool QwiicButton::isConnected(){
  _i2cPort->beginTransmission(_deviceAddress);
  if(_i2cPort->endTransmission()!=0){
    return false;
  }
  return true;
}

//Returns the button's Device ID
uint8_t QwiicButton::getDeviceID(){
  return readSingleRegister(BUTTON_ID);
}

//Returns the version of firmware running on the button
uint16_t QwiicButton::getFirmwareVersion(){
  return readDoubleRegister(BUTTON_FIRMWARE);
}

//Returns the value of the time that the button waits for debouncing (in ms)
uint8_t QwiicButton::getDebounceTime(){
  return readSingleRegister(BUTTON_DEBOUNCE_TIME);
}

//Sets how long to wait for debouncing (in ms)
uint8_t QwiicButton::setDebounceTime(uint8_t time){
  return writeSingleRegister(BUTTON_DEBOUNCE_TIME, uint8_t(time));
}

//Configures the LED built into the button
uint8_t QwiicButton::configureLED(uint8_t brightness, uint8_t granularity, uint16_t cycleTime, uint16_t offTime){
  writeSingleRegister(BUTTON_LED_BRIGHTNESS, brightness);
  writeSingleRegister(BUTTON_LED_PULSE_GRANULATITY, granularity);
  writeDoubleRegister(BUTTON_LED_PULSE_CYCLE_TIME, cycleTime);
  writeDoubleRegister(BUTTON_LED_PULSE_OFF_TIME, offTime); 
}

//Returns true if the button is pressed, false otherwise
bool QwiicButton::isPressed(){
  return bitRead(readSingleRegister(BUTTON_STATUS), statusButtonPressedBit);
}

//Returns true if the button has been clicked, false otherwise
bool QwiicButton::isClicked(){
  return bitRead(readSingleRegister(BUTTON_STATUS), statusButtonClickedBit);
}

//Returns true if the ButtonPressed buffer is empty, false otherwise
bool QwiicButton::isPressedBufferEmpty(){
  return bitRead(readSingleRegister(BUTTON_STATUS), statusButtonPressedBufferEmptyBit);
}

//Returns true if the ButtonPressed buffer is full, false otherwise
bool QwiicButton::isPressedBufferFull(){
  return bitRead(readSingleRegister(BUTTON_STATUS), statusButtonPressedBufferFullBit);
}

//Returns true if the ButtonClicked buffer is empty, false otherwise
bool QwiicButton::isClickedBufferEmpty(){
  return bitRead(readSingleRegister(BUTTON_STATUS), statusButtonClickedBufferEmptyBit);
}

//Returns true if the ButtonClicked buffer is full, false otherwise
bool QwiicButton::isClickedBufferFull(){
  return bitRead(readSingleRegister(BUTTON_STATUS), statusButtonClickedBufferFullBit);
}


/*------------------- Queue Manipulation Functions -------------------- */

uint16_t QwiicButton::getOldestButtonPress(){ //returns the amount of time that has passed since the oldest button press (in ms)
  return readDoubleRegister(BUTTON_TIME_SINCE_OLDEST_PRESS);
}

uint16_t QwiicButton::getOldestButtonClick(){ //returns the amount of time that has passed since the oldest button click (in ms)
  return readDoubleRegister(BUTTON_TIME_SINCE_OLDEST_CLICK);
}

uint8_t QwiicButton::popButtonPressedQueue(){ //clears the oldest entry from the queue of button press event times
  return writeDoubleRegister(BUTTON_TIME_SINCE_OLDEST_PRESS, 0x0000);
}

uint16_t QwiicButton::popButtonClickedQueue(){ //clears the oldest entry from the queue of button click event times
  return writeDoubleRegister(BUTTON_TIME_SINCE_OLDEST_CLICK, 0x0000);
}

void QwiicButton::wipeButtonPressedQueue(){ //clears the queue of button press event times
  while(!isPressedBufferEmpty()){
    popButtonPressedQueue();
  }
}

void QwiicButton::wipeButtonClickedQueue(){ //clears the queue of button click event times
  while(!isClickedBufferEmpty()){
    popButtonClickedQueue();
  }
}


/*---------------- Direct Register Read/Write ------------------ */

//Reads a 8-bit register from the Qwiic Button
uint8_t QwiicButton::readSingleRegister(QwiicButton_Register reg){ 
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(uint8_t(reg));
  _i2cPort->endTransmission();
  _i2cPort->requestFrom(_deviceAddress, 1);
  if(_i2cPort->available()){
    return _i2cPort->read();
  }
  else{
    return 0;
  }
}

//Reads a 16-bit register from the Qwiic Button (little endian)
uint16_t QwiicButton::readDoubleRegister(QwiicButton_Register reg){ 
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(uint8_t(reg));
  _i2cPort->endTransmission();
  _i2cPort->requestFrom(_deviceAddress, 2);

  if(_i2cPort->available()){
    uint16_t data = _i2cPort->read();
		data |= _i2cPort->read() << 8;
		return data;
  }
  
  else {
    return 0;
  }

}

//Write a byte to a single 8-bit register
uint8_t QwiicButton::writeSingleRegister(QwiicButton_Register reg, uint8_t data){
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->write(data);
  return _i2cPort->endTransmission();
}

//Write two bytes to a 16-bit register (little endian)
uint8_t QwiicButton::writeDoubleRegister(QwiicButton_Register reg, uint16_t data){ 
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->write(lowByte(data));
  _i2cPort->write(highByte(data));
  return _i2cPort->endTransmission();
}