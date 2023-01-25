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
#define DEBUG

#ifdef DEBUG // si DEBUG activé
#include <ArduinoLog.h>
// #define DEBUG_VOID
#define DEBUG_LOOP
// #define DEBUG_LOOP_HARD
// #define DEBUG_POT
// #define DEBUG_RELAIS
// #define DEBUG_LECTURE
// #define DEBUG_BOUTON
#define DEBUG_MOTEUR
#define DEBUG_MOTEUR_HARD
bool count_motor = false;
// #define DEBUG_VALEURS
#define DEBUG_DIFF
#define DEBUG_CONST
#define DEBUG_PID

bool counter = true;
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
#include "const_set.h"

// *******************************************************************************************************
// ****************************************** boucle setup ***********************************************
// *******************************************************************************************************
void setup()
{
#ifdef DEBUG // si DEBUG activé
  Serial.begin(115200);
  // Pass log level, whether to show log level, and print interface.
  // Available levels are:
  // LOG_LEVEL_SILENT, LOG_LEVEL_FATAL, LOG_LEVEL_ERROR, LOG_LEVEL_WARNING, LOG_LEVEL_INFO, LOG_LEVEL_TRACE, LOG_LEVEL_VERBOSE
  // Note: if you want to fully remove all logging code, uncomment #define DISABLE_LOGGING in Logging.h
  //       this will significantly reduce your project size
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.notice(F(CR "Hlabs controle de volume et de gain" CR));
  Log.notice("par Clément SAILLANT" CR);
  Log.notice("initialisation des variables" CR);
  Log.notice("--------------------------------------------------" CR);
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
    save_pot(i);    // sauvegarde position potentiomètre
    state_pot_change[i] = true;
  }
  bouton_set();                // lecture et controle des boutons
  valeurs_set();               // controle des valeurs et des relais
  last_change_time = millis(); // sauvegarde du temps du dernier changement de position

#ifdef PID

  myPID.SetMode(myPID.Control::automatic);
  // myPID.SetMode(myPID.Control::manual);
  // myPID.SetSampleTimeUs(pid_time);
  myPID.SetOutputLimits(pid_limit_min, pid_limit_max); // Limit the PID output this is important to get rid of integral windup!
  // myPID.setBias(pid_bias / 2.0);                  // Set the bias to 127.5, this is the center of the output range
  // myPID.SetAntiWindupMode(myPID.iAwMode::iAwCondition); // Set the anti-windup mode to "do nothing"
#endif
#ifdef AUTOPID
  myPID.setBangBang(ECRART_V_STOP);
  myPID.setTimeStep(PID_TimeStep);
#endif

#ifdef DEBUG // si DEBUG activé
  Log.notice(F("FIN DE SETUP" CR));
  debug();
#endif
#ifdef DEBUG_CONST
  Log.notice(F("CONST OUT SET ON" CR));
  const_out_L_state = true;
#endif
}

// *******************************************************************************************************
// ****************************************** boucle principale ******************************************
// *******************************************************************************************************
void loop()
{

#ifdef DEBUG                                 // si DEBUG activé
  if (last_time >= interval_loop + millis()) // si intervalle de boucle atteint
  {
    Serial.print(".");
    Serial.print(debug_count);
    debug_count++;
    debug();
    last_time = millis(); // remise à zéro du compteur de temps
  }
#endif

  bouton_set(); // lecture et controle des boutons
  // *******************************************************************************************************
  // ********************************* boucle de lecture des potentiomètres ********************************
  // *******************************************************************************************************
  for (int i = 0; i <= 3; i++)
  {
    lecture_pot(i);                  // lecture analogique potentiomètre avec mise à jour du flag de changement de potentiomètre
    if (state_pot_change[i] == true) // si changement de position d'un potentiomètre
    {
      valeurs_set(); // controle des valeurs et des relais
    }

    // *******************************************************************************************************
    // ********************************* boucle de controle des moteurs **************************************
    // *******************************************************************************************************
    if (stereo_link_state == true || const_out_L_state == true || const_out_R_state == true) // si un des boutons est enfoncé
    {
      if (motor_change[i] != state_pot_change[i] && last_change_time + bounce_time_pot >= millis()) // si pas changement de position du moteur en cours et temps depuis changement de position potentiomètre supérieur au temps de rebond
      {
#ifdef DEBUG_LOOP // si DEBUG activé
        Log.warning(F("*** moteur %d nedd to go ***" CR), i);
        Log.notice(F("motor_change[%d] = %d" CR), i, motor_change[i]);
        Log.notice(F("state_pot_change[%d] = %d" CR), i, state_pot_change[i]);
        Log.notice(F("bounce_time_pot = %d" CR), bounce_time_pot);
        Log.notice(F("last_change_time = %d" CR), last_change_time);
        Log.notice(F("last_change_time + bounce_time_pot = %d" CR), last_change_time + bounce_time_pot);
        Log.notice(F("                          millis() = %d" CR), millis());
#endif
        lecture_pot(i); // lecture analogique potentiomètre
        if (motor_change[0] != true && motor_change[1] != true && motor_change[2] != true && motor_change[3] != true)
        {
          consigne_set(); // controle des consignes avec controle des valeurs et des relais
        }
      }
      if (motor_change[i] == true) // si changement de position du moteur en cours
      {
#ifdef DEBUG_CONST // si DEBUG activé
        if (count_motor == true)
        {

          Log.warning(F("*** changement de position du moteur %d ***" CR), i);
          // pidcontroller.start();
        }
#endif
        moteur_set(i); // controle des moteurs
      }
    }
    // *******************************************************************************************************
    // ********************************* boucle de controle des boutons **************************************
    // *******************************************************************************************************
    if (state_button_change == true) // si changement d'état d'un bouton
    {
#ifdef DEBUG_LOOP // si DEBUG activé
      Log.warning(F(CR "*** changement de bouton ***" CR));
#endif
      diff_set();                  // entregistrement des différences de gain et de volume
      state_button_change = false; // remise à zéro du flag de changement de bouton
    }

    // *******************************************************************************************************
    // ********************************* boucle de sauvegarde des potentiomètre ******************************
    // *******************************************************************************************************
    save_pot(i); // sauvegarde position potentiomètre
  }
}
