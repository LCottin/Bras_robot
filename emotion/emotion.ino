/**
 * Ce code permet de controler le bras robot avec les 4 phrases écrites
**/

#include "Braccio.h"
//#include <Servo.h>

//bibliothèques pour la comminucation sans fils
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

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
byte posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince;

//definition des vitesses
enum VITESSE {T_LENT = 30, LENT = 25, MOYEN = 20, RAPIDE = 15, T_RAPIDE = 10};
byte vitesse;

//Pour mesurer le temps d'execution d'un programme
unsigned long tempsDebut, tempsFin;
double duree;
byte i = 0;

//Pour la module SF
//RF24 radio(2, 4);
const byte adresse[6] = "00001";
struct data 
{
    int xAxis;
    int yAxis;
    int zAxis;        
};
data receive_data;

struct valeur
{
  int XMAX, XMIN;
  int YMIN, YMAX;
  int ZMIN, ZMAX;
};

valeur valeurs;



/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
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
  
    init1();
  
    Serial.print(__FILE__);
    Serial.println(" Prêt !");

/* ---------------------------------- METTRE LE CODE A EFFECTUER ICI ----------------------------------------*/
   
    surprise();
    delay(3000);
    joie();



/* -------------------------------------------------------------------------------------------------------------- */
}    

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
void loop() 
{
}

void init1()
{
    posBase       = 90;
    posEpaule     = 95;
    posCoude      = 95; 
    posPoignetRot = 90;
    posPoignetVer = 90;
    posPince      = 90;
}

