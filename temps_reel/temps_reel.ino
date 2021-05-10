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

//definition des vitesses
enum VITESSE {T_LENT = 30, LENT = 25, MOYEN = 20, RAPIDE = 15, T_RAPIDE = 10};

//pin pour différencier la provenance des données
const byte selecteur = A3;

//Valeurs extremes recues en analogread pour la radio 1
struct V_MAX1
{
    const short XMIN = 80;
    const short XMAX = 975;
    
    const short YMIN = 45; 
    const short YMAX = 975;
    
    const short ZMIN = 286;
    const short ZMAX = 442;
} V_MAX1; 

//Valeurs extremes recues en analogread pour la radio 2
struct V_MAX2
{
    const short XMIN = 10;
    const short XMAX = 935;
    const short XMOY = (XMIN + XMAX) / 2;
  
    const short YMIN = 0;
    const short YMAX = 830;
    const short YMOY = (YMIN + YMAX) / 2;
  
    const short ZMIN = 0;
    const short ZMAX = 860;
    const short ZMOY = (ZMIN + ZMAX) / 2;
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
    delay(latence);
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
    int   lectureSel  = analogRead(selecteur);
     
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

    //si les données sont en provenance de la radio 1
    if (lectureSel > 511)
    {    
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
        Serial.print("Position selecteur    = "); Serial.println(lecturePin);
        
        Serial.print("\n");
    }

    //sinon si les données sont en provenance de la radio 2
    else 
    {
        posPoignetVer = map(lectureX, V_MAX2.XMIN, V_MAX2.XMAX, 0, 180);
        posPince      = map(lectureY, V_MAX2.YMIN, V_MAX2.YMAX, 25, 90);
    
        if (posPoignetVer > 180) posPoignetVer = 180;
        if (posPoignetVer < 0)   posPoignetVer = 0;
    
        if (posPince > 90) posPince = 90;
        if (posPince < 25) posPince = 25;
      
        Serial.print("posPoignetVer envoyée = "); Serial.println(posPoignetVer);
        Serial.print("posPince envoyée      = "); Serial.println(posPince);
        Serial.print("Position selecteur    = "); Serial.println(lecturePin);
        
        Serial.print("\n");
    }
}
