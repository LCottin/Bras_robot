/**
 * Ce code joue le role de telecommande du Uno-Braccio.
 * Chaque appuie sur le boutton déclenche une action
 */

#include <U8g2lib.h>
#include <RF24Network.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf

#define TELECOMMANDE 4 //télécommande (emetteur numéro 4)

//init Réseau
RF24Network network(radio); 
const byte noeudMere  = 00; // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const byte monNoeud   = 04;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//Pins des boutons
//enum BOUTON {PIN_HAUT = 2, PIN_BAS = 3, PIN_PLAY = 4, PIN_PAUSE = 5, PIN_STOP = 6};
#define PIN_HAUT  2
#define PIN_BAS   3
#define PIN_PLAY  4
#define PIN_PAUSE 5
#define PIN_STOP  6

//Interéaction avec l'écran
#define NOMBRE_ORDRES 4
byte numOrdre      = -1; 

#define RIEN 10
#define PLAY 11
#define PAUSE 12
#define STOP 13
//enum ACTION {RIEN = 10, PLAY = 11, PAUSE = 12, STOP = 13};
const char* ordreTexte[NOMBRE_ORDRES] = {"Colere", "Joie", "Surprise", "T_Reel"};
const char  ordre[NOMBRE_ORDRES]      = {0, 1, 2, 3};


//structure de données pour l'envoie des ordres
struct dataToSend
{
    short id = TELECOMMANDE - 1;
    short xAxis = 0; //inutile pour la telecommande
    short yAxis = 0; //inutile pour la telecommande
    char mode;
} send_data;


void setup() 
{
    //Serial.begin(9600);
  
    //init radio
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    
    radio.stopListening();

    //init network
    network.begin(108, monNoeud);
  
    //init bouton
    pinMode(PIN_HAUT, INPUT);
    pinMode(PIN_BAS, INPUT);
    pinMode(PIN_PLAY, INPUT);
    pinMode(PIN_PAUSE, INPUT);
    pinMode(PIN_STOP, INPUT);
    
    //init ecran
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr) ;  // sélection d'une police de caractère
}

void loop() 
{
  //vide le buffer
  u8g2.clearBuffer();
  network.update();
  
  if (digitalRead(PIN_HAUT) == HIGH)
  {
        numOrdre = (numOrdre + 1) % NOMBRE_ORDRES;
        u8g2.drawStr(10, 15, ordreTexte[numOrdre]);
  }
  //    send_data.mode = HAUT;
  //    radio.write(&mode, sizeof(char));
  //    radio.startListening();
  //    startTimer = millis();
  //    timeout = false;
  //    while ( !radio.available() && !timeout ) 
  //    {
  //      if (millis() - startTimer > 1000 ) 
  //        timeout = true; //timed out
  //    }
  //    if (timeout)
  //    {
  //      u8g2.clearBuffer();
  //      u8g2.drawStr(0, 10, "pb");
  //      u8g2.drawStr(0, 28, "transmission");
  //      u8g2.drawStr(0, 46, "!!!");
  //    }
  //    else
  //    {
  //      radio.read(&codeRetour, sizeof(char));
  //      switch(codeRetour)
  //      {
  //        case 1 :
  //          u8g2.clearBuffer();
  //          u8g2.drawStr(40, 28, "Mode");
  //          u8g2.drawStr(20, 48, "Emotion 1");
  //          break;
  //        case 2 :
  //          u8g2.clearBuffer();
  //          u8g2.drawStr(40, 28, "Mode");
  //          u8g2.drawStr(20, 48, "Emotion 2");
  //          break;
  //        case 3 :
  //          u8g2.clearBuffer();
  //          u8g2.drawStr(40, 28, "Mode");
  //          u8g2.drawStr(20, 48, "Emotion 3");
  //          break;
  //        case 4 :
  //          u8g2.clearBuffer();
  //          u8g2.drawStr(40, 28, "Cmd");
  //          u8g2.drawStr(20, 48, "Tps reel");
  //          break;
  //        default :
  //          u8g2.clearBuffer();
  //          u8g2.drawStr(10, 28, "Pb Mode");
  //          itoa(codeRetour, str, 10);
  //          u8g2.drawStr(20, 48, str);
  //          break;
  //      }
  //    }
  //    radio.stopListening();
  //  }
    
    else if (digitalRead(PIN_BAS) == HIGH)
    {
        numOrdre = (numOrdre - 1) % NOMBRE_ORDRES;
        u8g2.drawStr(10, 15, ordreTexte[numOrdre]);
    }
    
    else if (digitalRead(PIN_PLAY) == HIGH)
    {
        u8g2.drawStr(10, 15, "En cours : ");
        u8g2.drawStr(10, 30, ordreTexte[numOrdre]);
        send_data.mode = ordre[numOrdre];
    }
    
    else if (digitalRead(PIN_PAUSE) == HIGH)
    {
        u8g2.drawStr(10, 15, "Pause !");
        send_data.mode = PAUSE;
    }
    
    else if (digitalRead(PIN_STOP) == HIGH)
    {
        u8g2.drawStr(10, 15, "STOP !");
        send_data.mode = STOP;
    }
    
    else
    {
        u8g2.drawStr(10, 15, "En attente !");
        send_data.mode = RIEN;
    }
  
    //affichage à l'écran
    u8g2.sendBuffer(); 
  
    //envoie sur le réseau
    RF24NetworkHeader nHeader(noeudMere);
    network.write(nHeader, &send_data, sizeof(send_data));
    delay(100);
  
}
