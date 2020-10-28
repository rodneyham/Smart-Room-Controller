/*
 * Project: Wemo
 * Description: Ethernet Text
 * Author: Rodney Ham
 * Date: 20 Oct 2020
*/

//Dynamic Host Configuration protocol 
//(connect to rj45) (get dhcp address) - spoof mac id
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <Encoder.h>
#include <hue.h>        //This MUST be put AFTER #include <Ethernet.h>


bool buttonState;
bool lastButton;
bool HueOn;

int lightNum;   //light in the classroom to light up
int HueColor;   //see hue.h
int HueBright=200;
int maxBrightness=255;
int minBrightness=0;
int buttonSelection;

const int buttonRed=1;
const int buttonGreen=2;
const int buttonYellow=3;
const int buttonBlue=4;
const int buttonBlack=5;

int buttonColorArray[]={buttonRed,buttonGreen,buttonYellow,buttonBlue,buttonBlack};


//Encoder myEnc(encoderPinB,encoderPinA);

EthernetClient client;
bool status;

void setup() {
  pinMode(buttonRed,INPUT);
  pinMode(buttonGreen,INPUT);
  pinMode(buttonYellow,INPUT);
  pinMode(buttonBlue,INPUT);
  pinMode(buttonBlack,INPUT);
  
  HueColor=HueRainbow[5];
  HueBright=0;
  lightNum=5;   //lights 1 to 5 in the classroom
     
  lastButton = false;

  //Open Serial Communication and wait for port to open:
  Serial.begin(9600);
  while(!Serial);  //wait for Serial monitor

  //Start ethernet connection
  status = Ethernet.begin(mac);
    }

void loop() {   
if(digitalRead(buttonRed)){
//  buttonSelection=buttonRed;
//}
  //buttonState=digitalRead(buttonRed);    //Read the PUSH Button Position
  if(buttonState!=lastButton){          //Change buttonState once when Button is pressed
    if(buttonState==true){
        HueOn=!HueOn;
    }
      lastButton=buttonState;
  }
}
// HueBright=myEnc.read();        //read encoder position
//  if(HueBright>maxBrightness){   //if position goes over 255 keep at last position
//      myEnc.write(maxBrightness);
//  }
//  if(HueBright<minBrightness){   //if position goes under 0 keep at last position
//      myEnc.write(minBrightness);
//  }

  if(HueOn){                    //Turn on the light when HueOn=true
    for(int i=0;i<7;i++){
        setHue(lightNum,HueOn,HueRainbow[i],HueBright);
    }
     setHue(lightNum,HueOn,HueRed,HueBright);
  }
  else{
        setHue(lightNum,HueOn,0,0);   //Turn off the light when HueOn=false
        delay(100);
  } 
 
}
