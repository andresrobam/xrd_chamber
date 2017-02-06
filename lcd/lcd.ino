#include <LiquidCrystal.h>
#include <EEPROMex.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int buttonPin = A0;

int EEPROM_init_address = 100;
long EEPROM_init_key = 13653473;

double sample_kp = 2;
double sample_ki = 5;
double sample_kd = 1;
double chamber_kp = 2;
double chamber_ki = 5;
double chamber_kd = 1;
int sample_kp_address = 10;
int sample_ki_address = 20;
int sample_kd_address = 30;
int chamber_kp_address = 40;
int chamber_ki_address = 50;
int chamber_kd_address = 60;

double sample_temp = 2.35;
double chamber_temp = 82.25;
double sample_set = 75.47;
double chamber_set = 12.95;
int sample_out = 40;
int chamber_out = 94;
double sensor_temp = 12.12;
double sensor_rh = 9.00;

boolean flip_peltier;
int flip_peltier_address = 0;
boolean control_sample;
boolean control_chamber;

boolean right;
boolean up;
boolean down;
boolean left;
boolean select;

boolean prev_right;
boolean prev_up;
boolean prev_down;
boolean prev_left;
boolean prev_select;

boolean edit;
boolean freeze;

int cursor_position;

int menu;
int max_menu = 16;

int temp_info = 0;
int set_info = 1;
int sample_info = 2;
int chamber_info = 3;
int sensor_info = 4;
int sample_control = 5;
int chamber_control = 6;
int sample_set_menu = 7;
int chamber_set_menu = 8;
int sample_kp_menu = 9;
int sample_ki_menu = 10;
int sample_kd_menu = 11;
int chamber_kp_menu = 12;
int chamber_ki_menu = 13;
int chamber_kd_menu = 14;
int peltier_flip = 15;
int defaults_menu = 16;
 
