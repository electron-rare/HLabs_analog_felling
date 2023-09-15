/*
Hlabs controle de volume et de gain pour préampli stéréo avec fonction de contrôle constant output

https://www.hlabs.audio
https://www.facebook.com/hlabs.audio
contact@hlabs.audio

code par Clément SAILLANT pour Hlabs Audio- 2023
c.saillant@gmail.com
0625334420
*/

#include <Arduino.h>
#include <EEPROM.h>

// *******************************************************************************************************
// ******************** pour activer ou non le DEBUG *****************************************************
// ******************** // #define DEBUG = INACTIF *******************************************************
// ******************** #define DEBUG = ACTIF ************************************************************
// *******************************************************************************************************

// #define DEBUG

#ifdef DEBUG           // si DEBUG activé
#include "avr8-stub.h" // avr debug stub
#include "app_api.h"   // only needed with flash breakpoints
#endif

// code pour breakpoint :

/*
#ifdef DEBUG // si DEBUG activé
  breakpoint();
#endif
*/

// *******************************************************************************************************
// ****************************************** include ***************************************************
// *******************************************************************************************************
#include "variables.h"
#include "pot.h"
#include "moteur.h"
#include "calibration.h"
#include "bouton.h"
#include "relais.h"
#include "valeurs.h"
#include "consigne.h"

// *******************************************************************************************************
// ****************************************** boucle setup ***********************************************
// *******************************************************************************************************
void setup()
{
#ifdef DEBUG // si DEBUG activé
  // initialize GDB stub
  debug_init();
#endif

  // set bouton const_out_L
  pinMode(const_out_sw_L, INPUT_PULLUP);
  pinMode(const_out_L_led, OUTPUT);
  const_out_L.attach(const_out_sw_L);
  const_out_L.interval(interval_button);
  const_out_L.setPressedState(LOW);
  // set bouton const_out_R
  pinMode(const_out_sw_R, INPUT_PULLUP);
  pinMode(const_out_R_led, OUTPUT);
  const_out_R.attach(const_out_sw_R);
  const_out_R.interval(interval_button);
  const_out_R.setPressedState(LOW);
  // set bouton stereo_link
  pinMode(stereo_link_sw, INPUT_PULLUP);
  pinMode(stereo_link_led, OUTPUT);
  stereo_link.attach(stereo_link_sw);
  stereo_link.interval(interval_button);
  stereo_link.setPressedState(LOW);

  // set motor stop
  for (int i = 0; i <= 3; i++)
  {
    motor[i].stop();
    delay(50);
    lecture_pot(i); // lecture analogique potentiomètre
    valeurs_set(i); // controle des valeurs et des relais
    save_pot(i);    // sauvegarde position potentiomètre
    position_set[i] = position_lue[i];
  }
  // calibration des potentiomètres avec bouton stereo_link enfoncé au démarrage
  if (digitalRead(stereo_link_sw) == LOW)
  {
    for (int i = 0; i <= 3; i++)
    {
      calibration_pot(i);
    }
    digitalWrite(const_out_L_led, LOW);
    digitalWrite(const_out_R_led, LOW);
    digitalWrite(stereo_link_led, LOW);
    stereo_link_state = false; // initialisation de l'état du bouton stereo_link en cas de démarrage avec le bouton enfoncé pour la calibration
  }
  // lecture des valeurs de calibration enregistrées dans la mémoire EEPROM
  for (int i = 0; i <= 3; i++)
  {
    max_pot[i] = EEPROM.read(i + 10) * 4;
    if (max_pot[i] == 0)
    {
      max_pot[i] = 1023;
    }
    min_pot[i] = (EEPROM.read(i) * 4) + 1;
  }
#ifdef DEBUG // si DEBUG activé
  breakpoint();
#endif
  bouton_set(); // lecture et controle des boutons
}

// *******************************************************************************************************
// ****************************************** boucle principale ******************************************
// *******************************************************************************************************
void loop()
{
  bouton_set();                    // lecture et controle des boutons
  if (state_button_change == true) // si changement d'état d'un bouton
  {
    button_position_save();      // enregistrement de position des potentiomètres
    state_button_change = false; // remise à zéro du flag de changement de bouton
  }
  // *******************************************************************************************************
  // ********************************* boucle de lecture des potentiomètres ********************************
  // *******************************************************************************************************
  for (int i = 0; i <= 3; i++)
  {
    // *******************************************************************************************************
    // ********************************* boucle de controle des boutons **************************************
    // *******************************************************************************************************
    lecture_pot(i);                 // lecture analogique potentiomètre avec mise à jour du flag de changement de potentiomètre
    if (position_change[i] == true) // si changement de position d'un potentiomètre
    {
      if (stereo_link_state != true && const_out_L_state != true && const_out_R_state != true) // si pas de contrôle constant output
      {
        valeurs_set(i); // controle des valeurs et des relais en mode normal
      }
      // si contrôle constant output ou stereo link avec moteur arrêté il y a plus de bounce_time_pot et mouvement supérieur à ECART_V_STOP
      else if (motor_change[i] != true && millis() > last_motor_end[i] + bounce_time_pot)
      {
        if (abs(position_lue[i] - position_set[i]) >= ECART_V_STOP)
        {
          consigne_set(i); // mise à jour des consignes
          for (int i = 0; i <= 3; i++)
          {
            valeurs_const_set(i); // controle des valeurs et des relais en mode constant output
          }
        }
        position_set[i] = position_lue[i];
      }
      save_pot(i); // sauvegarde position potentiomètre
    }
    if (motor_change[i] == true) // si moteur en mouvement
    {
      moteur_set(i); // controle du moteurs
    }
  }
  if (error_state == true)
  {
    error_led();
  }
}