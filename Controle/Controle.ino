/**
 * Ce code permet de controler le bras robot 
**/

#include <stdlib.h>
#include "Braccio.h"

//bibliothèques pour la comminucation sans fils
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#include <stdlib.h>  

/*
    Step Delay: un délai en millisecondes entre le mouvement de chaque servo. Valeurs autorisées
    de 10 à 30 ms.
    M1=base degrees.            Valeurs autorisées de 0 à 180 degrés  (milieu = 90°)
    M2=shoulder degrees.        Valeurs autorisées de 15 à 165 degrés (milieu = 95°)
    M3=elbow degrees.           Valeurs autorisées de 0 à 180 degrés  (milieu = 95°)
    M4=wrist vertical degrees.  Valeurs autorisées de 0 à 180 degrés  (milieu = 90°)
    M5=wrist rotation degrees.  Valeurs autorisées de 0 à 180 degrés  (milieu = 90°)
    M6=gripper degrees.         Valeurs autorisées de 25 à 90 degrés. 
        - 10: la pince est ouverte, 
        - 90: la pince est fermée.
*/

// ---------------------------------------- //
// -         VARAIABLES GLOBALES          - //
// ---------------------------------------- //
RF24 radio(48,49); //emission avec NRF24l01

RF24Network network(radio);

// Réseau
const uint16_t noeudMere      = 00;   // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const uint16_t noeudsFille[3] = {01, 02, 03};

const uint16_t monNoeud = noeudMere;
const uint16_t noeudCible = noeudMere;

//Valeurs extremes recues par les radios
struct V_MAX
{
    const short XMIN = 260;
    const short XMAX = 420;
    const short XMOY = (XMIN + XMAX)/2;
    
    const short YMIN = 260; 
    const short YMAX = 420;
    const short YMOY = (YMIN + YMAX)/2;
} vMax; 
           
// valeurs reçu par le module RF
struct data 
{
    short id;
    short xAxis;
    short yAxis;
    char mode;
    char _action;
} received_data;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  short posBase       = vMax.XMOY;
  short posEpaule     = vMax.YMOY;
  short posCoude      = vMax.XMOY; 
  short posPoignetRot = vMax.YMOY;
  short posPoignetVer = vMax.XMOY;
  short posPince      = vMax.YMOY;
  char mode;
  char _action;
} local_data;

Servo base;      //pour tourner sur la base
Servo shoulder;  //servo 2, en bas
Servo elbow;     //servo 3, central
Servo wrist_rot; //servo 4, baisse/monte la pince
Servo wrist_ver; //servo 5, tourne la pince
Servo gripper;   //servo 6, ouvre/ferme pince

//pour memoriser la positoin de chaque servo
short posBase       = 90;
short posEpaule     = 95;
short posCoude      = 95; 
short posPoignetRot = 90;
short posPoignetVer = 90;
short posPince      = 90;

enum BRACELETS {Bracelet1, Bracelet2, Bracelet3, Telecommande};
enum ACTIONS {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES {COLERE = 20, JOIE = 21, SURPRISE = 22, CONTROLE = 23, RIEN = 24};

//moyennage
byte cmp = 0;
const byte NB_MOYENNAGE = 7;
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

//Compteur de boucle global
short i;
bool droit = true;

//definition de la vitesse des moteurs
byte vitesse;

//Pour mesurer le temps d'execution d'un programme
unsigned long tempsDebut;
double duree;

//etat global 
bool _pauseGlobal;
bool _stopGlobal;

//macro pour mettre en pause le bras ou le stopper
#define ATTENTE PauseStop (); \
        while(_pauseGlobal){PauseStop();}; \
        if (_stopGlobal)return; 
// ---------------------------------------- //
// -                 SETUP                - //
// ---------------------------------------- //
void setup() 
{  
    Serial.begin(57600);
    
    /* ROUTINE D'INITIALISATION DU BRAS*/ 
    Braccio.begin();  
    delay(100);
    Braccio.positionDroite();   

    initBufferEchantillons();

    SPI.begin();
    
    //init radio
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);

    radio.startListening();

    //init network
    network.begin(108, monNoeud);

    //init interruption
    //tone(44, 50); //pin, frequence
    //attachInterrupt(digitalPinToInterrupt(21), PauseStop, FALLING);
}


