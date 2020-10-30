/* 
 *  Project:  Mid-Term project
 *  Description:  smart classroom
 *  Author:  Rodney Ham
 *  date:  28 Oct 2020 Monday
*/

#include <Encoder.h>
#include <Ethernet.h>
#include <mac.h>
#include <SPI.h>
#include <Wire.h>
#include <wemo.h>
#include <hue.h>            //This MUST be put AFTER #include <Ethernet.h>
#include <OneButton.h>      //for oneButton_Array()
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_BME280 bme;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int ledRed_pin=5;
const int ledClear_pin=4;
const int ledBlue_pin=3;
const int ledYellow_pin=2;

//Ethernet
  EthernetClient client;
  bool status;

//Encoder
int encPosition;   //for encoder position
bool buttonState;
bool lastButton_state;
const int enc_PinA=1;
const int enc_PinB=0;
const int encoderGreen=22;
const int encoderRed=21;
const int enc_sw_button=23;     // encoder switch connected to digital pin 23)
Encoder myEnc(enc_PinB,enc_PinA);

//Hue
bool HueOn;
int lightNum;   //light in the classroom to light up
int HueColor;   //see hue.h
int HueBright;  //see hue.h
int maxBrightness=255;
int minBrightness=0;

//Wemo
bool wemoON;
bool wemo_buttonState;
bool wemo_lastButton;
bool wemo_is_on;

//Ultrasonic Sensor
const int echoPin=7;        // attach digital pin Echo of HC-SR04
const int trigPin=8;        //attach digital pin Trig of HC-SR04
const int buttonRed=6;      //press red button to turn on ultrasonic sensor
int val_buttonRed;
long duration;    // variable for the duration of sound wave travel
int distance;     // variable for the distance measurement


//oneButton_Array
int mode;
//OneButton the_red_button(buttonRed,false,false);
OneButton encoder_sw(enc_sw_button,false,false);
//oneButton_Array()

int tempRange;

float tempC;
float tempC_F;

//This is for the BME280 humidity, temperature & pressure sensor
//to interface the device's I2C address is either 0x76 or 0x77.

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     14 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  
void setup() {
  Serial.begin (9600);
  delay(100);         // wait for Serial Monitor to Open
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  Serial.println("Ultrasonic Sensor HC-SR04 Test");

  status=bme.begin(0x76);
    if(status==false){
    Serial.print("initialization failed");
    }

  //oneButton_array
  mode=4;
  //the_red_button.attachClick(sonarLoop);
  encoder_sw.attachClick(oneButton_Array);
  
  
  //Ethernet
 pinMode(10, OUTPUT);
 digitalWrite(10,HIGH);

  //Hue
  pinMode(encoderGreen, OUTPUT);
  pinMode(encoderRed, OUTPUT);
  HueColor=HueRainbow[5];
  HueBright=0;
  lightNum=5;   //lights 1 to 5 in the classroom

  //wemo
  wemo_lastButton=false;
  pinMode(enc_sw_button,INPUT);     //used for wemo, Hue
  
  //Start ethernet connection
  status = Ethernet.begin(mac);
  if (!status) {
    Serial.println("failed to configure Ethernet using DHCP");
    //no point in continueing 
    while(1);
    }
  //print your local IP address
  Serial.print("My IP address:");
  for (byte thisbyte = 0; thisbyte < 4; thisbyte++) {
    //print value of each byte of the IP address
    Serial.print(Ethernet.localIP()[thisbyte], DEC);
    if (thisbyte < 3) Serial.print(".");
    }
  Serial.println();

//these have to deal with temperature
  pinMode(ledRed_pin,OUTPUT);     //set LED pin to Output
  pinMode(ledClear_pin,OUTPUT);
  pinMode(ledBlue_pin,OUTPUT);


  //Ultrasonic Sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(buttonRed,INPUT); //when button is pressed the pin goes low
  pinMode(ledYellow_pin,OUTPUT);    //light LED when sensor detects something
}

