/*******************************************************************************************************
gestion des consigne et des calcul de position des moteurs
*******************************************************************************************************/

void consigne_set(int i);                      // calcul des consignes
void motor_calc(int pot, int motor);           // calcul des position de moteurs
void position_const_calc(int pot, int motor);  // calcul de la consigne de position
void position_stereo_calc(int pot, int motor); // calcul de la consigne de position en stereo

void motor_calc(int pot, int motor) // calcul des consignes de moteurs et du flag de changement de position
{
    lecture_pot(pot);   // lecture des potentiomètres
    lecture_pot(motor); // lecture des potentiomètres
    if (position_set[motor] <= 0)
    {
        position_set[motor] = min_pot[motor];
    }
    else if (position_set[motor] >= 1023)
    {
        position_set[motor] = max_pot[motor];
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

void position_const_calc(int pot, int motor)
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

void position_stereo_const_calc(int pot, int motor)
{
    if (position_set[pot] < position_memory[pot] - ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] + (position_memory[pot] - position_set[pot]);
        motor_calc(pot, motor); // pot, motor
    }
    else if (position_set[pot] > position_memory[pot] + ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] - (position_set[pot] - position_memory[pot]);
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
    if (stereo_link_state == true) // si stereo_link actif
    {
        switch (i)
        {
        case 0:
            position_stereo_calc(0, 2); // pot, motor
            break;
        case 1:
            position_stereo_calc(1, 3); // pot, motor
            break;
        case 2:
            position_stereo_calc(2, 0); // pot, motor
            break;
        case 3:
            position_stereo_calc(3, 1); // pot, motor
            break;
        }
    }

    if (const_out_L_state == true) // si bouton const_out_L ou R appuyé
    {
        const int gain_const = 0;
        const int volume_const = 1;
        const int gain_norm = 2;
        const int volume_norm = 3;
        switch (i)
        {
        case gain_const:
            position_const_calc(gain_const, volume_const);
            if (stereo_link_state == true)
            {
                position_stereo_const_calc(gain_norm, volume_norm);
            }
            break;
        case volume_const:
            position_const_calc(volume_const, gain_const);
            if (stereo_link_state == true)
            {
                position_stereo_const_calc(volume_norm, gain_norm);
            }
            break;
        default:
            break;
        }
        if (stereo_link_state != true)
            position_set[i] = position_lue[i];
#ifdef DEBUG // si DEBUG activé
        breakpoint();
#endif
    }

    if (const_out_R_state == true) // si const_out_L actif
    {
        const int gain_const = 2;
        const int volume_const = 3;
        const int gain_norm = 0;
        const int volume_norm = 1;
        switch (i)
        {
        case gain_const:
            position_const_calc(gain_const, volume_const);
            if (stereo_link_state == true)
            {
                position_stereo_const_calc(gain_norm, volume_norm);
            }
            break;
        case volume_const:
            position_const_calc(volume_const, gain_const);
            if (stereo_link_state == true)
            {
                position_stereo_const_calc(volume_norm, gain_norm);
            }
            break;
        default:
            break;
        }
        if (stereo_link_state != true)
            position_set[i] = position_lue[i];
#ifdef DEBUG // si DEBUG activé
        breakpoint();
#endif
    }
}