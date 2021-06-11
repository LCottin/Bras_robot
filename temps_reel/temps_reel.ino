/**
 * Ce code permet de faire bouger le bras avec l'accéléromètre en temps réel
 */

#include "Braccio.h"
//#include <SoftwareSerial.h>

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


//Valeurs extremes recues pour la radio 1
struct V_MAX1
{
    const short XMIN = 265;
    const short XMAX = 415;
    const short XMOY = (XMIN + XMAX)/2;
    
    const short YMIN = 260; 
    const short YMAX = 405;
    const short YMOY = (YMIN + YMAX)/2;

    /*
    const short ZMIN = 265;
    const short ZMAX = 410;
    const short ZMOY = (ZMIN + ZMAX)/2;
    */
} V_MAX1; 

//Valeurs extremes en analogread pour la radio 2
struct V_MAX2
{
    const short XMIN = 270;
    const short XMAX = 415;
    const short XMOY = (XMIN + XMAX)/2;
      
    const short YMIN = 265;
    const short YMAX = 415;
    const short YMOY = (YMIN + YMAX)/2;
    /*
    const short ZMIN = 270;
    const short ZMAX = 420;
    const short ZMOY = (ZMIN + ZMAX)/2;
    */
} V_MAX2;

//Valeurs extremes recues pour la radio 3
struct V_MAX3
{
    const short XMIN = 270;
    const short XMAX = 415;
    const short XMOY = (XMIN + XMAX)/2;
      
    const short YMIN = 260;
    const short YMAX = 405;
    const short YMOY = (YMIN + YMAX)/2;
    /*
    const short ZMIN = 270;
    const short ZMAX = 420;
    const short ZMOY = (ZMIN + ZMAX)/2;
    */
} V_MAX3;

// les buffers echantillons pour le moyennage
const byte N = 5;
unsigned short cmp = 0;
short x1[N];
short y1[N];

short x2[N];
short y2[N];

short x3[N];
short y3[N];

struct data 
{
  short id;
  short xAxis;
  short yAxis;
  //short zAxis; //non utilisé
} receive_data;

char buff[20]; //buffer temporaire pour le décodage des données


// ---------------------------------------- //
// -                 SETUP                - //
// ---------------------------------------- //
void setup() 
{  
    Serial.begin(115200);
    Serial.print("Initialisation de ");
    Serial.println(__FILE__);
    
    /* ROUTINE D'INITIALISATION DU BRAS*/ 
    Braccio.begin();  
    delay(100);
    Braccio.positionDroite();
  
    Serial.print(__FILE__);
    Serial.println(" Prêt !");
    initBufferEchantillons();
    Serial.flush();
}


// ---------------------------------------- //
// -                  LOOP                - //
// ---------------------------------------- //
void loop() 
{
    buff[0] = 0;
    char i = 0;
    const byte vitesse  = T_RAPIDE;
    const short latence = 0;
    
    Serial.readBytesUntil('#', buff, 13);
    
    if(buff[0]){
      //Serial.flush();
      //Serial.println(buff);
      miseEnFormeDonnee(&receive_data, buff);
      miseEnForme();
      Braccio.ServoMovement(vitesse, posBase, posEpaule, posCoude, posPoignetRot, posPoignetVer, posPince);
    }
    
    delay(latence);
}

// ---------------------------------------- //
// -      MISE EN FORME DES DONNEES       - //
// ---------------------------------------- //

