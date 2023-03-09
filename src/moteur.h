// *******************************************************************************************************
// ****************************************** gestion moteur *********************************************
// *******************************************************************************************************
#include <SparkFunMiniMoto.h> // Include the MiniMoto library
/*
Attempts to set speed lower than 6 will be ignored; speeds higher than 63 will be truncated to 63. The sign of the value determines the direction of the motion.
*/
#include <FastPID.h>

float Kp = 0.1, Ki = 0.5, Kd = 0, Hz = 2;
int output_bits = 8;
bool output_signed = true;
FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

// Create  MiniMoto instances, pour controle des moteur en I2C (DRV8830)
MiniMoto motor[4] = {gain_0_motor, vol_0_motor, gain_1_motor, vol_1_motor};

/*In this section we have defined the gain values for the
   proportional,integral, and derivative controller i have set
   the gain values with the help of trial and error methods.
*/
#define ECART_V_STOP 7       // hystérésie de positionnement potentiomètre
#define ECART_V_SPEED_PID 20 // hystérésie de positionnement avec ou sans PID

#define speed_min 10 // vitesse min moteur

void moteur_set(int i);
void moteur_stop(int i);

void moteur_stop(int i)
{
        myPID.clear();                     // remise à zéro du PID
        lecture_pot(i);                    // lecture analogique du potentiomètre
        motor_change[i] = false;           // remise à zéro du flag de changement de position du potentiomètre
        position_set[i] = position_lue[i]; // remise à zéro de la consigne de position du potentiomètre
        save_pot(i);                       // sauvegarde position potentiomètre
        motor[i].stop();                   // stop moteur
}

void moteur_set(int i) // fonction de gestion des moteurs
{

        if (position_lue[i] <= (position_set[i] + ECART_V_STOP) && position_lue[i] >= (position_set[i] - ECART_V_STOP)) // si moteur entre consigne et ECART_V_STOP
        {
                motor[i].brake();    // frein moteur
                error_state = false; // remise à zéro du flag d'erreur
                moteur_stop(i);      // stop moteur
                return;
        }
        // *******************************************************************************************************
        else if ((position_lue[i] - ECART_V_STOP) > position_set[i]) // si moteur doit aller vers la gauche
        {
                myPID.configure(Kp, Ki, Kd, Hz, output_bits, output_signed);
                myPID.setOutputRange(-63, -6);
                error_state = false;                                          // remise à zéro du flag d'erreur
                motor_change[i] = true;                                       // flag de changement de position du potentiomètre
                if (position_lue[i] >= (position_set[i] + ECART_V_SPEED_PID)) // vitesse PID si moteur n'est pas entre consigne et ECART_V_SPEED_PID
                {
                        motor_speed[i] = myPID.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                }
                else if (position_lue[i] > (position_set[i]))
                {
                        myPID.clear();               // remise à zéro du PID
                        motor[i].brake();            // frein moteur pour inertie moteur
                        motor_speed[i] = -speed_min; // vitesse min moteur
                }
                motor[i].drive(motor_speed[i]); // drive moteur
        }
        // *******************************************************************************************************
        else if ((position_lue[i] + ECART_V_STOP) < position_set[i]) // si moteur doit aller vers la droite
        {
                myPID.configure(Kp, Ki, Kd, Hz, output_bits, output_signed);
                myPID.setOutputRange(6, 63);
                error_state = false;                                          // remise à zéro du flag d'erreur
                motor_change[i] = true;                                       // flag de changement de position du potentiomètre
                if (position_lue[i] <= (position_set[i] - ECART_V_SPEED_PID)) // viteur PID si moteur n'est pas entre consigne et ECART_V_SPEED_PID
                {
                        motor_speed[i] = myPID.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                }
                else if (position_lue[i] < (position_set[i]))
                {
                        myPID.clear();              // remise à zéro du PID
                        motor[i].brake();           // frein moteur pour inertie moteur
                        motor_speed[i] = speed_min; // vitesse min moteur
                }
                motor[i].drive(motor_speed[i]); // drive moteur
        }
        else
        {
#ifdef DEBUG
                breakpoint();
#endif
                error_state = true; // flag d'erreur
                motor[i].brake();   // frein moteur
                moteur_stop(i);
                return;
        }
}