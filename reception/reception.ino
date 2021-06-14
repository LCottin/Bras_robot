  /**
 * Code permettant de recevoir les données 
 * de l'acceléromètre avant de les transmettre au robot
 */

//#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf
const uint64_t Address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };
byte emetteur; //buffer correspondant à l'adresse d'un éméteur

char str[20]; // buffer temporaire pour la récupération de données
const char taille = 4; //taille max des valeurs converties en chaine

// valeurs reçu par le module RF
struct data 
{
    //short id;
    short xAxis;
    short yAxis;
    //short zAxis; // inutilisé
} receive_data;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  char c;
  short id;
  struct data receive_data;
} send_data;

char mode;
char codeRetour;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(115200);
    send_data.c = 0xAA;
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(108);
    radio.setDataRate(RF24_2MBPS);
    radio.openReadingPipe(0,Address[0]);
    radio.openReadingPipe(1,Address[1]);
    radio.openReadingPipe(2,Address[2]);
    radio.openReadingPipe(3,Address[3]);
    //radio.openReadingPipe(4,Address[4]);
    //radio.openReadingPipe(5,Address[5]);
    radio.openWritingPipe(Address[3]);
    radio.startListening();
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    while(radio.available(&emetteur)) 
    {
      if(emetteur != 3)
      {
        radio.read(&send_data.receive_data, sizeof(data));
        send_data.id = emetteur + 1;
        send_data.c = 0xAA;
        while(Serial.read() != '#');
        Serial.write((char*)&send_data, sizeof(send_data));
      }
      else
      {
        radio.read(&mode, sizeof(char));
        radio.stopListening();
        // to do : envoyer les infos à la uno
        send_data.id = emetteur + 1;
        send_data.c = mode;
        //while(Serial.read() != '#');
        Serial.write((char*)&send_data, sizeof(send_data));
        while(!Serial.available()){};
        codeRetour = Serial.read();
        //while(Serial.read() != '#');
        
        //delay(50);
        //Serial.println("recu ! ");
        radio.write(&codeRetour, sizeof(char));
        radio.startListening();
      }
        
      //delay(10);
    }
}
