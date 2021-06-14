/**
 * Ce code joue le role de telecommande du Uno-Braccio.
 * Chaque appuie sur le boutton déclenche une action
 */

#include <U8g2lib.h>
#include <Wire.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf

#define TELECOMMANDE 4 //télécommande (emetteur numéro 4)

const uint64_t Address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
const uint64_t monAdresse = Address[ TELECOMMANDE - 1 ];


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//Premier bouton
const byte haut = 2;
const byte bas = 3;
const byte play = 4;
const byte pause = 5;
const byte _stop = 6;
char analog[10];
char mode;
unsigned long startTimer;
bool timeout;
char codeRetour;
char str[4];

void setup() 
{
  //Serial.begin(9600);
  
  //configuration de l'télécommande
  radio.begin();
  radio.openWritingPipe(monAdresse);
  radio.openReadingPipe(0,monAdresse);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(108);
  radio.setDataRate(RF24_2MBPS);
  radio.stopListening();
  
  // Initialisation
  u8g2.begin();

  pinMode(haut, INPUT);
  pinMode(bas, INPUT);
  pinMode(play, INPUT);
  pinMode(pause, INPUT);
  pinMode(_stop, INPUT);
  
  // Affichage de données à l'écran
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr) ;  // sélection d'une police de caractère
  u8g2.drawStr(0, 10, "Ligne 1");   // écriture à la position (x=0 et y=10)
  u8g2.drawStr(0, 28, "Ligne 2");     // écriture à la position (x=0 et y=28)
  u8g2.drawStr(0, 46, "Ligne 3");     // écriture à la position (x=0 et y=46)
  u8g2.drawStr(0, 64, "Ligne 4");     // écriture à la position (x=0 et y=64)
  u8g2.sendBuffer();                // affichage de ces infos à l'écran
  delay(500);
  u8g2.clearBuffer();
}

void loop() 
{
  //vide le buffer
  //u8g2.clearBuffer();

  /*
  //affiche "bouton"
  u8g2.drawStr(0, 20, "Bouton");

  //affiche l'état du bouton (n'accepte pas les caractères accentués)
  if (digitalRead(haut) == LOW)
    u8g2.drawStr(0, 40, "relache");
  
  else 
    u8g2.drawStr(0, 40, "appuye");
  
  //envoie les infos dans le buffer
  u8g2.sendBuffer();
  */
  
  if (digitalRead(haut) == HIGH)
  {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 30, "Demmande");
    u8g2.drawStr(0, 48, "Precedent");
    u8g2.sendBuffer();
    mode = 1;
    radio.write(&mode, sizeof(char));
    radio.startListening();
    startTimer = millis();
    timeout = false;
    while ( !radio.available() && !timeout ) 
    {
      if (millis() - startTimer > 1000 ) 
        timeout = true; //timed out
    }
    if (timeout)
    {
      u8g2.clearBuffer();
      u8g2.drawStr(0, 10, "pb");
      u8g2.drawStr(0, 28, "transmission");
      u8g2.drawStr(0, 46, "!!!");
    }
    else
    {
      radio.read(&codeRetour, sizeof(char));
      //Serial.write('#');
      switch(codeRetour)
      {
        case 1 :
          u8g2.clearBuffer();
          u8g2.drawStr(40, 28, "Mode");
          u8g2.drawStr(20, 48, "Emotion 1");
          break;
        case 2 :
          u8g2.clearBuffer();
          u8g2.drawStr(40, 28, "Mode");
          u8g2.drawStr(20, 48, "Emotion 2");
          break;
        case 3 :
          u8g2.clearBuffer();
          u8g2.drawStr(40, 28, "Mode");
          u8g2.drawStr(20, 48, "Emotion 3");
          break;
        case 4 :
          u8g2.clearBuffer();
          u8g2.drawStr(40, 28, "Cmd");
          u8g2.drawStr(20, 48, "Tps reel");
          break;
        default :
          u8g2.clearBuffer();
          u8g2.drawStr(10, 28, "Pb Mode");
          itoa(codeRetour, str, 10);
          u8g2.drawStr(20, 48, str);
          break;
      }
    }
    radio.stopListening();
  }
  /*
  else if (digitalRead(bas) == HIGH)
  {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "bas");
    mode = 1;
    radio.write(&mode, sizeof(char));
  }
  else if (digitalRead(play) == HIGH)
  {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "play !");
    mode = 2;
    radio.write(&mode, sizeof(char));
  }
  else if (digitalRead(pause) == HIGH)
  {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "pause !");
    mode = 3;
    radio.write(&mode, sizeof(char));
  }
  else if (digitalRead(_stop) == HIGH)
  {
    u8g2.clearBuffer();
    mode = 4;
    radio.write(&mode, sizeof(char));
    u8g2.drawStr(0, 10, "stop !");
  }*/
  else
  {
    delay(1000);
    u8g2.clearBuffer();
    u8g2.drawStr(0, 64, "rien !");
  }
  u8g2.sendBuffer();
  
  
  delay(100);
  
}
