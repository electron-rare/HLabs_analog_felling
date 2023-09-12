//Bibliothèques LCD Keypad Shield
#include <LiquidCrystal.h>
// Création de l'objet lcd (avec les différents ports numériques qu'il utilise)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <SparkFunMiniMoto.h>  // Include the MiniMoto library

// Create  MiniMoto instances,
MiniMoto motor0(0xC8); // A1 = 1, A0 = clear

#define POT_REF A1   // entrée analogique reliée au commun de la piste du potentiomètre
int up = 0;
int down = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);              // Démarrage de l'écran
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HLAB POT ");
  lcd.setCursor(0, 1);
  lcd.print("TEST");
  delay(500);
}

void lcd_print()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UP : ");
  lcd.print(up);
  lcd.setCursor(0, 1);
  lcd.print("DOWN : ");
  lcd.print(down);
}

void loop()
{

  while (analogRead(POT_REF) < 1000) {
    motor0.drive(-63);
  }
  delay(1000);
  motor0.stop();
  up++;
  lcd_print();
  while (analogRead(POT_REF) > 10) {
    motor0.drive(63);
  }
  delay(1000);
  motor0.stop();
  down++;
  lcd_print();
}