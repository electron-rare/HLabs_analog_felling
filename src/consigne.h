
void consigne_set(int i);                      // calcul des consignes
void motor_calc(int pot, int motor);           // calcul des position de moteurs
void position_calc(int pot, int motor);        // calcul de la consigne de position
void position_stereo_calc(int pot, int motor); // calcul de la consigne de position en stereo

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
    if (position_lue[motor] <= (position_set[motor] + ECART_V_STOP) && position_lue[motor] >= (position_set[motor] - ECART_V_STOP)) // si moteur entre consigne et ECART_V_STOP
    {
        motor_change[motor] = false; // moteur entre consigne et ECART_V_STOP
    }
    else
    {
        motor_change[motor] = true; // il y a changement de position du potentiomètre
    }
}

void position_calc(int pot, int motor)
{
    if (position_lue[pot] < position_memory[pot] - ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] + (position_memory[pot] - position_lue[pot]);
        motor_calc(pot, motor); // pot, motor
    }
    else if (position_lue[pot] > position_memory[pot] + ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] - (position_lue[pot] - position_memory[pot]);
        motor_calc(pot, motor); // pot, motor
    }
    else
    {
        return;
    }
}

void position_stereo_calc(int pot, int motor)
{
    if (position_lue[pot] < position_memory[pot] - ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] - (position_memory[pot] - position_lue[pot]);
        motor_calc(pot, motor); // pot, motor
    }
    else if (position_lue[pot] > position_memory[pot] + ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] + (position_lue[pot] - position_memory[pot]);

        motor_calc(pot, motor); // pot, motor
    }
    else
    {
        return;
    }
}
void consigne_set(int i) // calcul des consignes de moteurs
{
    if (stereo_link_state == true) // si bouton const_out_L appuyé
    {
        switch (i)
        {
        case 0:
            position_set[0] = position_lue[0];
            position_stereo_calc(0, 2); // pot, motor
            break;
        case 1:
            position_set[1] = position_lue[1];
            position_stereo_calc(1, 3); // pot, motor
            break;
        case 2:
            position_set[2] = position_lue[2];
            position_stereo_calc(2, 0); // pot, motor
            break;
        case 3:
            position_set[3] = position_lue[3];
            position_stereo_calc(3, 1); // pot, motor
            break;
        }
    }

    if (const_out_L_state == true) // si bouton const_out_L appuyé
    {
        const int gain_const = 0;
        const int volume_const = 1;
        const int gain_norm = 2;
        const int volume_norm = 3;
        switch (i)
        {
        case gain_norm:
            position_set[gain_norm] = position_lue[gain_norm];
            break;
        case volume_norm:
            position_set[volume_norm] = position_lue[volume_norm];
            break;
        case gain_const:
            position_set[gain_const] = position_lue[gain_const];
            position_calc(gain_const, volume_const);
            break;
        case volume_const:
            position_set[volume_const] = position_lue[volume_const];
            position_calc(volume_const, gain_const);
            break;
        default:
            break;
        }
    }

    if (const_out_R_state == true) // si bouton const_out_L appuyé
    {
        const int gain_const = 2;
        const int volume_const = 3;
        const int gain_norm = 0;
        const int volume_norm = 1;
        switch (i)
        {
        case gain_norm:
            position_set[gain_norm] = position_lue[gain_norm];
            break;
        case volume_norm:
            position_set[volume_norm] = position_lue[volume_norm];
            break;
        case gain_const:
            position_set[gain_const] = position_lue[gain_const];
            position_calc(gain_const, volume_const);
            break;
        case volume_const:
            position_set[volume_const] = position_lue[volume_const];
            position_calc(volume_const, gain_const);
            break;
        default:
            break;
        }
    }
}