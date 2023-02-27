/*
Hlabs controle de volume et de gain
par Clément SAILLANT
c.saillant@gmail.com
02/2023
*/

/*
tableau de valeurs (i) pour les relais, les valeurs de potentiomètre et les valeurs de lissage
0 : vol_0   volume gauche
1 : gain_0  gain gauche
2 : vol_1   volume droite
3 : gain_1  gain droite
*/
#include <Arduino.h>

// *******************************************************************************************************
// ******************** pour activer ou non le DEBUG *****************************************************
// ******************** // #define DEBUG = INACTIF *******************************************************
// ******************** #define DEBUG = ACTIF ************************************************************
// *******************************************************************************************************
// #define DEBUG

#ifdef DEBUG // si DEBUG activé
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#endif

// *******************************************************************************************************
// **************************** Activation ou non LEFT & RIGHT *******************************************
// *******************************************************************************************************
#define ENABLE_LEFT
// #define ENABLE_RIGHT

// *******************************************************************************************************
// ****************************************** include ***************************************************
// *******************************************************************************************************
#include "variables.h"
#include "pot.h"
#include "moteur.h"
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

  // set LED output
  pinMode(const_out_L_led, OUTPUT);
  pinMode(const_out_R_led, OUTPUT);
  pinMode(stereo_link_led, OUTPUT);
  // set bouton const_out_L
  pinMode(const_out_sw_L, INPUT_PULLUP);
  const_out_L.attach(const_out_sw_L);
  const_out_L.interval(interval_button);
  const_out_L.setPressedState(LOW);
  // set bouton const_out_R
  pinMode(const_out_sw_R, INPUT_PULLUP);
  const_out_R.attach(const_out_sw_R);
  const_out_R.interval(interval_button);
  const_out_R.setPressedState(LOW);
  // set bouton stereo_link
  pinMode(stereo_link_sw, INPUT_PULLUP);
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
  bouton_set(); // lecture et controle des boutons
  //pid_setup();  // initialisation des PID
}

// *******************************************************************************************************
// ****************************************** boucle principale ******************************************
// *******************************************************************************************************
void loop()
{
  // *******************************************************************************************************
  // ********************************* boucle de controle des boutons **************************************
  // *******************************************************************************************************
  bouton_set();                    // lecture et controle des boutons
  if (state_button_change == true) // si changement d'état d'un bouton
  {
    button_position_save();      // entregistrement de position des potentiomètres
    state_button_change = false; // remise à zéro du flag de changement de bouton
  }
  // *******************************************************************************************************
  // ********************************* boucle de lecture des potentiomètres ********************************
  // *******************************************************************************************************
  for (int i = 0; i <= 3; i++)
  {
    lecture_pot(i);                                            // lecture analogique potentiomètre avec mise à jour du flag de changement de potentiomètre
    if (position_change[i] == true && motor_change[i] != true) // si changement de position d'un potentiomètre
    {
      if (stereo_link_state != true && const_out_L_state != true && const_out_R_state != true) // si pas de contrôle constant output
      {
        valeurs_set(i); // controle des valeurs et des relais
        if (motor_change[i] == true)
        {
          moteur_stop(i); // arret du moteur
          position_set[i] = position_lue[i];
          motor_change[i] = false;
          save_pot(i); // sauvegarde position potentiomètre
        }
        else
        {
          save_pot(i); // sauvegarde position potentiomètre
        }
      }
      else // si controle constant output
      {
        current_time = millis(); // enregistrement du temps actuel
        if (motor_change[i] != true)
        {
          consigne_set();       // mise à jour des consignes
          valeurs_const_set(i); // controle des valeurs et des relais
          save_pot(i);          // sauvegarde position potentiomètre
        }
      }
    }
    if (motor_change[i] == true)
    {
      if (position_set[i] > position_lue[i] + ECART_V_STOP || position_set[i] < position_lue[i] - ECART_V_STOP)
      {
        moteur_set(i); // controle des moteurs
      }
      else
      {
        moteur_stop(i); // arret du moteur
        motor_change[i] = false;
        save_pot(i); // sauvegarde position potentiomètre
      }
    }
  }
}
