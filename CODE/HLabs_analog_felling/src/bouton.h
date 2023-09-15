/*******************************************************************************************************
gestion des états de boutton
*******************************************************************************************************/

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// création instance bouton
Bounce2::Button const_out_L = Bounce2::Button(); // instance du bouton const_out_L
Bounce2::Button const_out_R = Bounce2::Button(); // instance du bouton const_out_R
Bounce2::Button stereo_link = Bounce2::Button(); // instance du bouton stereo_link

void button_position_save(); // enregistrement des différences de valeurs
void lecture_switch();       // lecture bouton
void bouton_set();           // gestion des boutons
void stereo_link_set();      // gestion du bouton stereo_link
void const_out_L_set();      // gestion du bouton const_out_L
void const_out_R_set();      // gestion du bouton const_out_R

void button_position_save() // calcul de la différence entre gain et volume
{
    if (const_out_L_state == true) // si bouton const_out_L appuyé
    {
        for (int i = 0; i <= 1; i++)
        {
            moteur_stop(i);                    // arrêt des moteurs
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_memory[i] = position_lue[i];
        }
    }

    if (const_out_R_state == true) // si bouton const_out_R appuyé
    {
        for (int i = 2; i <= 3; i++)
        {
            moteur_stop(i);                    // arrêt des moteurs
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_memory[i] = position_lue[i];
        }
    }

    if (stereo_link_state == true) // si bouton stereo_link appuyé
    {
        for (int i = 0; i <= 3; i++)
        {
            moteur_stop(i); // arrêt des moteurs
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_memory[i] = position_lue[i];
        }
    }
}

void lecture_switch()
{
    // lecture des boutons
    stereo_link.update();
    const_out_L.update();
    const_out_R.update();
    // mise à jour des états des boutons
    if (stereo_link.pressed())
    {
        stereo_link_state = !stereo_link_state;
    }
    if (const_out_L.pressed())
    {
        const_out_L_state = !const_out_L_state;
    }
    if (const_out_R.pressed())
    {
        const_out_R_state = !const_out_R_state;
    }
}

void bouton_set()
{
    lecture_switch();
    if (stereo_link_state_old != stereo_link_state) // si changement d'état du bouton stereo_link
    {
        stereo_link_set();                         // gestion du bouton stereo_link
        state_button_change = true;                // il y a changement d'état d'un bouton
        stereo_link_state_old = stereo_link_state; // sauvegarde état bouton stereo_link
    }
    if (const_out_L_state_old != const_out_L_state) // si changement d'état du bouton const_out_L
    {
        const_out_L_set();                         // gestion du bouton const_out_L
        state_button_change = true;                // il y a changement d'état d'un bouton
        const_out_L_state_old = const_out_L_state; // sauvegarde état bouton const_out_L
    }
    if (const_out_R_state_old != const_out_R_state) // si changement d'état du bouton const_out_R
    {
        const_out_R_set();                         // gestion du bouton const_out_R
        state_button_change = true;                // il y a changement d'état d'un bouton
        const_out_R_state_old = const_out_R_state; // sauvegarde état bouton const_out_R
    }
}

void stereo_link_set() // gestion du bouton stereo_link
{
    if (stereo_link_state == true) // si bouton stereo_link appuyé
    {
        digitalWrite(stereo_link_led, HIGH);                        // allume LED stereo_link
        if (const_out_L_state == true || const_out_R_state == true) // si bouton const_out_L ou R appuyé
        {
            const_out_L_state = stereo_link_state;
            const_out_R_state = stereo_link_state;
        }
    }
    else // si bouton stereo_link relaché
    {
        digitalWrite(stereo_link_led, LOW); // éteint LED stereo_link
        // sécurité d'arrêt des moteurs
        for (int i = 0; i <= 3; i++) // pour chaque moteur
        {
            motor[i].brake(); // frein moteur
            delay(5);
            motor[i].stop(); // arrêt moteur
        }
    }
}

void const_out_L_set() // gestion du bouton const_out_L
{
    if (const_out_L_state == true) // si bouton const_out_L appuyé
    {
        digitalWrite(const_out_L_led, HIGH); // allume LED const_out_L
        if (stereo_link_state == true)       // si bouton stereo_link appuyé
        {
            const_out_R_state = true;
        }
    }
    else if (stereo_link_state != true) // si bouton const_out_L relaché et pas de stereo_link
    {
        digitalWrite(const_out_L_led, LOW); // éteint LED const_out_L
        // sécurité d'arrêt des moteurs
        for (int i = 0; i <= 1; i++) // pour chaque moteur
        {
            motor[i].brake(); // frein moteur
            delay(5);
            motor[i].stop(); // arrêt moteur
        }
    }
}

void const_out_R_set() // gestion du bouton const_out_R
{
    if (const_out_R_state == true) // si bouton const_out_R appuyé
    {
        digitalWrite(const_out_R_led, HIGH); // allume LED const_out_R
        if (stereo_link_state == true)       // si bouton stereo_link appuyé
        {
            const_out_L_state = true;
        }
    }
    else if (stereo_link_state != true) // si bouton const_out_L relaché et pas de stereo_link
    {
        digitalWrite(const_out_R_led, LOW); // éteint LED const_out_R
        // sécurité d'arrêt des moteurs
        for (int i = 2; i <= 3; i++) // pour chaque moteur
        {
            motor[i].brake(); // frein moteur
            delay(5);
            motor[i].stop(); // arrêt moteur
        }
    }
}