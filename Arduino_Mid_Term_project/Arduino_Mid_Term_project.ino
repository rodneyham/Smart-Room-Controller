/* 
 *  Project:  Mid-Term project
 *  Description:  smart classroom
 *  Author:  Rodney Ham
 *  date:  28 Oct 2020 Monday
*/

#include <Encoder.h>

bool buttonState;
bool lastButton_state;

const int enc_PinA=1;
const int enc_PinB=0;
Encoder myEnc(enc_PinB,enc_PinA);
const int enc_sw_button=23;   // encoder switch connected to digital pin 23

void setup() {
Serial.begin (9600);

}

void loop() {
  buttonState=digitalRead(enc_sw_button);    //Read the switch Button Position of encoder
     Serial.print("kdjk");
    if(buttonState!=lastButton_state){       //Change buttonState once when Button is pressed
     
      if(buttonState==true){
        
        //pixelState=!pixelState;
      }
        lastButton_state=buttonState;
   }

}
