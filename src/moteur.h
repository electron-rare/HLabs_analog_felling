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
#define ECART_V_STOP 5 // hystérésie de positionnement potentiomètre
#define speed_max 63   // vitesse max moteur
#define speed_med 35   // vitesse moyenne moteur
#define speed_min 10   // vitesse min moteur

void moteur_set(int i);
void moteur_stop(int i);

void moteur_stop(int i)
{
        motor[i].stop();                   // stop moteur
        lecture_pot(i);                    // lecture analogique du potentiomètre
        motor_change[i] = false;           // remise à zéro du flag de changement de position du potentiomètre
        position_set[i] = position_lue[i]; // remise à zéro de la consigne de position du potentiomètre
}

void moteur_set(int i) // fonction de gestion des moteurs
{
        if (position_lue[i] == position_set[i] || position_set[i] > max_pot || position_set[i] < min_pot) // si position lue égale à la consigne ou consigne hors bornes
        {
                moteur_stop(i);
                return;
        }
        else if (position_lue[i] < (position_set[i] + ECART_V_STOP) && position_lue[i] > (position_set[i] - ECART_V_STOP)) // si moteur entre consigne et ECART_V_STOP
        {
                moteur_stop(i);
                return;
        }
        // *******************************************************************************************************
        else if (position_lue[i] > (position_set[i] + ECART_V_STOP)) // si moteur doit aller vers la gauche
        {
                motor_change[i] = true;
                if (position_lue[i] > (position_set[i] + (ECART_V_STOP * 4))) // vitesse max si moteur n'est pas entre consigne et ECART_V_STOP * 4
                {
                        motor[i].drive(-speed_max);
                }
                else if (position_lue[i] > (position_set[i] + (ECART_V_STOP * 3))) // vitesse moyenne si moteur n'est pas entre consigne et ECART_V_STOP * 3
                {
                        motor[i].drive(-speed_med);
                }
                else if (position_lue[i] > (position_set[i] + (ECART_V_STOP * 2))) // vitesse min si moteur n'est pas entre consigne et ECART_V_STOP * 2
                {
                        motor[i].drive(-speed_min);
                }
        }
        // *******************************************************************************************************
        if (position_lue[i] < (position_set[i] - ECART_V_STOP)) // si moteur doit aller vers la droite
        {
                motor_change[i] = true;
                if (position_lue[i] < (position_set[i] - (ECART_V_STOP * 4))) // vitesse max si moteur n'est pas entre consigne et ECART_V_STOP * 4
                {
                        motor[i].drive(speed_max);
                }
                else if (position_lue[i] < (position_set[i] - (ECART_V_STOP * 3))) // viteur moyenne si moteur n'est pas entre consigne et ECART_V_STOP * 3
                {
                        motor[i].drive(speed_med);
                }
                else if (position_lue[i] < (position_set[i] - (ECART_V_STOP * 2))) // viteur min si moteur n'est pas entre consigne et ECART_V_STOP * 2
                {
                        motor[i].drive(speed_min);
                }
        }
        else
        {
                motor[i].brake(); // stop moteur
                moteur_stop(i);
                return;
        }
}