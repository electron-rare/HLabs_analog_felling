
void diff_set();                                           // calcul des différences de valeurs
void consigne_set();                                       // calcul des consignes de moteurs
void consigne_calc(int pot, int motor, int position_calc); // calcul des consignes de moteurs

void diff_set() // calcul de la différence entre gain et volume
{
#ifdef DEBUG_DIFF // si DEBUG activé
    Log.notice(F("======= void diff set" CR));
#endif
    if (const_out_L_state == 1) // si bouton const_out_L appuyé
    {
        for (int i = 0; i <= 1; i++)
        {
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_change[i] = false;
            save_pot(i); // sauvegarde position potentiomètre
        }
        diff_const_out_L = position_lue[0] - position_lue[1]; // calcul de la différence entre gain et volume gauche
    }

    if (const_out_R_state == 1) // si bouton const_out_R appuyé
    {
        for (int i = 2; i <= 3; i++)
        {
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_change[i] = false;
            save_pot(i); // sauvegarde position potentiomètre
        }
        diff_const_out_R = position_lue[2] - position_lue[3]; // calcul de la différence entre gain et volume droit
    }

    if (stereo_link_state == 1) // si bouton stereo_link appuyé
    {
        for (int i = 0; i <= 3; i++)
        {
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_change[i] = false;
            save_pot(i); // sauvegarde position potentiomètre
        }
        diff_gain = position_lue[0] - position_lue[2]; // calcul de la différence entre gain gauche et droit
        diff_vol = position_lue[1] - position_lue[3];  // calcul de la différence entre volume gauche et droit
    }
#ifdef DEBUG_DIFF // si DEBUG activé
    Log.notice(F("diff_const_out_L = %d" CR), diff_const_out_L);
    Log.notice(F("diff_const_out_R = %d" CR), diff_const_out_R);
    Log.notice(F("diff_gain = %d" CR), diff_gain);
    Log.notice(F("diff_vol = %d" CR), diff_vol);
    for (int i = 0; i <= 3; i++)
    {
        Log.notice(F("position_lue[%d] = %d" CR), i, position_lue[i]);
        Log.notice(F("position_set[%d] = %d" CR), i, position_set[i]);
    }

#endif
}

void consign_calc(int pot, int motor, int position_calc) // calcul des consignes de moteurs
{
    lecture_pot(pot);                      // lecture des potentiomètres
    lecture_pot(motor);                    // lecture des potentiomètres
    position_set[pot] = position_lue[pot]; // set position potentiomètre gain et volume gauche et droit
#ifdef DEBUG_CONST                         // si DEBUG activé
    Log.notice(F("======= void consigne calc" CR));
    Log.notice(F("pot = %d position_lue[%d] = %d" CR), pot, pot, position_lue[pot]);
    Log.notice(F("pot = %d position_set[%d] = %d" CR), pot, pot, position_set[pot]);
    Log.notice(F("pot = %d position_save[%d] = %d" CR), pot, pot, position_save[pot]);
    Log.notice(F("motor = %d position_lue[%d] = %d" CR), motor, motor, position_lue[motor]);
    Log.notice(F("motor = %d position_set[%d] = %d" CR), motor, motor, position_set[motor]);
    Log.notice(F("motor = %d position_save[%d] = %d" CR), motor, motor, position_save[motor]);
    Log.notice(F("motor_change[%d] = %d" CR), motor, motor_change[motor]);
    Log.notice(F("pot motor_change[%d] = %d" CR), pot, motor_change[pot]);
    Log.notice(F("time elapsed= %d" CR), millis() - last_change_time);
    Log.notice(F("bounce_time_pot = %d" CR), bounce_time_pot);
    Log.notice(F("position_calc = %d" CR), position_calc);
    Log.notice(F("position_calc > (position_lue[motor] + ECRART_V_STOP) = %d" CR), position_calc > (position_lue[motor] + ECRART_V_STOP));
    Log.notice(F("position_calc < (position_lue[motor] - ECRART_V_STOP) = %d" CR), position_calc < (position_lue[motor] - ECRART_V_STOP));
#endif
    // position_calc = 60
    // ecart +
    // ecart -
    // si changement de position du potentiomètre depuis plus de bounce_time_pot et moteur n'est pas entre consigne et ECRART_V_STOP
    if (motor_change[motor] == false && motor_change[pot] == false)
    {
        if (position_calc >= 1023 || position_calc <= 0) // si consigne n'est pas hors bornes
        {
            save_pot(pot);   // sauvegarde position potentiomètre
            save_pot(motor); // sauvegarde position potentiomètre
            position_set[motor] = position_lue[motor];
#ifdef DEBUG_CONST // si DEBUG activé
            Log.notice(F("======= void consigne HSHSHSHSHSSH calc" CR));
            Log.notice(F("diff_const_out_L = %d" CR), diff_const_out_L);
            Log.notice(F("============================> position_calc = %d" CR), position_calc);
            Log.notice(F("motor = %d" CR), motor);
            Log.notice(F("position_lue[motor] = %d" CR), position_lue[motor]);
            Log.notice(F("position_set[motor] = %d" CR), position_set[motor]);
            Log.notice(F("position_save[motor] = %d" CR), position_save[motor]);
            Log.notice(F("pot = %d" CR), pot);
            Log.notice(F("position_lue[pot] = %d" CR), position_lue[pot]);
            Log.notice(F("position_set[pot] = %d" CR), position_set[pot]);
            Log.notice(F("position_save[pot] = %d" CR), position_save[pot]);
#endif
            return;
        }
        else if (position_calc > (position_lue[motor] + ECRART_V_STOP) || position_calc < (position_lue[motor] - ECRART_V_STOP)) // si consigne n'est pas hors bornes
        {
            position_set[motor] = position_calc; // set position potentiomètre volume gauche
            motor_change[motor] = true;          // il y a changement de position du potentiomètre de volume gauche
            myPID.SetMode(myPID.Control::timer); // set PID en mode automatique
#ifdef DEBUG_CONST // si DEBUG activé
            Log.notice(F("======= void consigne YES YES YES calc" CR));
            Log.notice(F("position_calc = %d" CR), position_calc);
#endif
        }
    }
}

void consigne_set() // calcul des consignes de moteurs
{
    if (const_out_L_state == true) // si bouton const_out_L appuyé
    {
        if (state_pot_change[0] == true && motor_change[0] == false && motor_change[1] == false) // si changement de position du potentiomètre de gain gauche et que pas de moteur en mouvement
        {
            int pot = 0;
            int motor = 1;
            int position_calc = constrain((position_lue[pot] - diff_const_out_L), 0, 1023); // set position potentiomètre volume gauche
            consign_calc(pot, motor, position_calc);
        }
        if (state_pot_change[1] == true && motor_change[0] == false && motor_change[1] == false) // si changement de position du potentiomètre de volume gauche et que pas de moteur en mouvement
        {
            int pot = 1;
            int motor = 0;
            int position_calc = constrain((position_lue[pot] + diff_const_out_L), 0, 1023); // set position potentiomètre volume gauche
            consign_calc(pot, motor, position_calc);
        }
    }
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
}
