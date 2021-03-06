/**
 * Ce code joue le role de telecommande du Uno-Braccio.
 * Chaque appuie sur le boutton déclenche une action
 */

#include <U8g2lib.h>
#include <RF24Network.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define TELECOMMANDE 4 //télécommande (emetteur numéro 4)

//RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
RF24 radio(9,10); //emission avec Arduino Nano-rf

//init Réseau
RF24Network network(radio); 
const byte noeudMere  = 00; // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const byte monNoeud   = 04;

//init affichage graphique
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//Pins des boutons
enum BOUTON {PIN_HAUT = 6, PIN_BAS = 5, PIN_PLAY = 3, PIN_PAUSE = 2, PIN_STOP = 4};

//Interaction avec l'écran
const byte NOMBRE_ORDRES = 4;
short numOrdre = 3; 

enum ACTIONS {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES {COLERE = 20, JOIE = 21, SURPRISE = 22, CONTROLE = 23, RIEN = 24};

const char ordreTexte[NOMBRE_ORDRES][10] = {"Colere", "Joie", "Surprise", "Controle"};
const byte ordre[NOMBRE_ORDRES]          = {COLERE, JOIE, SURPRISE, CONTROLE};

//structure de données pour l'envoie des ordres
struct dataToSend
{
    short id      = TELECOMMANDE - 1;
    short xAxis   = 0; //inutile pour la telecommande
    short yAxis   = 0; //inutile pour la telecommande
    char mode     = RIEN;
    char _action  = PLAY;
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
    u8g2.enableUTF8Print();
    
    u8g2.firstPage();
    do 
    {
        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.drawStr(20, 40, "Init ...");
    } while (u8g2.nextPage());

    delay(1000);
    u8g2.clear();
}

void loop() 
{
    //vide le buffer
    network.update();
  
    //action en fonction du bouton actif
    if (digitalRead(PIN_HAUT) == HIGH)
    {
        u8g2.clear();
        
        numOrdre = (numOrdre + 1) % (short)NOMBRE_ORDRES;
        
        while (digitalRead(PIN_HAUT) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
              u8g2.setFont(u8g2_font_ncenB14_tr);
              u8g2.drawStr(10, 50, ordreTexte[numOrdre]);
            } while (u8g2.nextPage());
        }
        u8g2.clear();
    }
    
    else if (digitalRead(PIN_BAS) == HIGH)
    {
        u8g2.clear();
       
        numOrdre--;
        numOrdre = numOrdre < 0 ? (short)(NOMBRE_ORDRES - 1): numOrdre;
        
        while (digitalRead(PIN_BAS) == HIGH)
        { 
            u8g2.firstPage();
            do 
            {
              u8g2.setFont(u8g2_font_ncenB14_tr);
              u8g2.drawStr(10, 50, ordreTexte[numOrdre]);
            } while (u8g2.nextPage());
        }
        u8g2.clear();
    }
    
    else if (digitalRead(PIN_PLAY) == HIGH)
    {
        u8g2.clear();
        while (digitalRead(PIN_PLAY) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 20, "Play : ");
                u8g2.drawStr(10, 50, ordreTexte[numOrdre]);
            } while (u8g2.nextPage());
        }
        send_data.mode = ordre[numOrdre];
        send_data._action = PLAY;
        u8g2.clear();
    }
    
    else if (digitalRead(PIN_PAUSE) == HIGH)
    {
        u8g2.clear();

        while(digitalRead(PIN_PAUSE) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 50, "Pause");
            } while (u8g2.nextPage());
        }
        send_data._action = PAUSE;
    }
    
    else if (digitalRead(PIN_STOP) == HIGH)
    {
        u8g2.clear();

        while (digitalRead(PIN_STOP) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 50, "STOP");
            } while (u8g2.nextPage());
        }
        send_data.mode = RIEN;
        send_data._action = STOP;
        
        u8g2.clear();
    }
    
    else
    {
        switch (send_data._action)
        {
            case PLAY :
                u8g2.firstPage();
                do 
                {
                    u8g2.setFont(u8g2_font_ncenB14_tr);
                    u8g2.drawStr(10, 20, "En cours : ");
                    u8g2.drawStr(10, 50, ordreTexte[numOrdre]);
                } while (u8g2.nextPage());
                break;
  
              case STOP :
                  u8g2.firstPage();
                  do 
                  {
                      u8g2.setFont(u8g2_font_ncenB14_tr);
                      u8g2.drawStr(10, 50, "STOP !");
                  } while (u8g2.nextPage());
                  break;
  
              case PAUSE :
                  u8g2.firstPage();
                  do 
                  {
                      u8g2.setFont(u8g2_font_ncenB14_tr);
                      u8g2.drawStr(10, 50, "PAUSE !");
                  } while (u8g2.nextPage());
                  break;
          }
    }

    //envoie sur le réseau
    RF24NetworkHeader nHeader(noeudMere);
    network.write(nHeader, &send_data, sizeof(send_data));
    delay(10);
}
