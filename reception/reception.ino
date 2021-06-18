  /**
 * Code permettant de recevoir les données 
 * de l'acceléromètre avant de les transmettre au robot
 */

//#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#include <stdlib.h>

RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf

RF24Network network(radio);  

// Réseau
const uint16_t noeudMere      = 00;   // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const uint16_t noeudsFille[3] = {01, 02, 03};

const uint16_t monNoeud = noeudMere;
const uint16_t noeudCible = noeudMere;

enum BRACELET {Bracelet1, Bracelet2, Bracelet3};

// valeurs reçu par le module RF
struct data 
{
    short id;
    short xAxis;
    short yAxis;
} received_data;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  short posBase       = 300;
  short posEpaule     = 300;
  short posCoude      = 300; 
  short posPoignetRot = 300;
  short posPoignetVer = 300;
  short posPince      = 300;
} sent_data;

//moyennage
const byte NB_MOYENNAGE = 9;
byte cmp = 0;
short x1[NB_MOYENNAGE];
short y1[NB_MOYENNAGE];

short x2[NB_MOYENNAGE];
short y2[NB_MOYENNAGE];

short x3[NB_MOYENNAGE];
short y3[NB_MOYENNAGE];

//variables temporaires pour moyennage
short moyenneX1 = 0;
short moyenneY1 = 0;

short moyenneX2 = 0;
short moyenneY2 = 0;

short moyenneX3 = 0;
short moyenneY3 = 0;
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
    initBufferEchantillons();
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    network.update(); //MAJ du réseau 

    //lecture des données dans le réseau    
    while(network.available())
    {
      RF24NetworkHeader nHeader;
      network.read(nHeader, &received_data, sizeof(received_data));

    /*
      //affectation en fonction des id
      switch (received_data.id)
      {
        case Bracelet1 :
          sent_data.posBase   = received_data.xAxis;
          sent_data.posEpaule = received_data.yAxis;
          break;

        case Bracelet2 :
          sent_data.posCoude      = received_data.xAxis;
          sent_data.posPoignetRot = received_data.yAxis;
          break;
          
        case Bracelet3 :
          sent_data.posPoignetVer = received_data.xAxis;
          sent_data.posPince      = received_data.yAxis;
          break;
      }
      */
      
      miseEnForme();

      //envoie la structure entre 2 caractères de délimitation
      char message = 'c';
      
      Serial.write((char*)&message, sizeof(message));
      Serial.write((char*)&sent_data, sizeof(sent_data));
      Serial.write((char*)&message, sizeof(message));
    }
    delay(20);
}

// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //
void miseEnForme()
{
    //mise à jour comteur
    cmp = (cmp + 1) % NB_MOYENNAGE;

    //affectation en fonction des id
      switch (received_data.id)
      {
        case Bracelet1 :
          x1[cmp] = received_data.xAxis;
          y1[cmp] = received_data.yAxis;
          break;

        case Bracelet2 :
          x2[cmp] = received_data.xAxis;
          y2[cmp] = received_data.yAxis;
          break;
          
        case Bracelet3 :
          x3[cmp] = received_data.xAxis;
          y3[cmp] = received_data.yAxis;
          break;
      }
    
    //tri des tableaux
    qsort(x1, NB_MOYENNAGE, sizeof(short), compare);
    qsort(y1, NB_MOYENNAGE, sizeof(short), compare);
    qsort(x2, NB_MOYENNAGE, sizeof(short), compare);
    qsort(y2, NB_MOYENNAGE, sizeof(short), compare);
    qsort(x3, NB_MOYENNAGE, sizeof(short), compare);
    qsort(y3, NB_MOYENNAGE, sizeof(short), compare);

    //recupération de la valeur médiane
    sent_data.posBase       = x1[NB_MOYENNAGE / 2];
    sent_data.posEpaule     = y1[NB_MOYENNAGE / 2];
    sent_data.posCoude      = x2[NB_MOYENNAGE / 2];
    sent_data.posPoignetRot = y2[NB_MOYENNAGE / 2];
    sent_data.posPoignetVer = x3[NB_MOYENNAGE / 2];
    sent_data.posPince      = y3[NB_MOYENNAGE / 2];
}

// ---------------------------------------- //
// -     INITIALISATION DES BUFFERS       - //
// ---------------------------------------- //
void initBufferEchantillons()
{
  for(int i = 0; i < NB_MOYENNAGE; i++)
    {
      x1[i] = 300;
      y1[i] = 300;
      
      x2[i] = 300;
      y2[i] = 300;

      x3[i] = 300;
      y3[i] = 300;
    }
}

// ---------------------------------------- //
// -              COMPARAISON             - //
// ---------------------------------------- //
int compare (const void * a, const void * b) 
{
   return ( *(int*)a - *(int*)b );
}
