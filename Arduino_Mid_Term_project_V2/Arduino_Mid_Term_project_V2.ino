/* 
 *  Project:  Mid-Term project
 *  Description:  smart classroom
 *  Author:  Rodney Ham
 *  date:  28 Oct 2020 Monday
*/

#include <Encoder.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_BME280 bme;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

bool buttonState;
bool lastButton_state;

const int ledRed_pin=5;
const int ledClear_pin=4;
const int ledBlue_pin=3;
const int ledYellow_pin=2;

const int enc_PinA=1;
const int enc_PinB=0;
Encoder myEnc(enc_PinB,enc_PinA);
const int enc_sw_button=23;   // encoder switch connected to digital pin 23

int status;
int tempRange;

float tempC;
float tempC_F;

//This is for the BME280 humidity, temperature & pressure sensor
//to interface the device's I2C address is either 0x76 or 0x77.

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     14 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  
void setup() {
  Serial.begin (9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  status=bme.begin(0x76);
    if(status==false){
    Serial.print("initialization failed");
    }

pinMode(ledRed_pin,OUTPUT);     //set LED pin to Output
pinMode(ledClear_pin,OUTPUT);
pinMode(ledBlue_pin,OUTPUT);
pinMode(ledYellow_pin,OUTPUT);
}

void loop() {
    testdrawstyles();    // Draw 'stylized' characters
    delay(2000);
}
void testdrawstyles(void) {
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
    else if(tempC>=72.0){
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