void test1()
{
  vitesse = T_LENT;
  Serial.println("Changement");
                      init1();
                      mainOuverte(vitesse);
                      delay(500);
                      mainFermee(vitesse);
                      delay(500);
                      Braccio.positionDroite();
                      tournerMain(0, vitesse);
                      delay(500);
                      tournerMain(180, vitesse);
                      delay(500);
                      tournerMain(90, vitesse);
                      delay(500);
                    
                      leverMain(20, vitesse);
                      delay(500);
                      tournerMain(0, vitesse);
                      delay(500);
                      leverMain(90, vitesse);
                      delay(500);
                    
                      tournerCoude(20, vitesse);
                      leverMain(180, vitesse);
                      Braccio.positionDroite();
                      tournerBase(60, vitesse);
                      delay(500);
                    
                      delay(1000);
                      Braccio.positionDroite();

                      delay(1000);
      vitesse = MOYEN;
                      init1();
                      mainOuverte(vitesse);
                      delay(500);
                      mainFermee(vitesse);
                      Braccio.positionDroite();
                      tournerMain(0, vitesse);
                      delay(500);
                      tournerMain(180, vitesse);
                      delay(500);
                      tournerMain(90, vitesse);
                      delay(500);
                    
                      leverMain(20, vitesse);
                      delay(500);
                      tournerMain(0, vitesse);
                      delay(500);
                      leverMain(90, vitesse);
                      delay(500);
                    
                      tournerCoude(20, vitesse);
                      leverMain(180, vitesse);
                      Braccio.positionDroite();
                      tournerBase(60, vitesse);
                      delay(500);
                    
                      delay(2000);
                      Braccio.positionDroite();
                      delay(2000);

  
vitesse = T_RAPIDE;
  Serial.println("Changement");
                      init1();
                      mainOuverte(vitesse);
                      delay(500);
                      mainFermee(vitesse);
                      delay(500);
                      Braccio.positionDroite();
                      tournerMain(0, vitesse);
                      delay(500);
                      tournerMain(180, vitesse);
                      delay(500);
                      tournerMain(90, vitesse);
                      delay(500);
                    
                      leverMain(20, vitesse);
                      delay(500);
                      tournerMain(0, vitesse);
                      delay(500);
                      leverMain(90, vitesse);
                      delay(500);
                    
                      tournerCoude(20, vitesse);
                      leverMain(180, vitesse);
                      Braccio.positionDroite();
                      tournerBase(60, vitesse);
                      delay(500);
                    
                      delay(3000);
                      Braccio.positionDroite();
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/**
 * EMOTION : COLERE
 */
void colere()
{
    tempsDebut = millis();
    vitesse = MOYEN;
  
    tournerEpaule(150, MOYEN);
    delay(10);
    Braccio.ServoMovement(vitesse, posBase, 90, 20, 180, posPoignetVer, posPince);
    delay(20);
    leverMain(115, vitesse);
    delay(10);
  
    for (i = 0; i< 3; i++)
    {
        mainOuverte(vitesse);
        delay(10);
        mainFermee(vitesse);
        delay(10);
    }
    ouvrirPince(90, vitesse);
    
    tournerEpaule(30, vitesse);
    delay(10);
    Braccio.ServoMovement(vitesse, posBase, 140, 95, 95, 90, 90);
    delay(30);
    Braccio.ServoMovement(vitesse, 30, 110, 80, 150, 90, 90);
    delay(30);
    Braccio.ServoMovement(vitesse, 90, 70, 60, 90, 90, 90);
    delay(30);
    Braccio.ServoMovement(vitesse, 120, 30, 90, 90, 90, 90);
    delay(100);
  
    tournerEpaule(160, vitesse);
    delay(10);
    leverMain(150, vitesse);
    delay(50);
    Braccio.ServoMovement(vitesse, posBase, 140, 95, 95, 90, 90);
  
    tournerEpaule(95, vitesse);
    delay(10);
    tournerCoude(95, vitesse);
    delay(10);
    leverMain(10, vitesse);
    delay(10);
  
    for (i = 0; i < 4; i++)
    {
        if (i%2)
        {
            tournerBase(10, vitesse);
            delay(10);
            mainFermee(vitesse);
            delay(10);
            leverMain(80, vitesse);
            delay(100);
        }
    
        else 
        {
            tournerBase(160, vitesse);
            delay(10);
            mainOuverte(vitesse);
            delay(10);
            leverMain(60, vitesse);
            delay(100);
        }
    
        delay(50);
    }
  
    tournerBase(90, LENT);
    delay(10);
    mainFermee(MOYEN);
    delay(10);
    tournerEpaule(60, LENT);
    delay(10);
    tournerCoude(170, LENT);
    delay(10);
    leverMain(20, LENT);
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

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/**
 * EMOTION : JOIE
 */
void joie()
{
    tempsDebut = millis();
    vitesse = RAPIDE;
  
    tournerEpaule(140, MOYEN);
    delay(100);
    tournerCoude(40, vitesse);
    delay(100);
  
    for (i = 0; i < 4; i++)
    {
        leverMain(150, vitesse);
        delay(50);
        leverMain(40, vitesse);
        delay(50);
    }

    delay(1000);
    Braccio.positionDroite();
    return;
  
    leverMain(90, vitesse);
    tournerBase(170, RAPIDE);
    Braccio.ServoMovement(RAPIDE, 170, posEpaule, posCoude, 90, posPoignetVer, posPince);
    delay(50);
    Braccio.ServoMovement(RAPIDE, 30, posEpaule, posCoude, 40, posPoignetVer, posPince);
    delay(50);
  
    for (i = 0; i < 2; i++)
    {
        tournerBase(150, RAPIDE);
        delay(25);
        tournerEpaule(60, MOYEN);
        delay(25);
        tournerCoude(150, vitesse);
        delay(250);
        leverMain(60, vitesse);
        delay(25);
      
        tournerBase(40, RAPIDE);
        delay(25);
        leverMain(110, vitesse);
        delay(250);
        tournerCoude(40, vitesse);
        delay(25);
        tournerEpaule(130, MOYEN);
        delay(25);
    }
  
    tournerBase(90, vitesse);
    tournerEpaule(140, MOYEN);
    delay(25);
    tournerCoude(40, RAPIDE);
    delay(25);
  
    for (i = 0; i < 3; i++)
    {
        mainOuverte(RAPIDE);
        delay(50);
        mainFermee(RAPIDE);
        delay(50);
    }
  
    ouvrirPince(90, MOYEN);
  
    for (i = 0; i < 3; i++)
    {
        tournerBase(40, RAPIDE);
        delay(20);
        tournerBase(140, RAPIDE);
        delay(20);
    }
  
    tournerBase(90, vitesse);
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

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/**
 * EMOTION : SURPRISE
 */
void surprise()
{
  tempsDebut = millis();
  vitesse = LENT;
  
  Braccio.positionDroite();
  tournerMain(0, vitesse);
  delay(500);
  tournerMain(180, vitesse);
  delay(500);
  tournerMain(90, vitesse);
  delay(500);

  leverMain(20, vitesse);
  delay(500);
  tournerMain(0, vitesse);
  delay(500);
  leverMain(90, vitesse);
  delay(500);

  tournerCoude(20, vitesse);
  leverMain(180, vitesse);
  Braccio.positionDroite();
  tournerBase(60, vitesse);
  delay(500);

  Braccio.positionDroite();
  delay(500);
  mainOuverte(T_RAPIDE);
  delay(500);
  mainFermee(MOYEN);

  delay(1000);
  Braccio.positionDroite();

  tempsFin = millis();
  duree = tempsFin - tempsDebut;
  Serial.print("Temps d'execution de surprise = ");
  Serial.print(duree/1000);
  Serial.println("s");
}


/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* FAIT LA VAGUE EN PARTANT DE LA POSITION DROITE */
void vague()
{
  const byte vitesse = T_RAPIDE;
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

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------- */
/* TOURNE LA BASE */
void tournerBase(byte valeurAngle, const byte vitesse)
{
  if (valeurAngle < 0)    valeurAngle = 0;
  if (valeurAngle > 180)  valeurAngle = 180;
  posBase = valeurAngle;
  Braccio.ServoMovement(vitesse, valeurAngle, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------- */
/* TOURNE L'EPAULE */
void tournerEpaule(byte valeurAngle, const byte vitesse)
{
  if (valeurAngle < 20)    valeurAngle = 20;
  if (valeurAngle > 160)   valeurAngle = 160;
  posEpaule = valeurAngle;
  Braccio.ServoMovement(vitesse, posBase, valeurAngle, posCoude, posPoignetRot, posPoignetVer, posPince);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------- */
/* TOURNE LE COUDE */
void tournerCoude(byte valeurAngle, const byte vitesse)
{
  if (valeurAngle < 0)    valeurAngle = 0;
  if (valeurAngle > 180)  valeurAngle = 180;
  posCoude = valeurAngle;
  Braccio.ServoMovement(vitesse, posBase, posEpaule, valeurAngle, posPoignetRot, posPoignetVer, posPince);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------- */
/* LEVE LA MAIN */
void leverMain(byte valeurAngle, const byte vitesse)
{
  if (valeurAngle < 0)    valeurAngle = 0;
  if (valeurAngle > 180)  valeurAngle = 180;
  posPoignetRot = valeurAngle;
  Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, valeurAngle, posPoignetVer, posPince);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------- */
/* TOURNE LA MAIN */
void tournerMain(byte valeurAngle, const byte vitesse)
{
  if (valeurAngle < 0)    valeurAngle = 0;
  if (valeurAngle > 180)  valeurAngle = 180;
  posPoignetVer = valeurAngle;
  Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, valeurAngle, posPince);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------- */
/* OUVRE LA PINCE */
void ouvrirPince(byte valeurAngle, const byte vitesse)
{
  if (valeurAngle < 25)    valeurAngle = 25;
  if (valeurAngle > 90)    valeurAngle = 90;
  posPince = valeurAngle;
  Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, valeurAngle);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/**
 * OUVRE LA MAIN EN GRAND
 */
 void mainOuverte(const byte vitesse)
{
  posPince = 90;
  ouvrirPince(posPince, vitesse);
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/* ---------------------------------------- */
/**
 * FERME LA MAIN
 */
 void mainFermee(const byte vitesse)
 {
  posPince = 25;
  ouvrirPince(posPince, vitesse);
 }