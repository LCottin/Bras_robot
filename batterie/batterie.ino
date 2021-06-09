/*************
 * Ce code permet d'allumer les led en fonction de 
 * la charge de la batterie
 */

//Associe une led à un pin
enum led {ROUGE = 3, 
          JAUNE1 = 5, JAUNE2 = 6, 
          VERT1 = 9, VERT2 = 10, VERT3 = 11};

//Stocke les pins des leds
const byte pinLed[6] = {3, 5, 6, 9, 10, 11}

//Pin de lecture de la tension de la batterie
const byte tension = A0;

//Différents paliers pour allumer ou non les led
const short paliers[7] = {827, 850, 873, 896, 920, 943, 966}


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
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
  short lecture = analogRead(tension);

  Serial.println(lecture);

  if(lecture < palier[0])
  {
    //Toutes les led sont éteintes
    for (int i = 0; i < 6; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= palier[0] )&& (lecture < palier[1]))
  {
    //La premiere led s'allume progressivement
    analogWrite(ROUGE, map(lecture, 827, 850, 0, 255));

    for (int i = 1; i < 6; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= palier[1] )&& (lecture < palier[2]))
  {
    //La premiere led est allumé et la seconde s'allume progressivement
    digitalWrite(ROUGE, HIGH);

    analogWrite(JAUNE1, map(lecture, 850, 873, 0, 255));

    for (int i = 2; i < 6; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= palier[2] )&& (lecture < palier[3]))
  {
    //Les 2 premières led sont allumées et la 3e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    
    analogWrite(JAUNE2, map(lecture, 873, 896, 0, 255));

    for (int i = 3; i < 6; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= palier[3] )&& (lecture < palier[4]))
  {
    //Les 3 premières led sont allumées et la 4e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    digitalWrite(JAUNE2, HIGH);

    analogWrite(9, map(lecture, 896, 920, 0, 255));

    for (int i = 4; i < 6; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= palier[4] )&& (lecture < palier[5]))
  {
    //Les 4 premières led sont allumées et la 5e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    digitalWrite(JAUNE2, HIGH);
    digitalWrite(VERT1, HIGH);

    analogWrite(VERT2, map(lecture, 920, 943, 0, 255));

    for (int i = 5; i < 6; i++)
    {
      digitalWrite(pinLed[i], LOW);
    }
  }

  else if((lecture >= palier[5] )&& (lecture < palier[6]))
  {
    //Les 5 premières led sont allumées et la 6e s'allume progressivement
    digitalWrite(ROUGE, HIGH);
    digitalWrite(JAUNE1, HIGH);
    digitalWrite(JAUNE2, HIGH);
    digitalWrite(VERT1, HIGH);
    digitalWrite(VERT2, HIGH);

    analogWrite(VERT3, map(lecture, 943, 966, 0, 255));
  }

  else if(lecture >= palier[6])
  {
    //Toutes les led sont allumées
    for (int i = 0; i < 6; i++)
    {
      digitalWrite(pinLed[i], HIGH);
    }
  }
}