void miseEnFormeDonnee(struct data* data, char* buff)
{
  int j = 0;
  int tmp;
  short test;
  char buffinter[] = {0, 0, 0, 0, 0, 0};
  char* pos_debut;
  char* pos_fin;
  
  if(buff[1] != ';' || buff[5] != ';')
  {
    //Serial.println("Pb de transmission ou pas de donnée transmis");
    
  }
  else
  {
    // 1er champ : id
    buffinter[0] = buff[0];
    buffinter[1] = 0;
    test = atoi(buffinter);
    if (test > 4 || test < 1)
    {
      //Serial.println("Pb sur l'id");
      return;
    }
    else
    {
      data->id = test;
    }

    // 2eme champ : x
    pos_debut = strchr(buff, ';');
    pos_debut++;
    pos_fin = strchr(pos_debut, ';');
    strncpy(buffinter, pos_debut, pos_fin - pos_debut);
    test = atoi(buffinter);
    if (test > 450 || test < 250)
    {
      //Serial.println("Pb sur x");
      return;
    }
    else
    {
      data->xAxis = test;
    }

    // 3eme champ : y
    pos_debut = pos_fin + 1;
    strncpy(buffinter, pos_debut, 3);
    test = atoi(buffinter);

    if (test > 450 || test < 250)
    {
      //Serial.println("Pb sur y");
      return;
    }
    else
    {
      data->yAxis = test;
    }
    
    /*
    // 2eme champ : x
    //tmp = j;
    for(j = 2; buff[j] != ';'; j++)
    {
      buffinter[j - 2] = buff[j];
    }
    buffinter[3] = 0;
    test = atoi(buffinter);

    if (test > 450 || test < 250)
    {
      //Serial.println("Pb sur x");
      return;
    }
    else
    {
      data->xAxis = test;
    }
    
    // 3eme champ : y
    tmp = j + 1;
    for(j=j+1; buff[j] != ';'; j++)
    {
      buffinter[j - tmp] = buff[j];
    }
    buffinter[tmp] = 0;
    test = atoi(buffinter);

    if (test > 450 || test < 250)
    {
      //Serial.println("Pb sur y");
      return;
    }
    else
    {
      data->yAxis = test;
    }
    
    // 4eme champ : z
    
    tmp = j + 1;
    for(j=j+1; j < strlen(buff); j++)
    {
      buffinter[j - tmp] = buff[j];
    }
    buffinter[tmp] = 0;
    test = atoi(buffinter);

    if (test > 450 || test < 250)
    {
      //Serial.println("Pb sur z");
      return;
    }
    else
    {
      data->zAxis = test;
    }
    */
    
  }
  
  /*
  Serial.print("Bracelet n°");
  Serial.println(data->id);
  Serial.println("Données reçu ");
  Serial.print("En x : ");
  Serial.println(data->xAxis);
  Serial.print("En y : ");
  Serial.println(data->yAxis);
  Serial.println("");
  Serial.print("En z : ");
  Serial.println(data->zAxis);
  Serial.println("");
  */
  
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
      
      x2[i] = V_MAX2.XMOY;
      y2[i] = V_MAX2.XMOY;

      x3[i] = V_MAX3.XMOY;
      y3[i] = V_MAX3.XMOY;
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
    
    short moyenneX2 = 0;
    short moyenneY2 = 0;
    
    short moyenneX3 = 0;
    short moyenneY3 = 0;

    //lectures des 4 entrées et maj du buffer
    switch(receive_data.id)
    {
        case 1 :
          x1[cmp] = receive_data.xAxis;
          y1[cmp] = receive_data.yAxis;
          break;
        case 2 :
          x2[cmp] = receive_data.xAxis;
          y2[cmp] = receive_data.yAxis;
          break;
        case 3 :
          x3[cmp] = receive_data.xAxis;
          y3[cmp] = receive_data.yAxis;
          break;
        //case 4 :
        default :
          //return;
          break;
    }
    
    if ((x1[cmp] == 0 && y1[cmp] == 0) || (x2[cmp] == 0 && y2[cmp] == 0) || (x3[cmp] == 0 && y3[cmp] == 0))
    {
        Braccio.positionDroite();
    }
    
    else 
    {
        //moyennage des valeurs
        for(int i = 0; i < N; i++)
        {
          moyenneX  += x1[i];
          moyenneY  += y1[i];
          
          moyenneX2 += x2[i];
          moyenneY2 += y2[i];

          moyenneX3 += x3[i];
          moyenneY3 += y3[i];
        }

        moyenneX  /= N;
        moyenneY  /= N;

        moyenneX2 /= N;
        moyenneY2 /= N;

        moyenneX3 /= N;
        moyenneY3 /= N;
        
        //recupere les valeurs émises
        posCoude      = map(moyenneX, V_MAX1.XMIN, V_MAX1.XMAX, 0, 180);
        posPoignetRot = map(moyenneY, V_MAX1.YMIN, V_MAX1.YMAX, 0, 180);
        posPoignetVer = map(moyenneX2, V_MAX2.XMIN, V_MAX2.XMAX, 0, 180);
        posPince      = map(moyenneY2, V_MAX2.YMIN, V_MAX2.YMAX, 25, 90);
        posBase       = map(moyenneX3, V_MAX2.XMIN, V_MAX2.XMAX, 0, 180);
        posEpaule     = map(moyenneY3, V_MAX2.YMIN, V_MAX2.YMAX, 15, 165);
        
        //sature en cas de valeurs trop importantes pour proteger les moteurs
        if (posCoude > 180) posCoude = 180;
        if (posCoude < 0)   posCoude = 0;
        
        if (posPoignetRot > 180) posPoignetRot = 180;
        if (posPoignetRot < 0)   posPoignetRot = 0;
        
        if (posPoignetVer > 180) posPoignetVer = 180;
        if (posPoignetVer < 0)   posPoignetVer = 0;
        
        if (posPince > 90) posPince = 90;
        if (posPince < 25) posPince = 25;

        if (posBase > 180) posBase = 180;
        if (posBase < 0)   posBase = 0;

        if (posEpaule > 165) posEpaule = 165;
        if (posEpaule < 15)  posEpaule = 15;

        //Serial.print("posCoude : ");
        //Serial.println(moyenneX2);
        //Serial.println(moyenneX);
    }
}
