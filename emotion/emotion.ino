/**
 * Ce code permet de controler le bras robot avec les 4 phrases écrites
**/

#include "Braccio.h"

//bibliothèques pour la comminucation sans fils
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

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

//definition de la vitesse des moteurs
byte vitesse;

//Pour mesurer le temps d'execution d'un programme
unsigned long tempsDebut, tempsFin;
double duree;
byte i = 0;

//Pour le module SF
//RF24 radio(2, 4);
const byte adresse[6] = "00001";

struct valeur
{
  int XMAX, XMIN;
  int YMIN, YMAX;
  int ZMIN, ZMAX;
};
valeur valeurs;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{  
    Serial.begin(9600);
    Serial.print("Initialisation de ");
    Serial.println(__FILE__);
  
    /* ROUTINE POUR LA SF */
//    radio.begin();
//    radio.openReadingPipe(0, adresse);
//    radio.setPALevel(RF24_PA_MAX);
//    radio.setDataRate(RF24_2MBPS);
//    radio.startListening();
    
    /* ROUTINE D'INITIALISATION DU BRAS*/ 
    Braccio.begin();  
    delay(1000);
    Braccio.positionDroite();

    Serial.print(__FILE__);
    Serial.println(" Prêt !");

 //---------------------------------- METTRE LE CODE A EFFECTUER ICI ----------------------------------------//
   
    surprise();
    delay(3000);
    joie();

  // ----------------------------------------------------------------------------------------------------------- //
}    


// ---------------------------------------- //
// -                 LOOP                 - //
// ---------------------------------------- //
void loop() 
{
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
  
    tempsFin = millis();
    duree = tempsFin - tempsDebut;
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
  
    tempsFin = millis();
    duree = tempsFin - tempsDebut;
    Serial.print("Temps d'execution de joie = ");
    Serial.print(duree/1000);
    Serial.println("s");
}


// ---------------------------------------- //
// -          EMOTION : SURPRISE          - //
// ---------------------------------------- //
void surprise()
{
    tempsDebut = millis();
    vitesse = LENT;
    
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
  
    Braccio.positionDroite();
    delay(500);
    Braccio.mainOuverte(T_RAPIDE);
    delay(500);
    Braccio.mainFermee(MOYEN);
  
    delay(1000);
    Braccio.positionDroite();
  
    tempsFin = millis();
    duree = tempsFin - tempsDebut;
    Serial.print("Temps d'execution de surprise = ");
    Serial.print(duree/1000);
    Serial.println("s");
}


// ---------------------------------------- //
// -           MOUVEMENT : VAGUE          - //
// ---------------------------------------- //
void vague()
{
    vitesse = T_RAPIDE;
    Braccio.positionDroite();
  
    posCoude = 135;
    posPoignetRot = 180;
    Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
  
    byte i = posCoude;
    byte j = posPoignetRot;
    posCoude = 180;
    posPoignetRot = 40;
    while ( i < posCoude || j > posPoignetRot)
    {
        //delay(100);
        i++; 
        j--;
        Braccio.ServoMovement(vitesse, posBase, posEpaule, i, j, posPoignetVer, posPince);
    }
}
