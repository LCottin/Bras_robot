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
    const short XMOY = (XMIN + XMAX)/2;
    
    const short YMIN = 45; 
    const short YMAX = 975;
    const short YMOY = (YMIN + YMAX)/2;
    
    const short ZMIN = 45;
    const short ZMAX = 975;
    const short ZMOY = (ZMIN + ZMAX)/2;
} V_MAX1; 

//Valeurs extremes recues en analogread pour la radio 2
struct V_MAX2
{
    const short XMIN = 10;
    const short XMAX = 935;
    const short XMOY = (XMIN + XMAX)/2;
      
    const short YMIN = 680;
    const short YMAX = 936;
    const short YMOY = (YMIN + YMAX)/2;
  
    const short ZMIN = 0;
    const short ZMAX = 860;
    const short ZMOY = (ZMIN + ZMAX)/2;
} V_MAX2;

// les buffers echantillons pour le moyennage
const byte N = 5;
unsigned short cmp = 0;
short x1[N];
short y1[N];
short z1[N];
short x2[N];
short y2[N];


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
    initBufferEchantillons();
}


// ---------------------------------------- //
// -                  LOOP                - //
// ---------------------------------------- //
void loop() 
{
    const byte vitesse  = T_RAPIDE;
    const short latence = 0;
    
    miseEnForme();
    Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
    delay(latence);
}


// ---------------------------------------- //
// -     INITIALISATION DES BUFFERS       - //
// ---------------------------------------- //
void initBufferEchantillons()
{
  for(int i = 0; i < N; i++)
    {
      x1[i] = V_MAX1.XMOY;
      y1[i] = V_MAX1.YMOY;
      z1[i] = V_MAX1.ZMIN;
      x2[i] = V_MAX2.XMIN;
      y2[i] = V_MAX2.YMIN;
    }
}


// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //
void miseEnForme()
{
    //mise à jour comteur
    cmp = (cmp + 1) % N;
    
    //variables temporaires pour moyennage
    short moyenneX = 0;
    short moyenneY = 0;
    short moyenneZ = 0;
    short moyenneX2 = 0;
    short moyenneY2 = 0;

    //lectures des 4 entrées et maj du buffer
    x1[cmp] = analogRead(A0);
    y1[cmp] = analogRead(A1);
    z1[cmp] = analogRead(A2);
    x2[cmp] = analogRead(A3);
    y2[cmp] = analogRead(A4);
    
    //moyennage des valeurs
    for(int i = 0; i < N; i++)
    {
      moyenneX  += x1[i];
      moyenneY  += y1[i];
      moyenneZ  += z1[i];
      moyenneX2 += x2[i];
      moyenneY2 += y2[i];
    }

    moyenneX  /= N;
    moyenneY  /= N;
    moyenneZ  /= N;
    moyenneX2 /= N;
    moyenneY2 /= N;
    
    //recupere les valeurs émises par la PWM
    posCoude      = map(moyenneX, V_MAX1.XMIN, V_MAX1.XMAX, 0, 180);
    posPoignetRot = map(moyenneY, V_MAX1.YMIN, V_MAX1.YMAX, 0, 180);
    posPoignetVer = map(moyenneX2, V_MAX2.XMIN, V_MAX2.XMAX, 0, 180);
    posPince      = map(moyenneY2, V_MAX2.YMIN, V_MAX2.YMAX, 25, 90);
    
    //sature en cas de valeurs trop importantes pour proteger les moteurs
    if (posCoude > 180) posCoude = 180;
    if (posCoude < 0)   posCoude = 0;
    
    if (posPoignetRot > 180) posPoignetRot = 180;
    if (posPoignetRot < 0)   posPoignetRot = 0;
    
    if (posPoignetVer > 180) posPoignetVer = 180;
    if (posPoignetVer < 0)   posPoignetVer = 0;
    
    if (posPince > 90) posPince = 90;
    if (posPince < 25) posPince = 25;
}