// ---------------------------------------- //
// -                  LOOP                - //
// ---------------------------------------- //
void loop() 
{
      const byte vitesse  = T_RAPIDE;
      _pauseGlobal = false;
      _stopGlobal = false;
      
      network.update();

      while(network.available())
      {
          RF24NetworkHeader nHeader;
          network.read(nHeader, &received_data, sizeof(received_data));
          
          //action en fonction du récépteur
          switch (received_data.mode)
          { 
            case COLERE : 
              colere();
              droit = false;
              break;
    
            case JOIE :  
              joie();
              droit = false;
              break;
    
            case SURPRISE : 
              surprise();
              droit = false;
              break;
    
            case CONTROLE :
              local_data._action = received_data._action;
              switch (received_data.id)
              {
                case Bracelet1 :
                  local_data.posBase       = received_data.xAxis;
                  local_data.posEpaule     = received_data.yAxis;
                  break;
        
                case Bracelet2 :
                  local_data.posCoude      = received_data.xAxis;
                  local_data.posPoignetRot = received_data.yAxis;
                  break;
                  
                case Bracelet3 :
                  local_data.posPoignetVer = received_data.xAxis;
                  local_data.posPince      = received_data.yAxis;
                  break;
              }
              miseEnForme();
              droit = false;
              Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
              break;
    
            case RIEN : 
            default : 
              if (droit == false)
              {
                Serial.println("Droit");
                Braccio.positionDroite();
                droit = true;
              }
              break;
          }
      }
}

      
// ---------------------------------------- //
// -     INITIALISATION DES BUFFERS       - //
// ---------------------------------------- //
void initBufferEchantillons()
{
  for(i = 0; i < NB_MOYENNAGE; i++)
    {
      x1[i] = vMax.XMOY;
      y1[i] = vMax.YMOY;
      
      x2[i] = vMax.XMOY;
      y2[i] = vMax.YMOY;

      x3[i] = vMax.XMOY;
      y3[i] = vMax.YMOY;
    }
}

// ---------------------------------------- //
// -        LECTURE PAUSE ET STOP         - //
// ---------------------------------------- //
void PauseStop()
{
    network.update(); //MAJ du réseau 

    while(network.available())
    {
      RF24NetworkHeader nHeader;
      network.read(nHeader, &received_data, sizeof(received_data));
      
      if (received_data.id == Telecommande)
      {
          local_data.mode    = received_data.mode;
          local_data._action = received_data._action;
            
          switch (local_data._action)
          {
              case PAUSE :
                  _pauseGlobal = true;
                  _stopGlobal  = false;
                  break;
  
              case STOP : 
                  _pauseGlobal = false;
                  _stopGlobal  = true;
                  local_data.mode = RIEN;
                  droit = true;
                  break;
  
              case PLAY : 
              default:
                  _pauseGlobal = false;
                  _stopGlobal  = false;
                  break;
          }
      }  
    }
}


// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //
void miseEnForme()
{
    //mise à jour comteur
    cmp = (cmp + 1) % NB_MOYENNAGE;

    //lecture des donnees
    x1[cmp] = local_data.posBase;
    y1[cmp] = local_data.posEpaule;
    x2[cmp] = local_data.posCoude;
    y2[cmp] = local_data.posPoignetRot;
    x3[cmp] = local_data.posPoignetVer;
    y3[cmp] = local_data.posPince;

    //tri des tableaux
    qsort(x1, NB_MOYENNAGE, sizeof(short), compare);
    qsort(y1, NB_MOYENNAGE, sizeof(short), compare);
    qsort(x2, NB_MOYENNAGE, sizeof(short), compare);
    qsort(y2, NB_MOYENNAGE, sizeof(short), compare);
    qsort(x3, NB_MOYENNAGE, sizeof(short), compare);
    qsort(y3, NB_MOYENNAGE, sizeof(short), compare);

    //recupération de la valeur médiane
    moyenneX1 = x1[NB_MOYENNAGE / 2];
    moyenneX2 = x2[NB_MOYENNAGE / 2];
    moyenneX3 = x3[NB_MOYENNAGE / 2];
    moyenneY1 = y1[NB_MOYENNAGE / 2];
    moyenneY2 = y2[NB_MOYENNAGE / 2];
    moyenneY3 = y3[NB_MOYENNAGE / 2];
    
    //affecte les positions des moteurs avec un mapping
    posBase       = map(moyenneX1, vMax.XMIN, vMax.XMAX, 0, 180);
    posEpaule     = map(moyenneY1, vMax.YMIN, vMax.YMAX, 15, 165);
    posCoude      = map(moyenneX2, vMax.XMIN, vMax.XMAX, 0, 180);
    posPoignetRot = map(moyenneY2, vMax.YMIN, vMax.YMAX, 0, 180);
    posPoignetVer = map(moyenneX3, vMax.XMIN, vMax.XMAX, 0, 180);
    posPince      = map(moyenneY3, vMax.YMIN, vMax.YMAX, 25, 90);
    
    //sature en cas de valeurs trop importantes pour proteger les moteurs
    if (posBase > 180) posBase = 180;
    if (posBase < 0)   posBase = 0;
    
    if (posEpaule > 165) posEpaule = 165;
    if (posEpaule < 15)  posEpaule = 15;
    
    if (posCoude > 180) posCoude = 180;
    if (posCoude < 0)   posCoude = 0;
    
    if (posPoignetRot > 180) posPoignetRot = 180;
    if (posPoignetRot < 0)   posPoignetRot = 0;
    
    if (posPoignetVer > 180) posPoignetVer = 180;
    if (posPoignetVer < 0)   posPoignetVer = 0;
    
    if (posPince > 90) posPince = 90;
    if (posPince < 25) posPince = 25;
}

// ---------------------------------------- //
// -      REINITIALISE LES POSITIONS      - //
// ---------------------------------------- //
void init1()
{
    posBase       = 90;
    posEpaule     = 95;
    posCoude      = 95; 
    posPoignetRot = 90;
    posPoignetVer = 90;
    posPince      = 90;
}


// ---------------------------------------- //
// -              COMPARAISON             - //
// ---------------------------------------- //
int compare (const void * a, const void * b) 
{
   return ( *(int*)a - *(int*)b );
}



// ---------------------------------------- //
// -          EMOTION : SURPRISE          - //
// ---------------------------------------- //
void surprise()
{
    tempsDebut = millis();
    vitesse = RAPIDE;

    //return en cas de stop, attend en cas de pause
    ATTENTE
    
    Serial.println("Debut surprise");
    
    Braccio.positionDroite();
    ATTENTE
    Braccio.tournerMain(0, vitesse);
    ATTENTE
    Braccio.tournerMain(180, vitesse);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.tournerMain(90, vitesse);
    ATTENTE
    delay(500);
  
    ATTENTE
    
    Braccio.leverMain(20, vitesse);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.tournerMain(0, vitesse);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.leverMain(90, vitesse);
    ATTENTE
    delay(500);

    ATTENTE
  
    Braccio.tournerCoude(20, vitesse);
    ATTENTE
    Braccio.leverMain(180, vitesse);
    ATTENTE
    Braccio.positionDroite();
    ATTENTE
    Braccio.tournerBase(60, vitesse);
    ATTENTE
    delay(500);

    ATTENTE
  
    Braccio.positionDroite();
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.mainOuverte(T_RAPIDE);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.mainFermee(MOYEN);
    
  
    delay(1000);
    Braccio.positionDroite();
  
    duree = millis() - tempsDebut;
    Serial.print("Temps d'execution de surprise = ");
    Serial.print(duree/1000);
    Serial.println("s");
}


