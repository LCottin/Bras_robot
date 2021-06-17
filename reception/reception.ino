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

enum BRACELET {BRACELET1, BRACELET2, BRACELET3};

// valeurs reçu par le module RF
struct data 
{
    short id;
    short xAxis;
    short yAxis;
    //short zAxis; // inutilisé
};

struct data temp;
//struct data data0;
//struct data data1;
//struct data data2;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  //short bouger;
  short posBase = 300;
  short posEpaule = 300;
  short posCoude = 300; 
  short posPoignetRot = 300;
  short posPoignetVer = 300;
  short posPince = 300;
  //struct data allData[3]= {data0, data1, data2};
} send_data;

char message = 'c';

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    //init serial
    Serial.begin(115200);
    SPI.begin();
    
    //init radio
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

      switch (temp.id)
      {
        case BRACELET3 :
          send_data.posBase = temp.xAxis;
          send_data.posEpaule = temp.yAxis;
          break;

        case BRACELET1 :
          send_data.posCoude = temp.xAxis;
          send_data.posPoignetRot = temp.yAxis;
          break;
          
        case BRACELET2 :
          send_data.posPoignetVer = temp.xAxis;
          send_data.posPince = temp.yAxis;
          break;
      }

    /*      
      Serial.println("Données : ");
      Serial.print("\t posBase       = "); Serial.println(send_data.posBase);
      Serial.print("\t posEpaule     = "); Serial.println(send_data.posEpaule);
      Serial.print("\t posCoude      = "); Serial.println(send_data.posCoude);
      Serial.print("\t posPoignetRot = "); Serial.println(send_data.posPoignetRot);
      Serial.print("\t posPoignetVer = "); Serial.println(send_data.posPoignetVer);
      Serial.print("\t posPince      = "); Serial.println(send_data.posPince);
      Serial.println("");
      delay(20);
      */
    }

    /*
    send_data.bouger = 999;

    if (Serial.available())
    {
      recu = Serial.read();
      if (recu == 0)
      {
        Serial.write((char*)&send_data, sizeof(send_data));
        delay(200);
      }
//    }*/
    Serial.write((char*)&message, sizeof(message));
    Serial.write((char*)&send_data, sizeof(send_data));
    Serial.write((char*)&message, sizeof(message));
    delay(50);
}
