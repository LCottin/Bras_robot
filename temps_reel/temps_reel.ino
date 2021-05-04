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

struct VMAX
{
  const int XMIN = 80;
  const int XMAX = 975;
  const int XMOY = (XMIN + XMAX) / 2;
  
  const int YMIN = 45; 
  const int YMAX = 975;
  const int YMOY = (YMIN + YMAX) / 2;
  
  const int ZMIN = 286;
  const int ZMAX = 442;
};
VMAX V_MAX;

int pos[3];

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
    byte vitesse = T_RAPIDE;
    byte latence = 10;
    
    miseEnForme();
    Braccio.tournerCoude(posCoude, vitesse);
    Braccio.leverMain(posPoignetRot, vitesse);
    delay(latence);
}


// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //
void miseEnForme()
{
    short lectureX = analogRead(A0);
    short lectureY = analogRead(A1);
    short lectureZ = analogRead(A2);

    Serial.print("X = "); Serial.println(lectureX);
    Serial.print("Y = "); Serial.println(lectureY);
    Serial.print("Z = "); Serial.println(lectureZ);

    /* CONE DE DECLENCHEMENT
    if (lectureX < V_MAX.X1) posCoude -= 2;
    if (lectureX > V_MAX.X2) posCoude += 2;

    if (posCoude < 0) posCoude = 0;
    if (posCoude > 180) posCoude = 180;

    Serial.print("X envoyé = "); Serial.println(posCoude);
    */
    
    posCoude      = map(lectureX, V_MAX.XMIN, V_MAX.XMAX, 0, 180);
    posPoignetRot = map(lectureY, V_MAX.YMIN, V_MAX.YMAX, 0, 180);

    if (posCoude > 180) posCoude = 180;
    if (posCoude < 0)   posCoude = 0;

    if (posPoignetRot > 180) posPoignetRot = 180;
    if (posPoignetRot < 0)   posPoignetRot = 0;
  
    Serial.print("X envoyé = "); Serial.println(posCoude);
    Serial.print("Y envoyé = "); Serial.println(posPoignetRot);

    Serial.print("\n");
    
}