// ---------------------------------------- //
// -           FONCTION DE TEST           - //
// ---------------------------------------- //
void test1()
{
    vitesse = T_LENT;
    Serial.println("Changement");
    init1();
    Braccio.mainOuverte(vitesse);
    delay(500);
    Braccio.mainFermee(vitesse);
    delay(500);
    Braccio.positionDroite();
    Braccio.tournerMain(0, vitesse);
    delay(500);
    Braccio.tournerMain(180, vitesse);
    delay(500);
    Braccio.tournerMain(90, vitesse);
    delay(500);
  
    Braccio.leverMain(20, vitesse);
    delay(500);
    Braccio.tournerMain(0, vitesse);
    delay(500);
    Braccio.leverMain(90, vitesse);
    delay(500);
  
    Braccio.tournerCoude(20, vitesse);
    Braccio.leverMain(180, vitesse);
    Braccio.positionDroite();
    Braccio.tournerBase(60, vitesse);
    delay(500);
  
    delay(1000);
    Braccio.positionDroite();

    delay(1000);
    vitesse = MOYEN;
    init1();
    Braccio.mainOuverte(vitesse);
    delay(500);
    Braccio.mainFermee(vitesse);
    Braccio.positionDroite();
    Braccio.tournerMain(0, vitesse);
    delay(500);
    Braccio.tournerMain(180, vitesse);
    delay(500);
    Braccio.tournerMain(90, vitesse);
    delay(500);
  
    Braccio.leverMain(20, vitesse);
    delay(500);
    Braccio.tournerMain(0, vitesse);
    delay(500);
    Braccio.leverMain(90, vitesse);
    delay(500);
  
    Braccio.tournerCoude(20, vitesse);
    Braccio.leverMain(180, vitesse);
    Braccio.positionDroite();
    Braccio.tournerBase(60, vitesse);
    delay(500);
  
    delay(2000);
    Braccio.positionDroite();
    delay(2000);
  
    vitesse = T_RAPIDE;
    Serial.println("Changement");
    init1();
    Braccio.mainOuverte(vitesse);
    delay(500);
    Braccio.mainFermee(vitesse);
    delay(500);
    Braccio.positionDroite();
    Braccio.tournerMain(0, vitesse);
    delay(500);
    Braccio.tournerMain(180, vitesse);
    delay(500);
    Braccio.tournerMain(90, vitesse);
    delay(500);
  
    Braccio.leverMain(20, vitesse);
    delay(500);
    Braccio.tournerMain(0, vitesse);
    delay(500);
    Braccio.leverMain(90, vitesse);
    delay(500);
  
    Braccio.tournerCoude(20, vitesse);
    Braccio.leverMain(180, vitesse);
    Braccio.positionDroite();
    Braccio.tournerBase(60, vitesse);
    delay(500);
  
    delay(3000);
    Braccio.positionDroite();
}


// ---------------------------------------- //
// -           EMOTION : COLERE           - //
// ---------------------------------------- //
void colere()
{
    tempsDebut = millis();
    vitesse = MOYEN;
  
    Braccio.tournerEpaule(150, MOYEN);
    delay(10);
    Braccio.ServoMovement(vitesse, posBase, 90, 20, 180, posPoignetVer, posPince);
    delay(20);
    Braccio.leverMain(115, vitesse);
    delay(10);
  
    for (i = 0; i< 3; i++)
    {
        Braccio.mainOuverte(vitesse);
        delay(10);
        Braccio.mainFermee(vitesse);
        delay(10);
    }
    Braccio.ouvrirPince(90, vitesse);
    
    Braccio.tournerEpaule(30, vitesse);
    delay(10);
    Braccio.ServoMovement(vitesse, posBase, 140, 95, 95, 90, 90);
    delay(30);
    Braccio.ServoMovement(vitesse, 30, 110, 80, 150, 90, 90);
    delay(30);
    Braccio.ServoMovement(vitesse, 90, 70, 60, 90, 90, 90);
    delay(30);
    Braccio.ServoMovement(vitesse, 120, 30, 90, 90, 90, 90);
    delay(100);
  
    Braccio.tournerEpaule(160, vitesse);
    delay(10);
    Braccio.leverMain(150, vitesse);
    delay(50);
    Braccio.ServoMovement(vitesse, posBase, 140, 95, 95, 90, 90);
  
    Braccio.tournerEpaule(95, vitesse);
    delay(10);
    Braccio.tournerCoude(95, vitesse);
    delay(10);
    Braccio.leverMain(10, vitesse);
    delay(10);
  
    for (i = 0; i < 4; i++)
    {
        if (i%2)
        {
            Braccio.tournerBase(10, vitesse);
            delay(10);
            Braccio.mainFermee(vitesse);
            delay(10);
            Braccio.leverMain(80, vitesse);
            delay(100);
        }
    
        else 
        {
            Braccio.tournerBase(160, vitesse);
            delay(10);
            Braccio.mainOuverte(vitesse);
            delay(10);
            Braccio.leverMain(60, vitesse);
            delay(100);
        }
    
        delay(50);
    }
  
    Braccio.tournerBase(90, LENT);
    delay(10);
    Braccio.mainFermee(MOYEN);
    delay(10);
    Braccio.tournerEpaule(60, LENT);
    delay(10);
    Braccio.tournerCoude(170, LENT);
    delay(10);
    Braccio.leverMain(20, LENT);
    delay(100);
  
    Braccio.ServoMovement(RAPIDE, posBase, 70, 70, 60, 90, 90);
    
    delay(1000);
    Braccio.positionDroite();
  
    duree = millis() - tempsDebut;
    Serial.print("Temps d'execution de colere = ");
    Serial.print(duree/1000);
    Serial.println("s");
}


