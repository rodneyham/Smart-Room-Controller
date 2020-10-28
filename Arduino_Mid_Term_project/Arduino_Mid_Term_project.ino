/* 
 *  Project:  Mid-Term project
 *  Description:  smart classroom
 *  Author:  Rodney Ham
 *  date:  28 Oct 2020 Monday
*/

#include <Encoder.h>

bool buttonState;

const int enc_PinA=2;
const int enc_PinB=3;
Encoder myEnc(enc_PinB,enc_PinA);
const int encoder_sw_button=23;   // encoder switch connected to digital pin 23

void setup() {
Serial.begin (9600);

}

void loop() {
  buttonState=digitalRead(BUTTON_PIN);    //Read the switch Button Position of encoder
    if(buttonState!=lastButton){          //Change pixelState once when Button is pressed
      if(buttonState==true){
        pixelState=!pixelState;
      }
        lastButton=buttonState;
    }

}
