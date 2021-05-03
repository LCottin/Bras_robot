/**
 * Ce code permet de faire bouger le bras avec l'accéléromètre en temps réel
 */

#include "Braccio.h"
//#include <Servo.h>

 /*
   Step Delay: un délai en millisecondes entre le mouvement de chaque servo. Valeurs autorisées
   de 10 à 30 ms.
   M1=base degrees.            Valeurs autorisées de 0 à 180 degrés  (milieu = 90°)
   M2=shoulder degrees.        Valeurs autorisées de 15 à 165 degrés (milieu = 90°)
   M3=elbow degrees.           Valeurs autorisées de 0 à 180 degrés  (milieu = 90°)
   M4=wrist vertical degrees.  Valeurs autorisées de 0 à 180 degrés  (milieu = 90°)
   M5=wrist rotation degrees.  Valeurs autorisées de 0 à 180 degrés
   M6=gripper degrees.         Valeurs autorisées de 10 à 90 degrés. 
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

//definition des vitesses
enum VITESSE {T_LENT = 30, LENT = 25, MOYEN = 20, RAPIDE = 15, T_RAPIDE = 10};
byte vitesse;

//Pour mesurer le temps d'execution d'un programme
unsigned long tempsDebut, tempsFin;
double duree;
byte i = 0;

struct VMAX
{
  const int XMIN = 291;
  const int XMAX = 444;
  
  const int YMIN = 288; 
  const int YMAX = 436;
  
  const int ZMIN = 286;
  const int ZMAX = 442;
};
VMAX V_MAX;

struct valeurs
{
  int x;
  int y;
  int z;
};
valeurs* valeurs;

int pos[3] = {0, 0, 0};

// ---------------------------------------- //
// -                 SETUP                - //
// ---------------------------------------- //
void setup() 
{  
    Serial.begin(9600);
    Serial.print("Initialisation de ");
    Serial.println(__FILE__);
    
    /* ROUTINE D'INITIALISATION DU BRAS*/ 
    Braccio.begin();  
    delay(100);
    Braccio.positionDroite();
  
    Serial.print(__FILE__);
    Serial.println(" Prêt !");
}


// ---------------------------------------- //
// -                  LOOP                - //
// ---------------------------------------- //
void loop() 
{
    miseEnForme();
    Braccio.tournerCoude(pos[0], MOYEN);
    delay(200);
}


// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //
void miseEnForme()
{
    int x_min = 20;
    int x_max = 1000;

    short lectureX = analogRead(A0);
    short lectureY = analogRead(A1);
    short lectureZ = analogRead(A2);

    if ( lectureX < x_min) lectureX = x_min;
    if ( lectureX > x_max) lectureX = x_max;
  
    Serial.print("X = "); Serial.println(lectureX);
    Serial.print("Y = "); Serial.println(lectureY);
    Serial.print("Z = "); Serial.println(lectureZ);
    
    pos[0] = map(lectureX + 80, 0, 2047, 10, 170);
    pos[1] = map(lectureY, 0, 2047, 10, 170);
    pos[2] = map(lectureZ, 0, 2047, 10, 170);
  
    Serial.print("X envoyé = "); Serial.println(pos[0]);
    Serial.print("Y envoyé = "); Serial.println(pos[1]);
    Serial.print("Z envoyé = "); Serial.println(pos[2]);
    Serial.print("\n");

}
