/**
 * Ce code permet de faire bouger le bras avec l'accéléromètre en temps réel
 */

#include "Braccio.h"

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

//pour memoriser la position de chaque servo
short posBase       = 90;
short posEpaule     = 95;
short posCoude      = 95; 
short posPoignetRot = 90;
short posPoignetVer = 90;
short posPince      = 90;

//Valeurs extremes recues en analogread pour la radio 1
struct V_MAX1
{
    const short XMIN = 80;
    const short XMAX = 975;
    
    const short YMIN = 45; 
    const short YMAX = 975;
    
    const short ZMIN = 45;
    const short ZMAX = 975;
} V_MAX1; 

//Valeurs extremes recues en analogread pour la radio 2
struct V_MAX2
{
    const short XMIN = 10;
    const short XMAX = 935;
  
    const short YMIN = 0;
    const short YMAX = 830;
  
    const short ZMIN = 0;
    const short ZMAX = 860;
} V_MAX2;


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
    const byte vitesse  = T_RAPIDE;
    const short latence = 40;
    
    miseEnForme();
    Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
    delay(500);
}


// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //
void miseEnForme()
{
    //lectures des 4 entrées
    short lectureX    = analogRead(A0);
    short lectureY    = analogRead(A1);
    short lectureZ    = analogRead(A2);
    short lectureX2    = analogRead(A3);
    short lectureY2    = analogRead(A4);
     
    //Serial.print("X = "); Serial.println(lectureX);
    //Serial.print("Y = "); Serial.println(lectureY);
    //Serial.print("Z = "); Serial.println(lectureZ);
    //Serial.print("X2 = "); Serial.println(lectureX2);
    Serial.print("Y2 = "); Serial.println(lectureY2);
   
    //recupere les valeurs émises par la PWM
    posCoude      = map(lectureX, V_MAX1.XMIN, V_MAX1.XMAX, 0, 180);
    posPoignetRot = map(lectureY, V_MAX1.YMIN, V_MAX1.YMAX, 0, 180);
    
    //sature en cas de valeurs trop importantes pour proteger les moteurs
    if (posCoude > 180) posCoude = 180;
    if (posCoude < 0)   posCoude = 0;
    
    if (posPoignetRot > 180) posPoignetRot = 180;
    if (posPoignetRot < 0)   posPoignetRot = 0;
    
    Serial.print("posCoude envoyée      = "); Serial.println(posCoude);
    Serial.print("posPoignetRot envoyée = "); Serial.println(posPoignetRot);
    
    Serial.print("\n");
    
    posPoignetVer = map(lectureX2, V_MAX2.XMIN, V_MAX2.XMAX, 0, 180);
    posPince      = map(lectureY2, V_MAX2.YMIN, V_MAX2.YMAX, 25, 90);
    
    if (posPoignetVer > 180) posPoignetVer = 180;
    if (posPoignetVer < 0)   posPoignetVer = 0;
    
    if (posPince > 90) posPince = 90;
    if (posPince < 25) posPince = 25;
    
    Serial.print("posPoignetVer envoyée = "); Serial.println(posPoignetVer);
    Serial.print("posPince envoyée      = "); Serial.println(posPince);
    
    Serial.print("\n");
    
}
