
void consigne_set(int i);            // calcul des consignes
void motor_calc(int pot, int motor); // calcul des position de moteurs

void motor_calc(int pot, int motor) // calcul des consignes de moteurs et du flag de changement de position
{
    lecture_pot(pot);                      // lecture des potentiomètres
    lecture_pot(motor);                    // lecture des potentiomètres
    position_set[pot] = position_lue[pot]; // set position potentiomètre gain et volume gauche et droit
    motor_change[pot] = false;
    if (position_set[motor] <= min_pot)
    {
        position_set[motor] = 0;
    }
    else if (position_set[motor] >= max_pot)
    {
        position_set[motor] = 1023;
    }
    if (position_set[motor] >= (position_lue[motor] + ECART_V_STOP) || position_set[motor] <= (position_lue[motor] - ECART_V_STOP)) // si consigne n'est pas atteinte
    {
        motor_change[motor] = true; // il y a changement de position du potentiomètre
    }
}

void consigne_set(int i) // calcul des consignes de moteurs
{
    if (const_out_L_state == true) // si bouton const_out_L appuyé
    {
        int gain = 0;
        int volume = 1;
        switch (i)
        {
        case 0:
            position_set[gain] = position_lue[gain];
            if (position_lue[gain] < position_memory[gain] - ECART_V_STOP)
            {
                position_set[volume] = position_memory[volume] + (position_memory[gain] - position_lue[gain]);
                motor_calc(gain, volume); // pot, motor
            }
            else if (position_lue[gain] > position_memory[gain] + ECART_V_STOP)
            {
                position_set[volume] = position_memory[volume] - (position_lue[gain] - position_memory[gain]);
                motor_calc(gain, volume); // pot, motor
            }
            else
            {
                return;
            }
            break;
        case 1:
            position_set[volume] = position_lue[volume];
            if (position_lue[volume] < position_memory[volume] - ECART_V_STOP)
            {
                position_set[gain] = position_memory[gain] + (position_memory[volume] - position_lue[volume]);
                motor_calc(volume, gain); // pot, motor
            }
            else if (position_lue[volume] > position_memory[volume] + ECART_V_STOP)
            {
                position_set[gain] = position_memory[gain] - (position_lue[volume] - position_memory[volume]);
                motor_calc(volume, gain); // pot, motor
            }
            else
            {
                return;
            }
            break;
        default:
            break;
        }
        /*
            if (const_out_R_state == true) // si bouton const_out_R appuyé
            {
            }
            if (stereo_link_state == true) // si bouton stereo_link appuyé
            {
                if (const_out_R_state == true || const_out_L_state == true) // si bouton const_out_R ou const_out_L appuyé
                {
                }
                else if (const_out_R_state == true && const_out_L_state == true) // si uniquement stereo_link est appuyé
                {
                }
                else if (const_out_R_state == false && const_out_L_state == false) // si aucun bouton n'est appuyé
                {
                }
            }
        */
    }
}