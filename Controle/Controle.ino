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
  short posBase     = vMax.XMOY;
  short posShoulder = vMax.YMOY;
  short posElbow    = vMax.XMOY; 
  short posWristRot = vMax.YMOY;
  short posWristVer = vMax.XMOY;
  short posGripper  = vMax.YMOY;
  char mode;
  char _action;
} local_data;

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

byte baseControle = 90;
byte shoulderControle = 95;
byte elbowControle = 95;
byte wristRotControle = 90;
byte wristVerControle = 90;
byte gripperControle = 95;

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
SPEED speed;

//Pour mesurer le temps d'execution d'un programme
unsigned long tempsDebut;
double duree;

//etat global 
bool _pauseGlobal;
bool _stopGlobal;

//macro pour mettre en pause le bras ou le stopper
#define ATTENTE PauseStop (); \
        while(_pauseGlobal){PauseStop();}; \
        if (_stopGlobal) return; 
// ---------------------------------------- //
// -                 SETUP                - //
// ---------------------------------------- //
void setup() 
{  
    Serial.begin(57600);
    
    /* ROUTINE D'INITIALISATION DU BRAS*/ 
    Braccio.begin();  
    delay(100);
    Braccio.stand();   

    initBufferEchantillons();

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
// -                  LOOP                - //
// ---------------------------------------- //
void loop() 
{
      speed  = T_RAPIDE;
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
                  local_data.posShoulder     = received_data.yAxis;
                  break;
        
                case Bracelet2 :
                  local_data.posElbow      = received_data.xAxis;
                  local_data.posWristRot = received_data.yAxis;
                  break;
                  
                case Bracelet3 :
                  local_data.posWristVer = received_data.xAxis;
                  local_data.posGripper      = received_data.yAxis;
                  break;
              }
              miseEnForme();
              droit = false;
              Braccio.moveAll(baseControle, shoulderControle, elbowControle, wristRotControle, wristVerControle, gripperControle, speed);
              break;
    
            case RIEN : 
            default : 
                if (droit == false)
                {
                    Serial.println("Droit");
                    Braccio.stand();
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
    Serial.println("Controle ...");
    //mise à jour comteur
    cmp = (cmp + 1) % NB_MOYENNAGE;

    //lecture des donnees
    x1[cmp] = local_data.posBase;
    y1[cmp] = local_data.posShoulder;
    x2[cmp] = local_data.posElbow;
    y2[cmp] = local_data.posWristRot;
    x3[cmp] = local_data.posWristVer;
    y3[cmp] = local_data.posGripper;

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
    baseControle       = map(moyenneX1, vMax.XMIN, vMax.XMAX, 0, 180);
    shoulderControle   = 6 + map(moyenneY1, vMax.YMIN, vMax.YMAX, 20, 160);
    elbowControle      = map(moyenneX2, vMax.XMIN, vMax.XMAX, 0, 180);
    wristRotControle   = map(moyenneY2, vMax.YMIN, vMax.YMAX, 0, 180);
    wristVerControle   = map(moyenneX3, vMax.XMIN, vMax.XMAX, 0, 180);
    gripperControle    = map(moyenneY3, vMax.YMIN, vMax.YMAX, 25, 90);
    
    //sature en cas de valeurs trop importantes pour proteger les moteurs
    if (baseControle > 180) baseControle = 180;
    
    if (shoulderControle > 165) shoulderControle = 165;
    if (shoulderControle < 15)  shoulderControle = 15;
    
    if (elbowControle > 180) elbowControle = 180;
    
    if (wristRotControle > 180) wristRotControle = 180;
    
    if (wristVerControle > 180) wristVerControle = 180;
    
    if (gripperControle > 90) gripperControle = 90;
    if (gripperControle < 25) gripperControle = 25;
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
    speed = RAPIDE;

    //return en cas de stop, attend en cas de pause
    ATTENTE
    
    Serial.println("Debut surprise");
    
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 180, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 90, speed);
    ATTENTE
    delay(500);
  
    ATTENTE
    
    Braccio.moveMotor(WRIST_ROT, 20, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 90, speed);
    ATTENTE
    delay(500);

    ATTENTE
  
    Braccio.moveMotor(ELBOW, 20, speed);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 180, speed);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(BASE, 60, speed);
    ATTENTE
    delay(500);

    ATTENTE
  
    Braccio.stand();
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.openGripper(T_RAPIDE);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.closeGripper(MOYEN);
    
  
    delay(1000);
    Braccio.stand();
  
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
    speed = T_LENT;
    Serial.println("Changement");
    Braccio.resetPos();
    ATTENTE
    Braccio.openGripper(speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.closeGripper(speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 180, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 90, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    Braccio.moveMotor(WRIST_ROT, 20, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 90, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    Braccio.moveMotor(ELBOW, 20, speed);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 180, speed);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(BASE, 60, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    delay(1000);
    Braccio.stand();

    delay(1000);
    speed = MOYEN;
    ATTENTE
    Braccio.resetPos();
    ATTENTE
    Braccio.openGripper(speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.closeGripper(speed);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 180, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 90, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    Braccio.moveMotor(WRIST_ROT, 20, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 90, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    Braccio.moveMotor(ELBOW, 20, speed);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 180, speed);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(BASE, 60, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    delay(2000);
    ATTENTE
    Braccio.stand();
    ATTENTE
    delay(2000);
    ATTENTE
  
    speed = T_RAPIDE;
    ATTENTE
    Serial.println("Changement");
    ATTENTE
    Braccio.resetPos();
    ATTENTE
    Braccio.openGripper(speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.closeGripper(speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 180, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 90, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    Braccio.moveMotor(WRIST_ROT, 20, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_VER, 0, speed);
    ATTENTE
    delay(500);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 90, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    Braccio.moveMotor(ELBOW, 20, speed);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 180, speed);
    ATTENTE
    Braccio.stand();
    ATTENTE
    Braccio.moveMotor(BASE, 60, speed);
    ATTENTE
    delay(500);
    ATTENTE
  
    delay(3000);
    Braccio.stand();
}


// ---------------------------------------- //
// -           EMOTION : COLERE           - //
// ---------------------------------------- //
void colere()
{
    tempsDebut = millis();
    speed = MOYEN;
  
    Braccio.moveMotor(SHOULDER, 150, MOYEN);
    ATTENTE
    delay(10);
    ATTENTE
    Braccio.moveAll(posBase, 90, 20, 180, posWristVer, posGripper, speed);
    ATTENTE
    delay(20);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 115, speed);
    ATTENTE
    delay(10);
    ATTENTE
  
    for (i = 0; i < 3; i++)
    {
        Braccio.openGripper(speed);
        ATTENTE
        delay(10);
        ATTENTE
        Braccio.closeGripper(speed);
        ATTENTE
        delay(10);
        ATTENTE
    }
    Braccio.moveMotor(GRIPPER, 90, speed);
    ATTENTE
    
    Braccio.moveMotor(SHOULDER, 30, speed);
    ATTENTE
    delay(10);
    Braccio.moveAll(posBase, 140, 95, 95, 90, 90, speed);
    ATTENTE
    delay(30);
    Braccio.moveAll(30, 110, 80, 150, 90, 90, speed);
    ATTENTE
    delay(30);
    Braccio.moveAll(90, 70, 60, 90, 90, 90, speed);
    ATTENTE
    delay(30);
    Braccio.moveAll(120, 30, 90, 90, 90, 90, speed);
    ATTENTE
    delay(100);
  
    Braccio.moveMotor(SHOULDER, 160, speed);
    ATTENTE
    delay(10);
    Braccio.moveMotor(WRIST_ROT, 150, speed);
    ATTENTE
    delay(50);
    Braccio.moveAll(posBase, 140, 95, 95, 90, 90, speed);
    ATTENTE
  
    Braccio.moveMotor(SHOULDER, 95, speed);
    ATTENTE
    delay(10);
    ATTENTE
    Braccio.moveMotor(ELBOW, 95, speed);
    ATTENTE
    delay(10);
    ATTENTE
    Braccio.moveMotor(WRIST_ROT, 10, speed);
    ATTENTE
    delay(10);
  
    for (i = 0; i < 4; i++)
    {
        if (i%2)
        {
            Braccio.moveMotor(BASE, 10, speed);
            ATTENTE
            delay(10);
            ATTENTE
            Braccio.closeGripper(speed);
            ATTENTE
            delay(10);
            ATTENTE
            Braccio.moveMotor(WRIST_ROT, 80, speed);
            ATTENTE
            delay(100);
            ATTENTE
        }
    
        else 
        {
            Braccio.moveMotor(BASE, 160, speed);
            ATTENTE
            delay(10);
            ATTENTE
            Braccio.openGripper(speed);
            ATTENTE
            delay(10);
            ATTENTE
            Braccio.moveMotor(WRIST_ROT, 60, speed);
            ATTENTE
            delay(100);
        }
    
        delay(50);
    }
  
    Braccio.moveMotor(BASE, 90, LENT);
    ATTENTE
    delay(10);
    Braccio.closeGripper(MOYEN);
    ATTENTE
    delay(10);
    Braccio.moveMotor(SHOULDER, 60, LENT);
    ATTENTE
    delay(10);
    Braccio.moveMotor(ELBOW, 170, LENT);
    ATTENTE
    delay(10);
    Braccio.moveMotor(WRIST_ROT, 20, LENT);
    ATTENTE
    delay(100);
  
    Braccio.moveAll(posBase, 70, 70, 60, 90, 90, RAPIDE);
    ATTENTE
    
    delay(1000);
    Braccio.stand();
  
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
    speed = RAPIDE;
  
    Braccio.moveMotor(SHOULDER, 140, MOYEN);
    ATTENTE
    delay(100);
    ATTENTE
    Braccio.moveMotor(ELBOW, 40, speed);
    ATTENTE
    delay(100);
    ATTENTE
  
    for (i = 0; i < 4; i++)
    {
        Braccio.moveMotor(WRIST_ROT, 150, speed);
        ATTENTE
        delay(50);
        Braccio.moveMotor(WRIST_ROT, 40, speed);
        ATTENTE
        delay(50);
    }

    ATTENTE
    delay(1000);
    Braccio.stand();
    ATTENTE
  
    Braccio.moveMotor(WRIST_ROT, 90, speed);
    ATTENTE
    Braccio.moveMotor(BASE, 170, RAPIDE);
    ATTENTE
    Braccio.moveAll(170, posShoulder, posElbow, 90, posWristVer, posGripper, RAPIDE);
    ATTENTE
    delay(50);
    ATTENTE
    Braccio.moveAll(30, posShoulder, posElbow, 40, posWristVer, posGripper, RAPIDE);
    ATTENTE
    delay(50);
  
    for (i = 0; i < 2; i++)
    {
        Braccio.moveMotor(BASE, 150, RAPIDE);
        ATTENTE
        delay(25);
        ATTENTE
        Braccio.moveMotor(SHOULDER, 60, MOYEN);
        ATTENTE
        delay(25);
        ATTENTE
        Braccio.moveMotor(ELBOW, 150, speed);
        ATTENTE
        delay(250);
        ATTENTE
        Braccio.moveMotor(WRIST_ROT, 60, speed);
        ATTENTE
        delay(25);
        ATTENTE
      
        Braccio.moveMotor(BASE, 40, RAPIDE);
        ATTENTE
        delay(25);
        ATTENTE
        Braccio.moveMotor(WRIST_ROT, 110, speed);
        ATTENTE
        delay(250);
        ATTENTE
        Braccio.moveMotor(ELBOW, 40, speed);
        ATTENTE
        delay(25);
        ATTENTE
        Braccio.moveMotor(SHOULDER, 130, MOYEN);
        ATTENTE
        delay(25);
    }
  
    Braccio.moveMotor(BASE, 90, speed);
    ATTENTE
    Braccio.moveMotor(SHOULDER, 140, MOYEN);
    ATTENTE
    delay(25);
    ATTENTE
    Braccio.moveMotor(ELBOW, 40, RAPIDE);
    ATTENTE
    delay(25);
  
    for (i = 0; i < 3; i++)
    {
        Braccio.openGripper(RAPIDE);
        ATTENTE
        delay(50);
        ATTENTE
        Braccio.closeGripper(RAPIDE);
        ATTENTE
        delay(50);
        ATTENTE
    }
  
    Braccio.moveMotor(GRIPPER, 90, MOYEN);
    ATTENTE
  
    for (i = 0; i < 3; i++)
    {
        Braccio.moveMotor(BASE, 40, RAPIDE);
        ATTENTE
        delay(20);
        ATTENTE
        Braccio.moveMotor(BASE, 140, RAPIDE);
        ATTENTE
        delay(20);
        ATTENTE
    }
  
    Braccio.moveMotor(BASE, 90, speed);
    ATTENTE
    delay(200);
    ATTENTE
  
    for(i = 0; i < 2; i++)
    {
        Braccio.moveAll(posBase, 140, 25, 150, posWristVer, posGripper, RAPIDE);
        ATTENTE
        delay(200);
        ATTENTE
        Braccio.moveAll(posBase, 50, 175, 40, posWristVer, posGripper, RAPIDE);
        ATTENTE
        delay(200);
        ATTENTE
    }
  
    delay(1000);
    ATTENTE
    Braccio.stand();
    ATTENTE
  
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
    speed = T_RAPIDE;
    Braccio.stand();
  
    posElbow = 135;
    posWristRot = 180;
    Braccio.moveAll(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper, speed);
  
    byte coude = posElbow;
    byte poignetRot = posWristRot;
    posElbow = 180;
    posWristRot = 40;
    while ( coude < posElbow || poignetRot > posWristRot)
    {
        //delay(100);
        coude++; 
        poignetRot--;
        Braccio.moveAll(posBase, posShoulder, coude, poignetRot, posWristVer, posGripper, speed);
    }
}
