/*******************************************************************************************************
gestion de la vitesse et du sens de déplacement des moteur
*******************************************************************************************************/

#include <SparkFunMiniMoto.h> // Include the MiniMoto library
/*
Attempts to set speed lower than 6 will be ignored; speeds higher than 63 will be truncated to 63. The sign of the value determines the direction of the motion.
*/

MiniMoto motor[4] = {gain_0_motor, vol_0_motor, gain_1_motor, vol_1_motor}; // Create  MiniMoto instances, pour controle des moteur en I2C (DRV8830)
#define ECART_V_STOP 2                                                      // hystérésie de positionnement potentiomètre
#define ECART_V_SPEED_PID 50                                                // valeur d'écart pour vitesse moteur avec ou sans PID
#define ECART_V_MIN_MAX 5                                                   // valeur d'écart au min et au max  pour vitesse moteur

#define speed_min 10     // vitesse moteur sans PID
#define PID_speed_min 25 // vitesse min moteur avec PID
#define PID_speed_max 63 // vitesse max moteur avec PID

#include <FastPID.h>
#if bit_read_pot == 4                         // configuration PID en fonction de la précision de lecture du potentiomètre
float Kp = 0.8, Ki = 0.1, Kd = 0.2, Hz = 250; // a regler
#elif bit_read_pot == 2
float Kp = 0.6, Ki = 0.1, Kd = 0.2, Hz = 250; // a tester
#elif bit_read_pot == 1
float Kp = 0.6, Ki = 0.1, Kd = 0.2, Hz = 250; // OK
#endif

int output_bits = 8;
bool output_signed = true;
FastPID myPID_0(Kp, Ki, Kd, Hz, output_bits, output_signed);
FastPID myPID_1(Kp, Ki, Kd, Hz, output_bits, output_signed);
FastPID myPID_2(Kp, Ki, Kd, Hz, output_bits, output_signed);
FastPID myPID_3(Kp, Ki, Kd, Hz, output_bits, output_signed);

void moteur_set(int i);  // fonction de gestion de vitesse et position moteurs avec ou sans PID
void moteur_stop(int i); // fonction d'arret moteurs

void moteur_stop(int i) // fonction d'arret moteurs
{
        switch (i)
        {
        case 0:
                myPID_0.clear(); // remise à zéro du PID
                break;
        case 1:
                myPID_1.clear(); // remise à zéro du PID

                break;
        case 2:
                myPID_2.clear(); // remise à zéro du PID

                break;
        case 3:
                myPID_3.clear(); // remise à zéro du PID
                break;
        }
        motor_speed[i] = 0;                // remise à zéro de la vitesse du moteur
        motor[i].stop();                   // stop moteur
        motor_change[i] = false;           // remise à zéro du flag de changement de position du potentiomètre
        lecture_pot(i);                    // lecture analogique du potentiomètre
        position_set[i] = position_lue[i]; // remise à zéro de la consigne de position du potentiomètre
        save_pot(i);                       // sauvegarde position potentiomètre
        last_motor_end = millis();         // sauvegarde du temps d'arrêt du moteur
}

