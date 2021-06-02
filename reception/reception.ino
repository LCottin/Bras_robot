/**
 * Code permettant de recevoir les données 
 * de l'acceléromètre avant de les transmettre au robot
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

struct data 
{
    short xAxis;
    short yAxis;
    short zAxis;
} receive_data;

//valeurs extremes recues par la radio 1 (valeurs positives)
struct c_radio1
{
    const short xMin = 265;
    const short xMax = 415;
    
    const short yMin = 260;
    const short yMax = 405;
    
    const short zMin = 265;
    const short zMax = 410;
} c_radio1;

//valeurs extremes recues par la radio 2 (valeurs négatives)
struct c_radio2
{
    const short xMin = 270;
    const short xMax = 415;
  
    const short yMin = 265;
    const short yMax = 415;

    const short zMin = 270;
    const short zMax = 420;
} c_radio2;

//Pin analog de sortie pour la PWM
const byte xPWMpin = 3;
const byte yPWMpin = 5;
const byte zPWMpin = 6;
const byte xPWMpin2 = 9;
const byte yPWMpin2 = 10;

//valeurs de la PWM
short xPWM, yPWM, zPWM, xPWM2, yPWM2;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(9600);
    Serial.print(__FILE__); Serial.println(" initialising ...");
    
    radio.begin();
    radio.openReadingPipe(0,address);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.startListening();
    Serial.print(__FILE__); Serial.println(" ready !");
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    while(radio.available()) 
    {
        radio.read(&receive_data, sizeof(data));

        //si les données recues sont positives : elles viennent de la radio 1
        if (receive_data.xAxis >= 0)
        {
            xPWM = map(receive_data.xAxis, c_radio1.xMin, c_radio1.xMax, 0, 255);
            yPWM = map(receive_data.yAxis, c_radio1.yMin, c_radio1.yMax, 0, 255);
            zPWM = map(receive_data.zAxis, c_radio1.zMin, c_radio1.zMax, 0, 255);
        }
        //sinon les données recues viennent de la radio 2
        else 
        {
            xPWM2 = map(-1*receive_data.xAxis, c_radio2.xMin, c_radio2.xMax, 0, 255);
            yPWM2 = map(-1*receive_data.yAxis, c_radio2.yMin, c_radio2.yMax, 0, 255);
        }

        //sature la PWM en cas de dépassement 
        if(xPWM < 0 )   xPWM = 0;
        if(xPWM > 255 ) xPWM = 255;
  
        if(yPWM < 0 )   yPWM = 0;
        if(yPWM > 255 ) yPWM = 255;
  
        if(zPWM < 0 )   zPWM = 0;
        if(zPWM > 255 ) zPWM = 255;

        if(xPWM2 < 0 )   xPWM2 = 0;
        if(xPWM2 > 255 ) xPWM2 = 255;
  
        if(yPWM2 < 0 )   yPWM2 = 0;
        if(yPWM2 > 255 ) yPWM2 = 255;
  
        analogWrite(xPWMpin, xPWM);
        analogWrite(yPWMpin, yPWM);
        analogWrite(zPWMpin, zPWM);
        analogWrite(xPWMpin2, xPWM2);
        analogWrite(yPWMpin2, yPWM2);
    }
}
