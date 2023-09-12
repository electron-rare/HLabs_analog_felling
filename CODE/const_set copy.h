
void diff_set();                                           // calcul des différences de valeurs
void consigne_set();                                       // calcul des consignes de moteurs
void consigne_calc(int pot, int motor, int position_calc); // calcul des consignes de moteurs

void diff_set() // calcul de la différence entre gain et volume
{
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
}

void consign_calc(int pot, int motor, int position_calc) // calcul des consignes de moteurs
{
    lecture_pot(pot);                      // lecture des potentiomètres
    lecture_pot(motor);                    // lecture des potentiomètres
    position_set[pot] = position_lue[pot]; // set position potentiomètre gain et volume gauche et droit
    breakpoint();                          // breakpoint
    
    if (motor_change[motor] == false && motor_change[pot] == false)
    {
        if (position_calc >= 1023 || position_calc <= 0) // si consigne n'est pas hors bornes
        {
            save_pot(pot);   // sauvegarde position potentiomètre
            save_pot(motor); // sauvegarde position potentiomètre
            position_set[motor] = position_lue[motor];
            return;
        }
        else if (position_calc > (position_lue[motor] + ECRART_V_STOP) || position_calc < (position_lue[motor] - ECRART_V_STOP)) // si consigne n'est pas hors bornes
        {
            position_set[motor] = position_calc; // set position potentiomètre volume gauche
            motor_change[motor] = true;          // il y a changement de position du potentiomètre de volume gauche
            myPID.SetMode(myPID.Control::timer); // set PID en mode automatique
        }
    }
}

void consigne_set() // calcul des consignes de moteurs
{

    // si différence de position entre gain et volume gauche > pot - diff_set
    // si différence de position entre gain et volume droit > pot - diff_set
    
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
