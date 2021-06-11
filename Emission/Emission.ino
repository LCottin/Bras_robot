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
//const byte z_out = A2; //pour l'instant l'axe z est inutilisé

//RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
RF24 radio(9,10); //emission avec Arduino Nano-rf

#define EMETTEUR 1 //emetteur numero 1

const uint64_t Address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };
const uint64_t monAdresse = Address[ EMETTEUR - 1 ];

//structure de données pour l'envoie des inclinaisons fournies par l'accéléromètre
struct dataToSend
{
    //short id;
    short xAxis;
    short yAxis;
    //short zAxis;
} send_data;

//latence d'emission
int latence = 10;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    //configuration de l'émetteur
    radio.begin();
    radio.openWritingPipe(monAdresse);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(108);
    radio.setDataRate(RF24_2MBPS);
    radio.stopListening();

    //send_data.id = 1;
    
    /*
    //initialisation moniteur serie
    Serial.begin(9600);
    */
}   


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    //lecture des données sur les 2 axes (z inutilisé)
    send_data.xAxis = analogRead(x_out);
    send_data.yAxis = analogRead(y_out);
    //send_data.zAxis = analogRead(z_out);

    /*
    //Affichage des données
    Serial.println("Emetteur 1 envoie : ");
    Serial.print("en x : "); Serial.println(send_data.xAxis);
    Serial.print("en y : "); Serial.println(send_data.yAxis);
    Serial.print("en z : "); Serial.println(send_data.zAxis);
    Serial.println("");
    */
    
    //envoie des données lues
    radio.write(&send_data, sizeof(dataToSend));
    delay(latence);
}
