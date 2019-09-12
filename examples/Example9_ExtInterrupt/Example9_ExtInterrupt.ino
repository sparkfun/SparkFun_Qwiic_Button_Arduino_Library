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
uint8_t brightness = 100;   //The brightness to set the LED to when interrupt pin is high
                            //Can be any value between 0 (off) and 255 (max)
int interruptPin = 0;
int val;

void setup() {
  Serial.begin(115200);
  Wire.begin(); //Join I2C bus

  //intialize interrupt pin
  pinMode(interruptPin, INPUT);
  
  //check if button will acknowledge over I2C
  if (button.begin() == false){
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Button will acknowledge.");
}

void loop() {
  //check state of interrupt pin
  //interrupt pin is active-low, high-impedance when not triggered, goes low-impedance to ground when triggered
  val = digitalRead(interruptPin);
  Serial.println(val);
  if (val == 0){
    button.LEDon(brightness);
    button.clearInterrupt();
  }
  else if (val == 1){
    button.LEDoff();
  }

  delay(20); //Don't hammer too hard on the I2C bus
}