// ---------------------------------------- //
// -            EMOTION : JOIE            - //
// ---------------------------------------- //
void joie()
{
    tempsDebut = millis();
    vitesse = RAPIDE;
  
    Braccio.tournerEpaule(140, MOYEN);
    delay(100);
    Braccio.tournerCoude(40, vitesse);
    delay(100);
  
    for (i = 0; i < 4; i++)
    {
        Braccio.leverMain(150, vitesse);
        delay(50);
        Braccio.leverMain(40, vitesse);
        delay(50);
    }

    delay(1000);
    Braccio.positionDroite();
  
    Braccio.leverMain(90, vitesse);
    Braccio.tournerBase(170, RAPIDE);
    Braccio.ServoMovement(RAPIDE, 170, posEpaule, posCoude, 90, posPoignetVer, posPince);
    delay(50);
    Braccio.ServoMovement(RAPIDE, 30, posEpaule, posCoude, 40, posPoignetVer, posPince);
    delay(50);
  
    for (i = 0; i < 2; i++)
    {
        Braccio.tournerBase(150, RAPIDE);
        delay(25);
        Braccio.tournerEpaule(60, MOYEN);
        delay(25);
        Braccio.tournerCoude(150, vitesse);
        delay(250);
        Braccio.leverMain(60, vitesse);
        delay(25);
      
        Braccio.tournerBase(40, RAPIDE);
        delay(25);
        Braccio.leverMain(110, vitesse);
        delay(250);
        Braccio.tournerCoude(40, vitesse);
        delay(25);
        Braccio.tournerEpaule(130, MOYEN);
        delay(25);
    }
  
    Braccio.tournerBase(90, vitesse);
    Braccio.tournerEpaule(140, MOYEN);
    delay(25);
    Braccio.tournerCoude(40, RAPIDE);
    delay(25);
  
    for (i = 0; i < 3; i++)
    {
        Braccio.mainOuverte(RAPIDE);
        delay(50);
        Braccio.mainFermee(RAPIDE);
        delay(50);
    }
  
    Braccio.ouvrirPince(90, MOYEN);
  
    for (i = 0; i < 3; i++)
    {
        Braccio.tournerBase(40, RAPIDE);
        delay(20);
        Braccio.tournerBase(140, RAPIDE);
        delay(20);
    }
  
    Braccio.tournerBase(90, vitesse);
    delay(200);
  
    for(i = 0; i < 2; i++)
    {
        Braccio.ServoMovement(RAPIDE, posBase, 140, 25, 150, posPoignetVer, posPince);
        delay(200);
        Braccio.ServoMovement(RAPIDE, posBase, 50, 175, 40, posPoignetVer, posPince);
        delay(200);
    }
  
    delay(1000);
    Braccio.positionDroite();
  
    duree = millis() - tempsDebut;
    Serial.print("Temps d'execution de joie = ");
    Serial.print(duree/1000);
    Serial.println("s");
}


// ---------------------------------------- //
// -     MOUVEMENT : VAGUE (A REFAIRE)    - //
// ---------------------------------------- //
void vague()
{
    vitesse = T_RAPIDE;
    Braccio.positionDroite();
  
    posCoude = 135;
    posPoignetRot = 180;
    Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
  
    byte coude = posCoude;
    byte poignetRot = posPoignetRot;
    posCoude = 180;
    posPoignetRot = 40;
    while ( coude < posCoude || poignetRot > posPoignetRot)
    {
        //delay(100);
        coude++; 
        poignetRot--;
        Braccio.ServoMovement(vitesse, posBase, posEpaule, coude, poignetRot, posPoignetVer, posPince);
    }
}
