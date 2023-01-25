// *******************************************************************************************************
// ****************************************** gestion moteur *********************************************
// *******************************************************************************************************
#include <SparkFunMiniMoto.h> // Include the MiniMoto library

// Create  MiniMoto instances, pour controle des moteur en I2C (DRV8830)
MiniMoto motor[4] = {gain_0_motor, vol_0_motor, gain_1_motor, vol_1_motor};

/*In this section we have defined the gain values for the
   proportional,integral, and derivative controller i have set
   the gain values with the help of trial and error methods.
*/
#define ECRART_V_STOP 5 // hystérésie de positionnement potentiomètre
#define PID
// #define AUTOPID

#ifdef PID
#include "QuickPID.h"
// Define the aggressive and conservative and POn Tuning Parameters
float aggKp = 2, aggKi = 0.5, aggKd = 0.5;
float consKp = 1, consKi = 0.05, consKd = 0.25;

#define ECRART_V_STOP 5 // hystérésie de positionnement potentiomètre
#define pid_windup 10
#define pid_limit_min 6
#define pid_limit_max 63
#define pid_bias 255.0
#define pid_time_step 1000
#define gap_setpoint 25
float pid_input, pid_output, pid_setpoint;
QuickPID myPID(&pid_input, &pid_output, &pid_setpoint);

#endif

#ifdef AUTOPID
#include <AutoPID.h>
// pid settings and gains
#define PID_TimeStep 250 // set PID update interval to 4000ms
#define OUTPUT_MIN 100
#define OUTPUT_MAX 1000
#define KP .12
#define KI .0003
#define KD 0
double motor_read, motor_set, out_speed_value;
AutoPID myPID(&motor_read, &motor_set, &out_speed_value, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);
#endif

void moteur_set(int i);
void moteur_stop(int i);
void pid_set(int i);

void pid_set(int i)
{
        // #define anal_read

#ifdef anal_read
        switch (i)
        {
        case 0:
                pid_input = analogRead(gain_0_pot);
                break;
        case 1:
                pid_input = analogRead(vol_0_pot);
                break;
        case 2:
                pid_input = analogRead(gain_1_pot);
                break;
        case 3:
                pid_input = analogRead(vol_1_pot);
                break;
        default:
                break;
        }
#endif
#ifndef anal_read
        lecture_pot(i);
        pid_input = float(position_lue[i]);
#endif
        pid_setpoint = float(position_set[i]);
        float gap = abs(pid_setpoint - pid_input); // distance away from setpoint
#ifdef DEBUG_CONST                                 // si DEBUG activé
        Log.notice(F("IIIIII = %d" CR), i);
        Log.notice(F("gap motor = %d" CR), gap);
        Log.notice(F("pid_setpoint = %d" CR), pid_setpoint);
        Log.notice(F("pid_input = %d" CR), pid_input);
        Log.notice(F("abs(pid_setpoint - pid_input) %d" CR), abs(pid_setpoint - pid_input));
#endif
        if (gap < gap_setpoint)
        { // we're close to setpoint, use conservative tuning parameters
                myPID.SetTunings(consKp, consKi, consKd);
        }
        else
        {
                // we're far from setpoint, use aggressive tuning parameters
                myPID.SetTunings(aggKp, aggKi, aggKd);
        }
        myPID.Compute(); // Let the PID compute the value, returns the calculated optimal output
}

void moteur_stop(int i)
{
#ifdef AUTOPID
        myPID.stop();
#endif
#ifdef PID
        //   myPID.stop();
        myPID.SetMode(myPID.Control::manual);
#endif
        motor[i].stop();                   // stop moteur
        delay(50);                         // attente 50ms
        lecture_pot(i);                    // lecture analogique du potentiomètre
        save_pot(i);                       // sauvegarde position potentiomètre
        motor_change[i] = false;           // remise à zéro du flag de changement de position du potentiomètre
        position_set[i] = position_lue[i]; // remise à zéro de la consigne de position du potentiomètre
        last_change_time = millis();       // sauvegarde du temps du dernier changement de position
#ifdef DEBUG_CONST                         // si DEBUG activé
        Log.notice(F("========================== void moteur_stop(%d) ===================================" CR), i);
        Log.notice(F("motor_change[%d] = %d" CR), i, motor_change[i]);
        Log.notice(F("position_lue[%d] = %d" CR), i, position_lue[i]);
        Log.notice(F("position_set[%d] = %d" CR), i, position_set[i]);
        Log.notice(F("position_save[%d] = %d" CR), i, position_save[i]);
        Log.notice(F("state_pot_change[%d] = %d" CR), i, state_pot_change[i]);
        count_motor = true; // remise à zéro du compteur de boucle
#endif
}

