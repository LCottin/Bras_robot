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
    short xAxis;
    short yAxis;
    short zAxis;
} receive_data;

//valeurs extremes recues par la radio 1
struct c_radio1
{
    const short xMin = 260;
    const short xMax = 415;
    
    const short yMin = 255;
    const short yMax = 405;
    
    const short zMin = 270;
    const short zMax = 420;
} c_radio1;

//valeurs extremes recues par la radio 2
struct c_radio2
{
    const short xMin = 280;
    const short xMax = 435;
  
    const short yMin = 280;
    const short yMax = 435;
  
    const short zMin = 285;
    const short zMax = 435;
} c_radio2;

//Pin analog de sortie pour la PWM
const byte xPWMpin = 3;
const byte yPWMpin = 5;
const byte zPWMpin = 6;

//valeurs de la PWM
short xPWM, yPWM, zPWM;

//pin de selection des données
const byte selecteur = 4;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(9600);
    pinMode(selecteur, OUTPUT);
    
    radio.begin();
    radio.openReadingPipe(0,address);
    radio.setPALevel(RF24_PA_MAX);
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
  
        //si les données recues sont positives : elles viennent de la radio 1
        if (receive_data.xAxis >= 0)
        {
            xPWM = map(receive_data.xAxis, c_radio1.xMin, c_radio1.xMax, 0, 255);
            yPWM = map(receive_data.yAxis, c_radio1.yMin, c_radio1.yMax, 0, 255);
            zPWM = map(receive_data.zAxis, c_radio1.zMin, c_radio1.zMax, 0, 255);
            digitalWrite(selecteur, HIGH);
        }
  
        //sinon les données recues viennent de la radio 2
        else 
        {
            xPWM = map(-1*receive_data.xAxis, c_radio2.xMin, c_radio2.xMax, 0, 255);
            yPWM = map(-1*receive_data.yAxis, c_radio2.yMin, c_radio2.yMax, 0, 255);
            zPWM = map(-1*receive_data.zAxis, c_radio2.zMin, c_radio2.zMax, 0, 255);
            digitalWrite(selecteur, LOW);
        }

        //sature la PWM en cas de dépassement 
        if(xPWM < 0 )   xPWM = 0;
        if(xPWM > 255 ) xPWM = 255;
  
        if(yPWM < 0 )   yPWM = 0;
        if(yPWM > 255 ) yPWM = 255;
  
        if(zPWM < 0 )   zPWM = 0;
        if(zPWM > 255 ) zPWM = 255;
  
        Serial.println(xPWM, DEC);
        Serial.println(yPWM, DEC);
        Serial.println(zPWM, DEC);
        Serial.println("");
        
        analogWrite(xPWMpin, xPWM);
        analogWrite(yPWMpin, yPWM);
        analogWrite(zPWMpin, zPWM);
    }
}
