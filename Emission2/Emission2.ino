/**
 * Code permettant d'envoyer les données 
 * de l'acceléromètre au robot
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//ports de sortie
const byte x_out = A0;
const byte y_out = A1;
const byte z_out = A2;

//RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
RF24 radio(9,10); //emission avec Arduino Nano-rf
const byte address[6] = "00001";

//structure de données pour l'envoie des inclinaisons 
struct dataToSend
{
    short xAxis;
    short yAxis;
    short zAxis;
} send_data;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    //Configuration de l'émetteur
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.stopListening();
}

// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    //lecture des données sur les 3 axes et inversion de signe
    send_data.xAxis = -1*analogRead(x_out);
    send_data.yAxis = -1*analogRead(y_out);
    send_data.zAxis = -1*analogRead(z_out);

    //envoie des données lues
    radio.write(&send_data, sizeof(dataToSend));
    delay(70);
}
