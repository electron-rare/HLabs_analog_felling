// *******************************************************************************************************
// ****************************************** PIN MAPPING ************************************************
// *******************************************************************************************************
// définition LED
#define const_out_L_led 5 // Digital OUT led const_out_L
#define const_out_R_led 6 // Digital OUT led const_out_R
#define stereo_link_led 7 // Digital OUT led stereo_link
// définition switch
#define const_out_sw_L 2 // Digital IN switch const_out_L
#define const_out_sw_R 3 // Digital IN switch const_out_R
#define stereo_link_sw 4 // Digital IN switch stereo_link
// définition PIN curseur potentiomètre
#define gain_0_pot A1 // entrée pot gain gauche
#define vol_0_pot A0  // entrée pot volume gauche
#define gain_1_pot A2 // entrée pot gain droite
#define vol_1_pot A3  // entrée pot volume droite

// *******************************************************************************************************
// ****************************************** potentiomètre **********************************************
// *******************************************************************************************************
// adresse moteur
#define gain_0_motor 0xC8 // A0 = open, A1 = 1
#define vol_0_motor 0xCE  // A0 = open, A1 = open
#define gain_1_motor 0xD0 // A0 = 1, A1 = open
#define vol_1_motor 0xCA  // A0 = 1, A1 = 1

// variables potentiomètre
int consigne[4] = {0, 0, 0, 0};                                      // tableau de consigne des potentiomètre
int position_lue[4] = {0, 0, 0, 0};                                  // tableau de valeur lus sur les potentiomètre
int position_set[4] = {0, 0, 0, 0};                                  // tableau de consigne de position des potentiomètre
int position_save[4] = {0, 0, 0, 0};                                 // tableau de sauvegarde de valeurs des potentiomètre
bool position_change[4] = {false, false, false, false};              // tableau de flag de positionnement des potentiomètre
bool motor_change[4] = {false, false, false, false};                 // tableau de flag de changement de positionnement des potentiomètre
byte analog_pot[4] = {gain_0_pot, vol_0_pot, gain_1_pot, vol_1_pot}; // tableau de PIN des potentiomètre

unsigned long last_change_time = millis(); // remise à zéro du compteur de temps
#define bounce_time_pot 300 // interval de temps entre 2 lecture de l'état du potentiomètre

// *******************************************************************************************************
// ****************************************** relais *****************************************************
// *******************************************************************************************************
// adresse carte relais
#define gain_0_relais 0x39 // A0 = 0, A1 = 1, A2 = 1
#define vol_0_relais 0x38  // A0 = 1, A1 = 1, A2 = 1
#define gain_1_relais 0x00
#define vol_1_relais 0x00
// variables relais
byte relais_vol_val[2] = {0, 0};   // tableau de valeur des relais volumes
byte relais_gain_val[2] = {0, 0};  // tableau de valeur des relais gain
byte relais_old[4] = {0, 0, 0, 0}; // tableau de comparaison de valeur des relais
byte relais_map[4] = {0, 0, 0, 0}; // tableau de mappage des valeur des relais

// *******************************************************************************************************
// ****************************************** boutton *****************************************************
// *******************************************************************************************************
bool const_out_L_state_old = false; // variable de comparaison d'état du bouton const_out_L
bool const_out_R_state_old = false; // variable de comparaison d'état du bouton const_out_R
bool stereo_link_state_old = false; // variable de comparaison d'état du bouton stereo_link
bool const_out_L_state = false;     // variable d'état du bouton const_out_L
bool const_out_R_state = false;     // variable d'état du bouton const_out_R
bool stereo_link_state = false;     // variable d'état du bouton stereo_link
#define interval_button 10          // interval de temps entre 2 lecture de l'état du bouton

// *******************************************************************************************************
// ****************************************** variable diverse *******************************************
// *******************************************************************************************************
bool state_pot_change[4] = {false, false, false, false}; // variable de changement d'état
bool state_button_change = true;                         // variable de changement d'état
int diff_gain;                                           // variable de différence de valeur entre les 2 potentiomètre de gain
int diff_vol;                                            // variable de différence de valeur entre les 2 potentiomètre de volume
int diff_const_out_L;                                    // variable de différence de valeur du canal gauche
int diff_const_out_R;                                    // variable de différence de valeur du canal droite
int smoothgain[2] = {0, 0};                              // tableau de valeur de lissage du gain
int smoothvol[2] = {0, 0};                               // tableau de valeur de lissage du volume

// ******************************************************************************************************
#ifdef DEBUG                  // si DEBUG activé
unsigned long last_time;      // variable de comparaison de temps
#define interval_loop 1000    // variable de comparaison de temps
unsigned int debug_count = 1; // variable de comptage de boucle

void debug()
{
#ifdef DEBUG_LOOP_HARD // si DEBUG activé

    Log.verbose(F("--------------------------------------------------" CR));
    Log.verbose(F("-----------------  DEBUG  ------------------------" CR));
    Log.verbose(F("--------------------------------------------------" CR));
    Log.verbose(F("millis() = %d" CR), millis());
    Log.verbose(F("last_time = %d" CR), last_time);
    Log.verbose(F("interval_loop = %d" CR), interval_loop);
    Log.verbose(F("state_pot_change = %d" CR), state_pot_change);
    Log.verbose(F("state_button_change = %d" CR), state_button_change);
    Log.verbose(F("stereo_link_state = %d" CR), stereo_link_state);
    Log.verbose(F("const_out_L_state = %d" CR), const_out_L_state);
    Log.verbose(F("const_out_R_state = %d" CR), const_out_R_state);
    Log.verbose(F("stereo_link_state_old = %d" CR), stereo_link_state_old);
    Log.verbose(F("const_out_L_state_old = %d" CR), const_out_L_state_old);
    Log.verbose(F("const_out_R_state_old = %d" CR CR), const_out_R_state_old);
#ifdef DEBUG_LEFT
    Log.verbose(F("diff_const_out_L = %d" CR), diff_const_out_L);
    for (int i = 0; i <= 1; i++)
    {
        Log.verbose(F("--------< %d >--------" CR), i);
        Log.verbose(F("position_save[%d] = %d" CR), i, position_save[i]);
        Log.verbose(F("position_lue[%d] = %d" CR), i, position_lue[i]);
        Log.verbose(F("position_set[%d] = %d" CR), i, position_set[i]);
        Log.verbose(F("relais_old[%d] = %d" CR), i, relais_old[i]);
        Log.verbose(F("relais_map[%d] = %d" CR), i, relais_map[i]);
    }
#ifdef DEBUG_RIGHT
    Log.verbose(F("diff_const_out_R = %d" CR CR), diff_const_out_R);
    for (int i = 2; i <= 3; i++)
    {
        Log.verbose(F("--------< %d >--------" CR), i);
        Log.verbose(F("position_save[%d] = %d" CR), i, position_save[i]);
        Log.verbose(F("position_lue[%d] = %d" CR), i, position_lue[i]);
        Log.verbose(F("position_set[%d] = %d" CR), i, position_set[i]);
        Log.verbose(F("relais_old[%d] = %d" CR), i, relais_old[i]);
        Log.verbose(F("relais_map[%d] = %d" CR), i, relais_map[i]);
    }
#endif
    last_time = millis(); // remise à zéro du compteur de temps
    Log.verbose(F("--------------------------------------------------" CR));
    Log.verbose(F("----------------  END DEBUG  ---------------------" CR));
    Log.verbose(F("--------------------------------------------------" CR));
#endif
#endif
}
#endif