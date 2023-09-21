/*******************************************************************************************************
gestion des consigne et des calcul de position des moteurs
*******************************************************************************************************/

void motor_calc(int pot, int motor);                 // calcul des position de moteurs
void position_stereo_calc(int pot, int motor);       // calcul de la consigne de position en stereo
void position_stereo_const_calc(int pot, int motor); // calcul de la consigne de position en stereo et constant output
void position_const_calc(int pot, int motor);        // calcul de la consigne de position en constant output
void consigne_set(int i);                            // calcul des consignes

void motor_calc(int pot, int motor) // calcul des consignes de moteurs et du flag de changement de position
{
    lecture_pot(motor); // lecture des potentiomètres
    // calcul de la consigne de position des moteurs en min et max
    if (position_set[motor] < min_val_pot)
    {
        position_set[motor] = min_val_pot;
    }
    else if (position_set[motor] > max_val_pot)
    {
        position_set[motor] = max_val_pot;
    }
    // calcul du flag de changement de position du potentiomètre
    if (position_lue[motor] <= (position_set[motor] + ECART_V_STOP) && position_lue[motor] >= (position_set[motor] - ECART_V_STOP)) // si moteur entre consigne et ECART_V_STOP
    {
        motor_change[motor] = false; // moteur entre consigne et ECART_V_STOP
    }
    else
    {
        motor_change[motor] = true; // il y a changement de position du potentiomètre
    }
}

void position_stereo_calc(int pot, int motor)
{
    if (position_lue[pot] < position_memory[pot] - ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] - (position_memory[pot] - position_lue[pot]);
        motor_calc(pot, motor);
    }
    else if (position_lue[pot] > position_memory[pot] + ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] + (position_lue[pot] - position_memory[pot]);
        motor_calc(pot, motor);
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
        motor_calc(pot, motor);
    }
    else if (position_set[pot] > position_memory[pot] + ECART_V_STOP)
    {
        position_set[motor] = position_memory[motor] - (position_set[pot] - position_memory[pot]);
        motor_calc(pot, motor);
    }
    else
    {
        return;
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

void consigne_set(int i) // calcul des consignes de moteurs
{
    if (stereo_link_state == true) // si stereo_link actif
    {
        switch (i)
        {
        case 0:
            position_stereo_calc(0, 2);
            if (const_out_L_state == true || const_out_R_state == true)
            {
                position_const_calc(0, 1);
                position_stereo_const_calc(2, 3);
            }
            break;
        case 1:
            position_stereo_calc(1, 3);
            if (const_out_L_state == true || const_out_R_state == true)
            {
                position_const_calc(1, 0);
                position_stereo_const_calc(3, 2);
            }
            break;
        case 2:
            position_stereo_calc(2, 0); // pot, motor
            if (const_out_L_state == true || const_out_R_state == true)
            {
                position_const_calc(2, 3);
                position_stereo_const_calc(0, 1);
            }
            break;
        case 3:
            position_stereo_calc(3, 1); // pot, motor
            if (const_out_L_state == true || const_out_R_state == true)
            {
                position_const_calc(3, 2);
                position_stereo_const_calc(1, 0);
            }
            break;
        }
    }

    else if (const_out_L_state == true) // si uniquement  const_out_L actif
    {
        const int gain_const = 0;
        const int volume_const = 1;
        switch (i)
        {
        case gain_const:
            position_const_calc(gain_const, volume_const);
            break;
        case volume_const:
            position_const_calc(volume_const, gain_const);
            break;
        default:
            break;
        }
#ifdef DEBUG // si DEBUG activé
        breakpoint();
#endif
    }

    else if (const_out_R_state == true) // si uniquement const_out_L actif
    {
        const int gain_const = 2;
        const int volume_const = 3;
        switch (i)
        {
        case gain_const:
            position_const_calc(gain_const, volume_const);
            break;
        case volume_const:
            position_const_calc(volume_const, gain_const);
            break;
        default:
            break;
        }
#ifdef DEBUG // si DEBUG activé
        breakpoint();
#endif
    }
    /*
    Serial.print("position_set[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(position_set[i]);
    Serial.print("position_memory[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(position_memory[i]);
    Serial.print("position_lue[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(position_lue[i]);
    */
}