
#include "Arduino.h"
#include <SparkFunMiniMoto.h>  // Include the MiniMoto library


// adresse moteur
#define vol_0_motor 0xC8 // A1 = 1, A0 = 1
#define gain_0_motor 0xCE // A1 = 1, A0 = 1
#define vol_1_motor 0xD0 // A1 = 1, A0 = clear
#define gain_1_motor 0xCA // A1 = 1, A0 = 1 

// définition PIN curseur potentiomètre
#define vol_0_ref A0
#define gain_0_ref A1
#define vol_1_ref A2
#define gain_1_ref A3

#define ECRART_V_DIV2 10// rapport PWM min pour déplacement moteur potentiomètre
#define ECRART_V_STOP 5
// Create  MiniMoto instances,
MiniMoto motor[4] = {vol_0_motor, gain_0_motor, vol_1_motor, gain_1_motor};

bool looping = 1;
int consigne[4] = {0, 0, 0, 0};
int positionlue[4] = {0, 0, 0, 0};
int positionold[4] = {0, 0, 0, 0};
int pulse_pwm[4] = {0, 0, 0, 0};
bool dir[4];

byte analog_ref[4] = {vol_0_ref, gain_0_ref, vol_1_ref, gain_1_ref};


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println ("----- START MOTOR TESTER ---------");
  delay(500);
  // put your setup code here, to run once:
  for (int i = 0; i <= 3; i++)
  {
    motor[i].stop();
    delay(500);
    positionlue[i] = analogRead(analog_ref[i]);
    positionold[i] = positionlue[i];
    Serial.print("Motor ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println (positionlue[i]);
    delay(200);
  }
  Serial.println (" --------------- SETUP END ------------------- ");
}



void direction (int i)
{

  if (dir[i] == 0)
  {
    motor[i].drive(pulse_pwm[i]);
    /*
        Serial.print("Motor ");
        Serial.print(i);
        Serial.print(" position : ");
        Serial.print(positionlue[i]);
        Serial.print(" drive at : ");
        Serial.println (pulse_pwm[i]);
    */
  }
  if (dir[i] == 1)
  {
    motor[i].drive(-pulse_pwm[i]);
    /*
        Serial.print("Motor ");
        Serial.print(i);
        Serial.print(" position : ");
        Serial.print(positionlue[i]);
        Serial.print(" drive at : ");
        Serial.println (-pulse_pwm[i]);
    */
  }
}


void allerA (int i)
{
  Serial.print("Motor ");
  Serial.print(i);
  Serial.print(" vas a : ");
  Serial.println (consigne[i]);
  positionlue[i] = analogRead(analog_ref[i]);
  motor[i].brake();
  motor[i].stop();

  if (consigne[i] == positionlue[i])    return;
  // if (consigne[i] <= positionlue[i] - ECRART_V_STOP) return;
  // if (consigne[i] >= positionlue[i] + ECRART_V_STOP) return;
  if (consigne[i] > 1023) return;
  if (consigne[i] < 0) return;


  if (consigne[i] > positionlue[i])
  {
    dir[i] = 0; // set direction

    if (positionlue[i] < (consigne[i] - ECRART_V_DIV2 )) {
      positionlue[i] = analogRead (analog_ref[i]);
      pulse_pwm[i] = 63;
      direction(i);

      while (positionlue[i] < (consigne[i] - ECRART_V_DIV2)) {
        //motor[i].stop();
        positionlue[i] = analogRead (analog_ref[i]);
        pulse_pwm[i] = 35;
        direction(i);
      }
      while (positionlue[i] < consigne[i] )
      {
        positionlue[i] = analogRead (analog_ref[i]);
        pulse_pwm[i] = 15;
        direction(i);
      }
    }
  }



  else if (consigne[i] < positionlue[i])
  {
    dir[i] = 1; // set direction

    if (positionlue[i] > (consigne[i] + ECRART_V_DIV2 ))
      positionlue[i] = analogRead (analog_ref[i]);
    pulse_pwm[i] = 63;
    direction(i);

    while (positionlue[i] > (consigne[i] + ECRART_V_DIV2)) {
      positionlue[i] = analogRead (analog_ref[i]);
      pulse_pwm[i] = 35;
      direction(i);
    }
    while (positionlue[i] > consigne[i]) {
      positionlue[i] = analogRead (analog_ref[i]);
      pulse_pwm[i] = 15;
      direction(i);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (looping == 1) {
    Serial.println (" --------------- LOOP START ------------------- ");
  }
  for (int i = 0; i <= 3; i++)
  {
    positionlue[i] = analogRead(analog_ref[i]);
    if (looping == 1) {
      Serial.print("Motor ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println (positionlue[i]);
    }
  }
  if (looping == 1) {
    Serial.println (" --------------------------------------------- ");
    looping = 0;
  }
  consigne[0] = positionlue[0];
  positionold[0] = consigne[0];
  for (int i = 0; i <= 3; i++)

  {

    if (positionold[i] != positionlue[0]) {
      for (int i = 0; i <= 3; i++)
      {
        positionlue[i] = analogRead(analog_ref[i]);
      }
      consigne[i] = positionlue[0];
      if (consigne[i] < (positionlue[i] - ECRART_V_STOP-5)  || consigne[i] > (positionlue[i]  + ECRART_V_STOP+5)  )
      {
        looping = 1;
        Serial.print("Motor ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.println (positionlue[i]);
        Serial.print(" dois aller à : ");
        Serial.println (consigne[i]);
        allerA (i);
        motor[i].brake();
        motor[i].stop();
      }
      else {
        motor[i].brake();
        motor[i].stop();
        positionlue[i] = analogRead (analog_ref[i]);
        if (looping == 1) {
          Serial.println("Position OK");
          Serial.print("Pos = ");
          Serial.println(positionlue[i]);
        }
      }
      positionlue[i] = analogRead(analog_ref[i]);
    }
    positionold[i] = positionlue[i];
  }
}