void loop() {
  switch (mode){
    case 0:
      sonarLoop();
      break;
    case 1:
      tempLoop();
      break;
    case 2:
      wemoLoop();
      break;
    case 3:
      wemoLoop();    //this mode is no longer to 2 so it will shut them off
      hueLoop();
      break; 
    case 4:  
      break;    
  }
  //the_red_button.tick();   //check the state of the button
  encoder_sw.tick();   //check the state of the encoder switch button
  
  //tempLoop();
  //wemoLoop();
 //sonarLoop();
  //hueLoop();
}

void oneButton_Array(){
  Serial.printf ("mode is click no. %i \n", mode);
  if(mode>=4){
    mode=0;
  }
    else {
      mode++;
    }
 }
void tempLoop() {
  Serial.printf("temp mode");
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setRotation(0);             //direction screen shows characters
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  
  tempC=bme.readTemperature();
  tempC_F=(tempC*9.0/5+32);
  Serial.printf("The current temperture is %0.2f \n",tempC_F);
    if(tempC_F<=65.0){
      digitalWrite(ledBlue_pin,HIGH);
      digitalWrite(ledRed_pin,LOW);
      digitalWrite(ledClear_pin,LOW);
      Serial.printf("Weather is cold at %0.2f \n",tempC_F);
      //display.printf("Weather is cold at %0.2f \n",tempC_F);
      display.printf("Weather is cold at %0.2f \n",tempC_F);
    }
    else if(tempC_F>=80.0){
      digitalWrite(ledRed_pin,HIGH);
      digitalWrite(ledBlue_pin,LOW);
      digitalWrite(ledClear_pin,LOW);
      display.printf("Weather is HOT at %0.2f \n",tempC_F);        
    }
    else{
      digitalWrite(ledClear_pin,HIGH);
      digitalWrite(ledRed_pin,LOW);
      digitalWrite(ledBlue_pin,LOW);
      display.printf("Nice weather at %0.2f \n",tempC_F);
    }

  display.display();
}

void wemoLoop(){
      if(mode==2){
        switchON(0);
        switchON(1);
        switchON(2);
        switchON(3);
        wemo_is_on=true;
      }
      else{
        if(wemo_is_on==true) {    //if wemo is on turn it off just one time
          switchOFF(0);
          switchOFF(1);
          switchOFF(2);
          switchOFF(3);
          wemo_is_on=false;
        }
      }   
}
void sonarLoop(){
    Serial.println("sonar mode");
  //"When red button pressed & held sonar sensor cycles & LED flashes for each cycle"
  
  val_buttonRed=digitalRead(buttonRed);
  if(val_buttonRed){
  delay(1000); // Execute once per second 
  Serial.println("button has been pressed");
  
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds, this is the pulse that will be detected
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  digitalWrite(ledYellow_pin,HIGH);
  duration = pulseIn(echoPin, HIGH);
  
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.printf("Time %i, Distance: %i, Duration %i \n",millis(),distance, duration);
  
  digitalWrite(ledYellow_pin,LOW);
  }
}
void hueLoop(){
  Serial.println("hue mode");
  
    val_buttonRed=digitalRead(buttonRed);
  if(val_buttonRed){
      HueOn=!HueOn;
  }
    HueBright=myEnc.read();        //read encoder position
    if(HueBright>maxBrightness){   //if position goes over 255 keep at last position
      HueBright=(maxBrightness);
    }
    if(HueBright<minBrightness){   //if position goes under 0 keep at last position
      HueBright=(minBrightness);
    }
    
    if(HueOn){                    //Turn on the light when HueOn=true
      digitalWrite(encoderGreen,LOW);
      digitalWrite(encoderRed,HIGH);
      for(int i=0;i<7;i++){
        setHue(lightNum,HueOn,HueRainbow[i],HueBright);
      }
    }
      else{
        digitalWrite(encoderGreen,HIGH);
        digitalWrite(encoderRed,LOW);
        setHue(lightNum,HueOn,0,0);   //Turn off the light when HueOn=false
        delay(100);
      }
}
