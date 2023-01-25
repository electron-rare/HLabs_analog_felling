// *******************************************************************************************************
// ****************************************** gestion bouttons ********************************************
// *******************************************************************************************************

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// création instance bouton
Bounce2::Button const_out_L = Bounce2::Button(); // instance du bouton const_out_L
Bounce2::Button const_out_R = Bounce2::Button(); // instance du bouton const_out_R
Bounce2::Button stereo_link = Bounce2::Button(); // instance du bouton stereo_link

void lecture_switch();   // lecture bouton
void bouton_set();       // gestion des boutons
void stereo_link_set();  // gestion du bouton stereo_link
void const_out_L_set();  // gestion du bouton const_out_L
void const_out_R_set();  // gestion du bouton const_out_R

void lecture_switch()
{
    // lecture des boutons
    const_out_L.update();
    const_out_R.update();
    stereo_link.update();
    // mise à jour des états des boutons
    if (const_out_L.pressed())
    {
        const_out_L_state = !const_out_L_state;
    }
    if (const_out_R.pressed())
    {
        const_out_R_state = !const_out_R_state;
    }
    if (stereo_link.pressed())
    {
        stereo_link_state = !stereo_link_state;
    }
}

void bouton_set()
{
    lecture_switch();
    if (stereo_link_state_old != stereo_link_state) // si changement d'état du bouton stereo_link
    {
        stereo_link_set();                         // gestion du bouton stereo_link
        state_button_change = 1;                   // il y a changement d'état d'un bouton
        stereo_link_state_old = stereo_link_state; // sauvegarde état bouton stereo_link
    }
    if (const_out_L_state_old != const_out_L_state) // si changement d'état du bouton const_out_L
    {
        const_out_L_set();          // gestion du bouton const_out_L
        if (stereo_link_state == 1) // si bouton stereo_link appuyé
        {
            const_out_R_state = const_out_L_state;     // copie état bouton const_out_L
            const_out_R_state_old = const_out_L_state; // sauvegarde copie état bouton const_out_R
        }
        state_button_change = 1;                   // il y a changement d'état d'un bouton
        const_out_L_state_old = const_out_L_state; // sauvegarde état bouton const_out_L
    }
    if (const_out_R_state_old != const_out_R_state) // si changement d'état du bouton const_out_R
    {
        const_out_R_set();          // gestion du bouton const_out_R
        if (stereo_link_state == 1) // si bouton stereo_link appuyé
        {
            const_out_L_state = const_out_R_state;     // copie état bouton const_out_R
            const_out_L_state_old = const_out_R_state; // sauvegarde copie état bouton const_out_L
        }
        state_button_change = 1;                   // il y a changement d'état d'un bouton
        const_out_R_state_old = const_out_R_state; // sauvegarde état bouton const_out_R
    }

#ifdef DEBUG_BOUTON // si DEBUG activé
    if (state_button_change == true)
    {
        Log.notice("bouton_change" CR);
        Log.trace("const_out_L_state = %d" CR, const_out_L_state);
        Log.trace("const_out_R_state = %d" CR, const_out_R_state);
        Log.trace("stereo_link_state = %d" CR, stereo_link_state);
    }
#endif
}

void stereo_link_set() // gestion du bouton stereo_link
{
#ifdef DEBUG_BOUTON // si DEBUG activé
    Log.notice(F(CR "void stereo_link_set()" CR));
#endif
    if (stereo_link_state == 1) // si bouton stereo_link appuyé
    {
        digitalWrite(stereo_link_led, HIGH);                  // allume LED stereo_link
        if (const_out_L_state == 1 || const_out_R_state == 1) // si bouton const_out_L ou R appuyé
        {
            const_out_L_state = 1;
            const_out_R_state = 1;
        }
    }
    else // si bouton stereo_link relaché
    {
        digitalWrite(stereo_link_led, LOW); // éteint LED stereo_link
        if (const_out_L_state == 0)
        {
            motor[0].stop(); // arrêt moteur 0
            motor[1].stop(); // arrêt moteur 1
            motor[2].stop(); // arrêt moteur 2
            motor[3].stop(); // arrêt moteur 3
        }
    }
}

void const_out_L_set() // gestion du bouton const_out_L
{
#ifdef DEBUG_BOUTON // si DEBUG activé
    Log.notice(F(CR "void const_out_L_set()" CR));
#endif
    if (const_out_L_state == 1) // si bouton const_out_L appuyé
    {
        digitalWrite(const_out_L_led, HIGH); // allume LED const_out_L
    }
    else // si bouton const_out_L relaché
    {
        digitalWrite(const_out_L_led, LOW); // éteint LED const_out_L
        motor[0].stop();                    // arrêt moteur 0
        motor[1].stop();                    // arrêt moteur 1
    }
}

void const_out_R_set() // gestion du bouton const_out_R
{
#ifdef DEBUG_BOUTON // si DEBUG activé
    Log.notice(F(CR "void const_out_R_set()" CR));
#endif
    if (const_out_R_state == 1) // si bouton const_out_R appuyé
    {
        digitalWrite(const_out_R_led, HIGH); // allume LED const_out_R
    }
    else // si bouton const_out_R relaché
    {
        digitalWrite(const_out_R_led, LOW); // éteint LED const_out_R
        motor[2].stop();                    // arrêt moteur 2
        motor[3].stop();                    // arrêt moteur 3
    }
}
