/******************************************************************************
  Configures the button to raise an interrupt when pressed, and notifies us over
  serial. Also allows us to enable/disable/reset the interrupt too!

  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: July 29, 2019

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
//To keep track of button transitions
int state = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Qwiic button examples");
  Wire.begin(); //Join I2C bus

  //check if button will acknowledge over I2C
  if (button.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  Serial.println("Button acknowledged.");

  button.resetInterruptConfig();      //reset all the interrupt configuration stuff to defaults, so we have a clean slate to work with!
  button.enablePressedInterrupt();    //configure the interrupt to trigger when we press the button. Change to enableClickedInterrupt()
  //to interrupt when the button is clicked!
}

void loop() {
  if (button.interruptTriggered() == true && state == 0) {
    state = 1;
    Serial.println("Interrupt Triggered!");
  }
  else if (button.interruptTriggered() == true && state == 1){
    button.clearInterrupt();
    delay(1);
  }
  else if(button.interruptTriggered() == false && state == 1){
    state = 0;
    Serial.println("Interrupt Reset");
  }
  else {
    delay(1);
  }

  if (Serial.available()) {
    uint8_t recieved = Serial.read();

    if (recieved == 'd' || recieved == 'D') { //if the user has sent either d or D over serial, then disable the interrupt!
      Serial.println("Disabling Interrupt...");
      button.disablePressedInterrupt();
    }

    if (recieved == 'e' || recieved == 'E') { //if the user has sent either e or E over serial, then enable the interrupt!
      Serial.println("Enabling Interrupt...");
      button.enablePressedInterrupt();
    }
  }
  delay(20); //let's not hammer too hard on the I2C bus
}