void setup() {

  if (EEPROM.readLong(EEPROM_init_address) != EEPROM_init_key) setDefaults(true);
  
  flip_peltier = EEPROM.read(flip_peltier_address);
  sample_kp = EEPROM.readDouble(sample_kp_address);
  sample_ki = EEPROM.readDouble(sample_ki_address);
  sample_kd = EEPROM.readDouble(sample_kd_address);
  chamber_kp = EEPROM.readDouble(chamber_kp_address);
  chamber_ki = EEPROM.readDouble(chamber_ki_address);
  chamber_kd = EEPROM.readDouble(chamber_kd_address);
  pinMode(buttonPin, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  
  readButtons();
  drawScreen();
}

void readButtons() {
  
  prev_right = right;
  prev_up = up;
  prev_down = down;
  prev_left = left;
  prev_select = select;
  
  int adc_key_in = analogRead(buttonPin);
  
  right = false;
  up = false;
  down = false;
  left = false;
  select = false;
  
  if (adc_key_in < 50) right = true;
  else if (adc_key_in < 250) up = true;
  else if (adc_key_in < 450) down = true;
  else if (adc_key_in < 650) left = true;
  else if (adc_key_in < 850) select = true;
}
 
void drawScreen() {

  if (menu == temp_info) {
    
    lcd.setCursor(0,0);
    lcd.print("Sample  T:");
    printDouble(sample_temp);

    lcd.setCursor(0,1);
    lcd.print("Chamber T:");
    printDouble(chamber_temp);
  
    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
  }
  else if (menu == set_info) {

    lcd.setCursor(0,0);
    lcd.print("Sample  S:");
    printDouble(sample_set);

    lcd.setCursor(0,1);
    lcd.print("Chamber S:");
    printDouble(chamber_set);
  
    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
  }
  else if (menu == sample_info) {

    lcd.setCursor(0,0);
    lcd.print("Sample  T:");
    printDouble(sample_temp);

    lcd.setCursor(0,1);
    lcd.print(sample_out);
    lcd.print("%");

    lcd.setCursor(8,1);
    lcd.print("S:");
    printDouble(sample_set);

    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
  }
  else if (menu == chamber_info) {

    lcd.setCursor(0,0);
    lcd.print("Chamber T:");
    printDouble(chamber_temp);

    lcd.setCursor(0,1);
    lcd.print(chamber_out);
    lcd.print("%");

    lcd.setCursor(8,1);
    lcd.print("S:");
    printDouble(chamber_set);

    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
  }
  else if (menu == sensor_info) {

    lcd.setCursor(0,0);
    lcd.print("Sensor  T:");
    printDouble(sensor_temp);
  
    lcd.setCursor(7,1);
    lcd.print("RH:");
    printDouble(sensor_rh);

    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
  }
  else if (menu == sample_control) {

    lcd.setCursor(0,0);
    lcd.print("Control sample");
    lcd.setCursor(0,1);
    lcd.print("temperature: ");

    if (control_sample) lcd.print("Yes");
    else lcd.print("No ");
    
    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
    else if (select && !prev_select) control_sample = !control_sample;
  }
  else if (menu == chamber_control) {

    lcd.setCursor(0,0);
    lcd.print("Control chamber");
    lcd.setCursor(0,1);
    lcd.print("temperature: ");

    if (control_chamber) lcd.print("Yes");
    else lcd.print("No ");
    
    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
    else if (select && !prev_select) control_chamber = !control_chamber;
  }
  else if (menu >= sample_set_menu && menu <= chamber_kd_menu) {

    if (!freeze) {
      
      lcd.setCursor(0,0);
      lcd.print(getEditTitle());
      lcd.setCursor(0,1);
      printEditDouble(getEditValue());

      if (edit) {
        
        freeze = true;
        lcd.setCursor(4,1);
        cursor_position = 4;
      }
      else {
        
        if (down && !prev_down) menuUp();
        else if (up && !prev_up) menuDown();
      }
    }
    else {

      if (left && !prev_left) cursorLeft();
      else if (right && !prev_right) cursorRight();
      else if (down && !prev_down) setEditValue(false, getEditValue());
      else if (up && !prev_up) setEditValue(true, getEditValue());
    }
    if (select && !prev_select) menuEdit();
  }
  else if (menu == peltier_flip) {

    lcd.setCursor(0,0);
    lcd.print("Flip peltier");
    lcd.setCursor(0,1);
    lcd.print("polarity: ");

    if (flip_peltier) lcd.print("Yes");
    else lcd.print("No ");
    
    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
    else if (select && !prev_select) {
      flip_peltier = !flip_peltier;
      EEPROM.write(flip_peltier_address, flip_peltier);
    }
  }
  else if (menu == defaults_menu) {

    lcd.setCursor(0,0);
    lcd.print("Press SELECT to");
    lcd.setCursor(0,1);
    lcd.print("reset values.");
    
    if (down && !prev_down) menuUp();
    else if (up && !prev_up) menuDown();
    else if (select && !prev_select) {
      setDefaults(false);
    }
  }
}

void menuUp() {

  menuClear();
  menu++;
  if (menu > max_menu) menu = 0;
}

void menuDown() {

  menuClear();
  menu--;
  if (menu < 0) menu = max_menu;
}

void menuEdit() {

  menuClear();
  edit = !edit;
  freeze = false;

  if (edit) lcd.blink();
}

void menuClear() {
  
  lcd.clear();
  lcd.noBlink();
}

void cursorLeft() {

  cursor_position--;
  if (cursor_position < 0) cursor_position = 0;
  else if (cursor_position == 2) cursor_position = 1;
  lcd.setCursor(cursor_position,1);
}

void cursorRight() {

  cursor_position++;
  if (cursor_position > 4) cursor_position = 4;
  else if (cursor_position == 2) cursor_position = 3;
  lcd.setCursor(cursor_position,1);
}

void setEditValue(boolean positive, double &val) {

  lcd.setCursor(0,1);

  double add = getSize(positive);

  val += add;

  if (val < 0) val -= add;
  else if (val >= 100) val -= add;
  printEditDouble(val);

  if (getEditValueAddress() != 100) {

    EEPROM.writeDouble(getEditValueAddress(), val);
  }
  
  lcd.setCursor(cursor_position,1);
}

double getSize(boolean positive) {

  double out = 0;

  if (cursor_position == 0) out = 10;
  else if (cursor_position == 1) out = 1;
  else if (cursor_position == 3) out = 0.1;
  else if (cursor_position == 4) out = 0.01;

  if (!positive) out *= -1;

  return out;
}

void printDouble(double in) {

  String prefix = "";

  if (in < 10 && in >= 0) prefix = "  ";
  else if (in >= 10 || (in < 0 && in > -10)) prefix = " ";

  lcd.print(prefix+in);
}

void printEditDouble(double in) {
  
  if (in < 10) lcd.print(0);
  lcd.print(in);
}

String getEditTitle() {

  if (menu == sample_set_menu) return "Sample setpoint";
  else if (menu == chamber_set_menu) return "Chamber setpoint";
  else if (menu == sample_kp_menu) return "Sample Kp";
  else if (menu == sample_ki_menu) return "Sample Ki";
  else if (menu == sample_kd_menu) return "Sample Kd";
  else if (menu == chamber_kp_menu) return "Chamber Kp";
  else if (menu == chamber_ki_menu) return "Chamber Ki";
  else if (menu == chamber_kd_menu) return "Chamber Kd";
}

double &getEditValue() {

  if (menu == sample_set_menu) return sample_set;
  else if (menu == chamber_set_menu) return chamber_set;
  else if (menu == sample_kp_menu) return sample_kp;
  else if (menu == sample_ki_menu) return sample_ki;
  else if (menu == sample_kd_menu) return sample_kd;
  else if (menu == chamber_kp_menu) return chamber_kp;
  else if (menu == chamber_ki_menu) return chamber_ki;
  else if (menu == chamber_kd_menu) return chamber_kd;
}

double getEditValueAddress() {

  if (menu == sample_kp_menu) return sample_kp_address;
  else if (menu == sample_ki_menu) return sample_ki_address;
  else if (menu == sample_kd_menu) return sample_kd_address;
  else if (menu == chamber_kp_menu) return chamber_kp_address;
  else if (menu == chamber_ki_menu) return chamber_ki_address;
  else if (menu == chamber_kd_menu) return chamber_kd_address;
  else return 100;
}

void setDefaults(boolean initialize) {

  if (initialize) EEPROM.writeLong(EEPROM_init_address, EEPROM_init_key);
  EEPROM.write(flip_peltier_address, false);
  EEPROM.writeDouble(sample_kp_address, 2);
  EEPROM.writeDouble(sample_ki_address, 5);
  EEPROM.writeDouble(sample_kd_address, 1);
  EEPROM.writeDouble(chamber_kp_address, 2);
  EEPROM.writeDouble(chamber_ki_address, 5);
  EEPROM.writeDouble(chamber_kd_address, 1);

  flip_peltier = false;
  sample_kp = 2;
  sample_ki = 5;
  sample_kd = 1;
  chamber_kp = 2;
  chamber_ki = 5;
  chamber_kd = 1;
}
