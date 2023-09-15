/*******************************************************************************************************
gestion de la vitesse et du sens de déplacement des moteur
*******************************************************************************************************/

#include <SparkFunMiniMoto.h> // Include the MiniMoto library
/*
Attempts to set speed lower than 6 will be ignored; speeds higher than 63 will be truncated to 63. The sign of the value determines the direction of the motion.
*/

MiniMoto motor[4] = {gain_0_motor, vol_0_motor, gain_1_motor, vol_1_motor}; // Create  MiniMoto instances, pour controle des moteur en I2C (DRV8830)
#define ECART_V_STOP 3                                                      // hystérésie de positionnement potentiomètre
#define ECART_V_SPEED_PID 50                                                // valeur d'écart pour vitesse moteur avec ou sans PID
#define ECART_V_MIN_MAX 10                                                  // valeur d'écart au min et au max  pour vitesse moteur

#define speed_min 10     // vitesse moteur sans PID
#define PID_speed_min 15 // vitesse min moteur avec PID
#define PID_speed_max 63 // vitesse max moteur avec PID

#include <FastPID.h>
float Kp = 0.1, Ki = 0.5, Kd = 0, Hz = 2;
int output_bits = 8;
bool output_signed = true;
FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

void moteur_set(int i);  // fonction de gestion de vitesse et position moteurs
void moteur_stop(int i); // fonction d'arret moteurs

void moteur_stop(int i)
{
        myPID.clear();                     // remise à zéro du PID
        motor_speed[i] = 0;                // remise à zéro de la vitesse du moteur
        motor[i].stop();                   // stop moteur
        //delay(100);                        // attente 100ms
        motor_change[i] = false;           // remise à zéro du flag de changement de position du potentiomètre
        lecture_pot(i);                    // lecture analogique du potentiomètre
        position_set[i] = position_lue[i]; // remise à zéro de la consigne de position du potentiomètre
        save_pot(i);                       // sauvegarde position potentiomètre
        last_motor_end[i] = millis();
#ifdef DEBUG // si DEBUG activé
        breakpoint();
#endif
}

void moteur_set(int i)
{
        if (position_lue[i] <= (position_set[i] + ECART_V_STOP) && position_lue[i] >= (position_set[i] - ECART_V_STOP)) // si moteur entre consigne et ECART_V_STOP
        {
                motor[i].brake();    // frein moteur
                error_state = false; // remise à zéro du flag d'erreur
                moteur_stop(i);      // stop moteur
                return;
        }
        // *******************************************************************************************************
        else if (position_lue[i] > position_set[i]) // si moteur doit aller vers la gauche
        {
                myPID.configure(Kp, Ki, Kd, Hz, output_bits, output_signed); // configuration PID
                myPID.setOutputRange(-PID_speed_max, -PID_speed_min);        // plage de sortie du PID
                error_state = false;                                         // remise à zéro du flag d'erreur
                if (position_lue[i] >= position_set[i] + ECART_V_SPEED_PID)  // vitesse PID si moteur n'est pas entre consigne et ECART_V_SPEED_PID
                {
                        motor_speed[i] = myPID.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                }
                else if (position_lue[i] < position_set[i] + ECART_V_SPEED_PID && position_lue[i] > position_set[i] + ECART_V_STOP)
                {
                        // motor[i].brake();            // frein moteur pour inertie moteur
                        motor_speed[i] = -speed_min; // vitesse min moteur
                        myPID.clear();               // remise à zéro du PID
                }
                else
                {
                        motor[i].brake(); // frein moteur pour inertie moteur
                        moteur_stop(i);   // stop moteur
                }
                if (position_set[i] <= min_pot[i] + ECART_V_MIN_MAX) // si consigne de position du potentiomètre = 0
                {
                        position_set[i] = min_pot[i];

                        motor_speed[i] = -63;
                }
                motor[i].drive(motor_speed[i]); // controle moteur
        }
        // *******************************************************************************************************
        else if (position_lue[i] < position_set[i]) // si moteur doit aller vers la droite
        {
                myPID.configure(Kp, Ki, Kd, Hz, output_bits, output_signed); // configuration PID
                myPID.setOutputRange(PID_speed_min, PID_speed_max);          // plage de sortie du PID
                error_state = false;                                         // remise à zéro du flag d'erreur
                if (position_lue[i] <= position_set[i] - ECART_V_SPEED_PID)  // vitesse PID si moteur n'est pas entre consigne et ECART_V_SPEED_PID
                {
                        motor_speed[i] = myPID.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                }
                else if (position_lue[i] > position_set[i] - ECART_V_SPEED_PID && position_lue[i] < position_set[i] - ECART_V_STOP)
                {
                        // motor[i].brake();           // frein moteur pour inertie moteur
                        motor_speed[i] = speed_min; // vitesse min moteur
                        myPID.clear();              // remise à zéro du PID
                }
                else
                {
                        motor[i].brake(); // frein moteur pour inertie moteur
                        moteur_stop(i);   // stop moteur
                }
                if (position_set[i] >= max_pot[i] - ECART_V_MIN_MAX) // si consigne de position du potentiomètre = 0
                {
                        position_set[i] = max_pot[i];
                        motor_speed[i] = 63;
                }
                motor[i].drive(motor_speed[i]); // controle moteur
        }
        else
        {
                error_state = true; // flag d'erreur
                motor[i].brake();   // frein moteur
                moteur_stop(i);
                return;
        }
}