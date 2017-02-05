#include <LiquidCrystal.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int buttonPin = A0;

double sample_temp = 2.35;
double chamber_temp = 82.25;
double sample_set = 75.47;
double chamber_set = 12.95;
int sample_out = 40;
int chamber_out = 94;
double sensor_temp = 12.12;
double sensor_rh = 9.00;

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
int max_menu = 2;

int sample_info = 0;
int chamber_info = 1;
int sensor_info = 2;
 
void setup() {

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

  if (menu == sample_info) {

    if (!edit) {
      
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
    else if (!freeze) {

      lcd.setCursor(0,0);
      lcd.print("Sample setpoint");
  
      lcd.setCursor(0,1);
      if (sample_set < 10) lcd.print(0);
      lcd.print(sample_set);

      freeze = true;
      lcd.setCursor(4,1);
      cursor_position = 4;
    }
    else if (freeze) {

      if (left && !prev_left) cursorLeft();
      else if (right && !prev_right) cursorRight();
      else if (down && !prev_down) setTemp(false);
      else if (up && !prev_up) setTemp(true);
    }
    
    if (select && !prev_select) menuEdit();
  }
  else if (menu == chamber_info) {

    if (!edit) {
      
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
    else if (!freeze) {

      lcd.setCursor(0,0);
      lcd.print("Chamber setpoint");
  
      lcd.setCursor(0,1);
      if (chamber_set < 10) lcd.print(0);
      lcd.print(chamber_set);

      freeze = true;
      lcd.setCursor(4,1);
      cursor_position = 4;
    }
    else if (freeze) {

      if (left && !prev_left) cursorLeft();
      else if (right && !prev_right) cursorRight();
      else if (down && !prev_down) setTemp(false);
      else if (up && !prev_up) setTemp(true);
    }
    
    if (select && !prev_select) menuEdit();
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

  if (edit) lcd.blink();
  else freeze = false;
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

void setTemp(boolean positive) {

  lcd.setCursor(0,1);

  double add = getSize(positive);

  if (menu == sample_info) {

    sample_set += add;

    if (sample_set < 0) sample_set -= add;
    else if (sample_set >= 100) sample_set -= add;
    if (sample_set < 10) lcd.print(0);
    lcd.print(sample_set);
  }
  else if (menu == chamber_info) {

    chamber_set += add;

    if (chamber_set < 0) chamber_set -= add;
    else if (chamber_set >= 100) chamber_set -= add;
    if (chamber_set < 10) lcd.print(0);
    lcd.print(chamber_set);
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
