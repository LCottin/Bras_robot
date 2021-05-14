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

RF24 radio(7,8);
const byte address[6] = "00001";

//structure de données pour l'envoie des inclinaisons fournies par l'accéléromètre
struct dataToSend
{
    byte xAxis;
    byte yAxis;
    byte zAxis;
} send_data;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    //configuration de l'émetteur
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
    //lecture des données sur les 3 axes
    send_data.xAxis = analogRead(x_out);
    send_data.yAxis = analogRead(y_out);
    send_data.zAxis = analogRead(z_out);

    //envoie des données lues
    radio.write(&send_data, sizeof(dataToSend));
    delay(70);
}