void moteur_set(int i) // fonction de gestion des moteurs
{
#ifdef PID
        myPID.SetMode(myPID.Control::automatic);
        pid_set(i);
        int motor_speed_value = pid_output; // Let the PID compute the value, returns the calculated optimal output
#endif

#ifdef AUTOPID
        motor_read = position_lue[i]; // The "goal" the PID controller tries to "reach",
        motor_set = position_set[i];  // The "goal" the PID controller tries to "reach",
        myPID.run();                  // Let the PID compute the value, returns the calculated optimal output
        int motor_speed_value = (int)out_speed_value;
        int map_speed_value = map(motor_speed_value, 100, 1000, -63, 63);
        int const_speed_value = constrain(motor_speed_value, -63, 63);
#endif

#ifdef DEBUG_CONST // si DEBUG activé
        if (count_motor == true)
        {
                Log.notice(F("========================= void moteur_set(%d)" CR), i);
                Log.notice(F("position_lue[%d] = %d" CR), i, position_lue[i]);
                Log.notice(F("position_set[%d] = %d" CR), i, position_set[i]);
                Log.notice(F("motor_change[%d] = %d" CR), i, motor_change[i]);
                Log.notice(F("position_save[%d] = %d" CR), i, position_save[i]); // position sauvegardée
                Log.notice(F("motor_speed_value = %d" CR), motor_speed_value);   // valeur de vitesse moteur
#ifdef AUTOPID
                Log.notice(F("out_speed_value = %d" CR), out_speed_value);
                Log.notice(F("(int) out_speed_value = %d" CR), (int)out_speed_value);
                Log.notice(F("map_speed_value = %d" CR), map_speed_value);
                Log.notice(F("const_speed_value = %d" CR), const_speed_value);
#endif
        }
#endif
#ifdef DEBUG_MOTEUR_HARD // si DEBUG activé
        Log.notice(F("========================= void moteur_set(%d)" CR), i);
        Log.notice(F("position_lue[%d] = %d" CR), i, position_lue[i]);
        Log.notice(F("position_set[%d] = %d" CR), i, position_set[i]);
        Log.notice(F("motor_change[%d] = %d" CR), i, motor_change[i]);
        Log.notice(F("position_save[%d] = %d" CR), i, position_save[i]);
        Log.notice(F("motor_speed_value = %d" CR), motor_speed_value);
#ifdef AUTOPID
        Log.notice(F("out_speed_value = %d" CR), out_speed_value);
        Log.notice(F("(int) out_speed_value = %d" CR), (int)out_speed_value);
        Log.notice(F("map_speed_value = %d" CR), map_speed_value);
        Log.notice(F("const_speed_value = %d" CR), const_speed_value);
#endif
#endif

        if (position_lue[i] == position_set[i] || position_set[i] > 1023 || position_set[i] < 0) // si position lue égale à la consigne ou consigne hors bornes
        {
#ifdef DEBUG_MOTEUR // si DEBUG activé
                Log.notice(F("MOTEUR[%d] OK" CR), i);
#endif
#ifdef DEBUG_CONST // si DEBUG activé
                count_motor = true;
#endif
                moteur_stop(i);
                return;
        }
        else if (position_lue[i] < (position_set[i] + ECRART_V_STOP) && position_lue[i] > (position_set[i] - ECRART_V_STOP)) // si moteur entre consigne et ECRART_V_STOP
        {
#ifdef DEBUG_MOTEUR // si DEBUG activé
                Log.notice(F("MOTEUR[%d] OK" CR), i);
#endif
#ifdef DEBUG_CONST // si DEBUG activé
                count_motor = true;
#endif
                moteur_stop(i);
                return;
        }
        // *******************************************************************************************************
        // si moteur doit aller vers la gauche
        else if (position_lue[i] > (position_set[i] + (ECRART_V_STOP / 2))) // si moteur n'est pas entre consigne et ECRART_V_STOP
        {
#ifdef DEBUG_CONST // si DEBUG activé
                if (count_motor == true)
                {
                        Log.notice(F("========================== MOTEUR[%d] GO RIGHT at speed = %d ============================" CR), i, motor_speed_value);
                        count_motor = false;
                }
#endif
                motor_change[i] = true;
                motor[i].drive(-motor_speed_value);
        }
        else if (position_lue[i] < (position_set[i] - (ECRART_V_STOP / 2))) // si moteur n'est pas entre consigne et ECRART_V_STOP
        {
#ifdef DEBUG_CONST // si DEBUG activé
                if (count_motor == true)
                {
                        Log.notice(F("========================== MOTEUR[%d] GO LEFT at speed = %d ============================" CR), i, motor_speed_value);
                        count_motor = false;
                }
#endif
                motor_change[i] = true;
                motor[i].drive(motor_speed_value);
        }
        else
        {
#ifdef DEBUG_MOTEUR // si DEBUG activé
                Log.notice(F("MOTEUR[%d] NOK" CR), i);
#endif
#ifdef DEBUG_CONST // si DEBUG activé
                count_motor = true;
#endif
                motor[i].brake(); // stop moteur
                moteur_stop(i);
                return;
        }
}