void moteur_set(int i) // fonction de gestion de vitesse et position moteurs avec ou sans PID
{

        if (position_lue[i] <= (position_set[i] + ECART_V_STOP) && position_lue[i] >= (position_set[i] - ECART_V_STOP)) // si moteur entre consigne et ECART_V_STOP
        {
                motor[i].brake(); // frein moteur
                motor_speed[i] = 0;
                error_state = false; // remise à zéro du flag d'erreur
                moteur_stop(i);      // stop moteur
                return;
        }
        else if (position_set[i] <= (min_val_pot + ECART_V_MIN_MAX) && position_lue[i] <= (position_set[i] + ECART_V_MIN_MAX)) // si moteur est a la position min
        {
                motor[i].brake(); // frein moteur
                motor_speed[i] = 0;
                error_state = false; // remise à zéro du flag d'erreur
                moteur_stop(i);      // stop moteur
                return;
        }
        else if (position_set[i] >= (max_val_pot) && position_lue[i] >= (position_set[i])) // si moteur est a la position max
        {
                motor[i].brake(); // frein moteur
                motor_speed[i] = 0;
                error_state = false; // remise à zéro du flag d'erreur
                moteur_stop(i);      // stop moteur
                return;
        }
        // *******************************************************************************************************
        else if (position_lue[i] > position_set[i]) // si moteur doit aller vers la gauche
        {
                // myPID.configure(Kp, Ki, Kd, Hz, output_bits, output_signed); // configuration PID
                switch (i) // configuration PID en fonction du moteur (i)
                {
                case 0:
                        myPID_0.setOutputRange(-PID_speed_max, -PID_speed_min); // plage de sortie du PID
                        break;
                case 1:
                        myPID_1.setOutputRange(-PID_speed_max, -PID_speed_min); // plage de sortie du PID

                        break;
                case 2:
                        myPID_2.setOutputRange(-PID_speed_max, -PID_speed_min); // plage de sortie du PID

                        break;
                case 3:
                        myPID_3.setOutputRange(-PID_speed_max, -PID_speed_min); // plage de sortie du PID
                        break;
                }
                error_state = false;                                  // remise à zéro du flag d'erreur
                if (position_set[i] <= min_val_pot + ECART_V_MIN_MAX) // si consigne de position du potentiomètre = min
                {
                        motor_speed[i] = -63;
                        switch (i) // mise à zéro du PID en fonction du moteur (i)
                        {
                        case 0:
                                myPID_0.clear(); // remise à zéro du PID
                                break;
                        case 1:
                                myPID_1.clear(); // remise à zéro du PID
                                break;
                        case 2:
                                myPID_2.clear(); // remise à zéro du PID

                                break;
                        case 3:
                                myPID_3.clear(); // remise à zéro du PID
                                break;
                        }
                }
                else if (position_lue[i] >= position_set[i] + ECART_V_STOP) // vitesse PID si moteur n'est pas entre consigne et ECART_V_STOP
                {
                        switch (i) // calcul de la vitesse moteur en fonction du moteur (i)
                        {
                        case 0:
                                motor_speed[i] = myPID_0.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        case 1:
                                motor_speed[i] = myPID_1.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        case 2:
                                motor_speed[i] = myPID_2.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        case 3:
                                motor_speed[i] = myPID_3.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        }
                }
                else
                {
                        motor_speed[i] = 0;
                        motor[i].brake(); // frein moteur pour inertie moteur
                        moteur_stop(i);   // stop moteur
                        return;
                }
                motor[i].drive(motor_speed[i]); // controle moteur
        }
        // *******************************************************************************************************
        else if (position_lue[i] < position_set[i]) // si moteur doit aller vers la droite
        {
                // myPID.configure(Kp, Ki, Kd, Hz, output_bits, output_signed); // configuration PID
                switch (i)
                {
                case 0:
                        myPID_0.setOutputRange(PID_speed_min, PID_speed_max); // plage de sortie du PID
                        break;
                case 1:
                        myPID_1.setOutputRange(PID_speed_min, PID_speed_max); // plage de sortie du PID

                        break;
                case 2:
                        myPID_2.setOutputRange(PID_speed_min, PID_speed_max); // plage de sortie du PID

                        break;
                case 3:
                        myPID_3.setOutputRange(PID_speed_min, PID_speed_max); // plage de sortie du PID
                        break;
                }
                error_state = false; // remise à zéro du flag d'erreur

                if (position_set[i] >= max_val_pot - ECART_V_MIN_MAX) // si consigne de position du potentiomètre = max
                {
                        motor_speed[i] = 63;
                        switch (i)
                        {
                        case 0:
                                myPID_0.clear(); // remise à zéro du PID
                                break;
                        case 1:
                                myPID_1.clear(); // remise à zéro du PID

                                break;
                        case 2:
                                myPID_2.clear(); // remise à zéro du PID

                                break;
                        case 3:
                                myPID_3.clear(); // remise à zéro du PID
                                break;
                        }
                }
                else if (position_lue[i] <= position_set[i] - ECART_V_STOP) // vitesse PID si moteur n'est pas entre consigne et ECART_V_STOP
                {
                        switch (i)
                        {
                        case 0:
                                motor_speed[i] = myPID_0.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        case 1:
                                motor_speed[i] = myPID_1.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        case 2:
                                motor_speed[i] = myPID_2.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        case 3:
                                motor_speed[i] = myPID_3.step(position_set[i], position_lue[i]); // vitesse moteur par PID
                                break;
                        }
                }
                else
                {
                        motor_speed[i] = 0;
                        motor[i].brake(); // frein moteur pour inertie moteur
                        moteur_stop(i);   // stop moteur
                        return;
                }
                motor[i].drive(motor_speed[i]); // controle moteur
        }
        else
        {
                error_state = true; // flag d'erreur
                motor_speed[i] = 0;
                motor[i].brake(); // frein moteur
                moteur_stop(i);
                return;
        }
}