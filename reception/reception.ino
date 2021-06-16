  /**
 * Code permettant de recevoir les données 
 * de l'acceléromètre avant de les transmettre au robot
 */

//#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>

RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf

RF24Network network(radio);   // Nota : "Network" utilise la librairie "radio"

// Réseau
const uint16_t noeudMere   = 00;                // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const uint16_t noeudsFille[3] = {01, 02, 03};

const uint16_t monNoeud = noeudMere;
const uint16_t noeudCible = noeudMere;

// valeurs reçu par le module RF
struct data 
{
    short id;
    short xAxis;
    short yAxis;
    //short zAxis; // inutilisé
};

struct data temp;
struct data data0;
struct data data1;
struct data data2;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  char c;
  //struct data* allData[3] = {data0, data1, data2};
  struct data allData[3]= {data0, data1, data2};
} send_data;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    //init serial
    Serial.begin(115200);
    SPI.begin();
    
    //init radio
    send_data.c = 0xAA;
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);

    radio.startListening();

    //init network
    network.begin(108, monNoeud);
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    network.update(); //MAJ du réseau
    
    while(network.available())
    {
      RF24NetworkHeader nHeader;
      network.read(nHeader, &temp, sizeof(temp));

      send_data.allData[temp.id] = temp;
      
      Serial.println("Données : ");
      Serial.print("\t posBase       = "); Serial.println(send_data.allData[0].xAxis);
      Serial.print("\t posEpaule     = "); Serial.println(send_data.allData[0].yAxis);
      Serial.print("\t posCoude      = "); Serial.println(send_data.allData[1].xAxis);
      Serial.print("\t posPoignetVer = "); Serial.println(send_data.allData[1].yAxis);
      Serial.print("\t posPoignetRot = "); Serial.println(send_data.allData[2].xAxis);
      Serial.print("\t posPince      = "); Serial.println(send_data.allData[2].yAxis);
      Serial.println("");
      delay(20);
    }

   
  //Serial.write((char*)&send_data, sizeof(send_data));

}
