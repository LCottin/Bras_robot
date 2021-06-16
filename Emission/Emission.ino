/**
 * Code permettant d'envoyer les données 
 * de l'acceléromètre au robot
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>

#define EMETTEUR 1

#if EMETTEUR == 3
  RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
#else
  RF24 radio(9,10); //emission avec Arduino Nano-rf
#endif

//RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf

RF24Network network(radio);   // Nota : "Network" utilise la librairie "radio"

// Réseau
const uint16_t noeudMere   = 00;                // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const uint16_t noeudsFille[3] = {01, 02, 03};

const uint16_t monNoeud = noeudsFille[EMETTEUR - 1];
const uint16_t noeudCible = noeudMere;

//structure de données pour l'envoie des inclinaisons fournies par l'accéléromètre
struct dataToSend
{
    short id = EMETTEUR - 1;
    short xAxis;
    short yAxis;
    //short zAxis;
} send_data;

//latence d'emission
int latence = 10;

//ports de sortie
const byte x_out = A0;
const byte y_out = A1;
//const byte z_out = A2; //pour l'instant l'axe z est inutilisé

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    SPI.begin();
    
    //init radio
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    
    radio.stopListening();

    //init network
    network.begin(108, monNoeud);
    
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
    network.update();
    
    RF24NetworkHeader nHeader(noeudCible);

    send_data.xAxis = analogRead(x_out);
    send_data.yAxis = analogRead(y_out);
    
    network.write(nHeader, &send_data, sizeof(send_data));  
   
  /*
    //Affichage des données
    Serial.println("Emetteur 1 envoie : ");
    Serial.print("en x : "); Serial.println(send_data.xAxis);
    Serial.print("en y : "); Serial.println(send_data.yAxis);
    Serial.print("en z : "); Serial.println(send_data.zAxis);
    Serial.println("");
  */
}
