/**
 * Code permettant de recevoir les données 
 * de l'acceléromètre avant de les transmettre au robot
 */

#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

RF24 radio(7, 8);

const byte address[6] = "00001";

struct data 
{
  int xAxis;
  int yAxis;
  int zAxis;
};
data receive_data;

// à compléter
int xMin = 260;
int yMin = 255;
int zMin = 270;

// à compléter
int xMax = 415;
int yMax = 405;
int zMax = 420;

int xPWMpin = 3;
int yPWMpin = 5;
int zPWMpin = 6;

int xPWM, yPWM, zPWM;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0,address);
    radio.setPALevel(RF24_PA_MAX);
    //radio.setDataRate(RF24_250KBPS);
    radio.setDataRate(RF24_2MBPS);
    radio.startListening();
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    while(radio.available()) 
    {
      radio.read(&receive_data, sizeof(data));
      Serial.println(receive_data.xAxis, DEC);
      Serial.println(receive_data.yAxis, DEC);
      Serial.println(receive_data.zAxis, DEC);
      Serial.println("");
      
      xPWM = map(receive_data.xAxis, xMin, xMax, 0, 255);
      yPWM = map(receive_data.yAxis, yMin, yMax, 0, 255);
      zPWM = map(receive_data.zAxis, zMin, zMax, 0, 255);

      if(xPWM < 0 ) xPWM = 0;
      if(xPWM > 255 ) xPWM = 255;

      if(yPWM < 0 ) yPWM = 0;
      if(yPWM > 255 ) yPWM = 255;

      if(zPWM < 0 ) zPWM = 0;
      if(zPWM > 255 ) zPWM = 255;
      
      Serial.println(xPWM, DEC);
      Serial.println(yPWM, DEC);
      Serial.println(zPWM, DEC);
      Serial.println("");
      
      analogWrite(xPWMpin, xPWM );
      analogWrite(yPWMpin, yPWM );
      analogWrite(zPWMpin, zPWM );
  }
}
