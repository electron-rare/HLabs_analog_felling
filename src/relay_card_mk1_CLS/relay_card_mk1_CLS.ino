// relay card

#include <Wire.h>  // lib pour gestion I2C
#include <ResponsiveAnalogRead.h>
#include <SparkFunMiniMoto.h>  // Include the MiniMoto library

// Create two MiniMoto instances, with different address settings.
MiniMoto motor0(0xCE); // A1 = 1, A0 = clear
MiniMoto motor1(0xD0); // A1 = 1, A0 = 1 (default)

#define vol_pot_1  A0
#define gain_pot_1  A1

#define vol_pot_2  A2
#define gain_pot_2  A3

#define const_out_sw  2 // = switchApin

ResponsiveAnalogRead vol_1(vol_pot_1, true);
ResponsiveAnalogRead gain_1(gain_pot_1, true);

ResponsiveAnalogRead vol_2(vol_pot_2, true);
ResponsiveAnalogRead gain_2(gain_pot_2, true);
//ResponsiveAnalogRead analog2(gainpin, true);

byte level = 0;               //      info qu'on envoi en I2C pour l'attenuateur relais
float pot1value = 0;          //   valeur position du potentiometre principal
const int potprincipal = A0;  // pinuche pour la mesure de position du potentiometre principal sur le nano : A2
int difference = 0;           // difference entre gain et volume
byte switchread = 1;
byte const_out_value_old = 0;
byte const_out_value = 0;
byte compensation = 0;
byte actualvalue = 127;
byte actualvaluegain = 127;
int newvalue = 0;
byte newvaluegain = 127;
byte smooth = 0;
byte smoothgain = 127;
byte smoothgainmap = 0;
byte gaincomensatedroot = 0;
byte I2Csend = 127;
byte I2Csendgain = 127;
unsigned long delaytime = 1;

// the setup function runs once when you press reset or power the board
void setup() {

  Wire.begin();  // join i2c bus (address optional for master)
  pinMode(vol_pot_1, INPUT);
  pinMode(gain_pot_1, INPUT);
  pinMode(vol_pot_2, INPUT);
  pinMode(gain_pot_2, INPUT);
  pinMode(const_out_sw, INPUT_PULLUP);
  
  Serial.begin(115200);
  Serial.setTimeout(5);
  
  delay(100);  // give me time to bring up serial monitor
  
  const_out_value_old = digitalRead(const_out_sw);
  
  Serial.println("relay card test");
}

// the loop function runs over and over again forever
void loop() {

  // update the ResponsiveAnalogRead object every loop
  vol_1.update();
  gain_1.update();
  vol_2.update();
  gain_2.update();

  vol_pot();              // lecture du potentiometre volume
  gain_pot();              // lecture du potentiometre gain
  mapgain ();           // mappage du potentiometre de gain
  anticlikvol ();       // gestion du clik volume
  anticlikgain ();      // gestion du clik gain
  serialmonitoring ();  // serial monitoring
  switchA ();           // lire le switch A
}
