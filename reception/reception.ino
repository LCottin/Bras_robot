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
int xMin = 291;
int yMin = 288;
int zMin = 286;

// à compléter
int xMax = 444;
int yMax = 436;
int zMax = 442;

int xPWMpin = 3;
int yPWMpin = 5;
int zPWMpin = 6;


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
  
      analogWrite(xPWMpin, map(receive_data.xAxis, xMin, xMax, 0, 255));
      analogWrite(yPWMpin, map(receive_data.yAxis, yMin, yMax, 0, 255));
      analogWrite(zPWMpin, map(receive_data.zAxis, zMin, zMax, 0, 255));
  }
}
