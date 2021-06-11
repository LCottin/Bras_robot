/*************
 * Ce code permet d'allumer les led en fonction de 
 * la charge de la batterie
 */

//Valeurs extremes : utiles pour la calibration
const short MINI  = 639;
const short MAXI  = 735;
const byte  NBLED = 6;
const short pas   = (MAXI - MINI) / NBLED;

//Différents paliers pour allumer ou non les led
const short paliers[NBLED + 1] = {MINI, MINI + pas, MINI + 2*pas, MINI + 3*pas, MINI + 4*pas, MINI + 5*pas, MAXI};

//Associe une led à un pin
enum led {ROUGE = 3, 
          JAUNE1 = 5, JAUNE2 = 6, 
          VERT1 = 9, VERT2 = 10, VERT3 = 11};

//Stocke les pins des leds
const byte pinLed[NBLED] = {3, 5, 6, 9, 10, 11};

//Pin de lecture de la tension de la batterie
const byte tension = A0;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
  Serial.begin(9600);
  Serial.print(__FILE__); Serial.println(" initialising ...");
  
  for(int i = 0; i < 6; i++)
  {
    pinMode(pinLed[i], OUTPUT);
  }

  for (int i = 0; i < NBLED; i++)
  {
    digitalWrite(pinLed[i], HIGH);
    delay(50);
  }

  delay(100);

  for (int i = 0; i < NBLED; i++)
  {
    digitalWrite(pinLed[i], LOW);
    delay(50);
  }

  delay(1000);
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
  short lecture = analogRead(A0);

  Serial.println(lecture);

  if(lecture < paliers[0])
  {
    //Toutes les led sont éteintes
    for (int i = 0; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= paliers[0] )&& (lecture < paliers[1]))
  {
    //La premiere led s'allume progressivement
    analogWrite(ROUGE, map(lecture, paliers[0], paliers[1], 0, 255));

    for (int i = 1; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= paliers[1] )&& (lecture < paliers[2]))
  {
    //La premiere led est allumé et la seconde s'allume progressivement
    digitalWrite(ROUGE, HIGH);

    analogWrite(JAUNE1, map(lecture, paliers[1], paliers[2], 0, 255));

    for (int i = 2; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= paliers[2] )&& (lecture < paliers[3]))
  {
    //Les 2 premières led sont allumées et la 3e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    
    analogWrite(JAUNE2, map(lecture, paliers[2], paliers[3], 0, 255));

    for (int i = 3; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= paliers[3] )&& (lecture < paliers[4]))
  {
    //Les 3 premières led sont allumées et la 4e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    digitalWrite(JAUNE2, HIGH);

    analogWrite(JAUNE1, map(lecture, paliers[3], paliers[4], 0, 255));

    for (int i = 4; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= paliers[4] )&& (lecture < paliers[5]))
  {
    //Les 4 premières led sont allumées et la 5e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    digitalWrite(JAUNE2, HIGH);
    digitalWrite(VERT1, HIGH);

    analogWrite(VERT2, map(lecture, paliers[4], paliers[5], 0, 255));

    for (int i = 5; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= paliers[5] )&& (lecture < paliers[6]))
  {
    //Les 5 premières led sont allumées et la 6e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    digitalWrite(JAUNE2, HIGH);
    digitalWrite(VERT1, HIGH);
    digitalWrite(VERT2, HIGH);

    analogWrite(VERT3, map(lecture, paliers[5], paliers[6], 0, 255));
  }

  else if(lecture >= paliers[6])
  {
    //Toutes les led sont allumées
    for (int i = 0; i < NBLED; i++)
    {
      digitalWrite(pinLed[i], HIGH);
    }
  }
}