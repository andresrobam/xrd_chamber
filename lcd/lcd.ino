#include <LiquidCrystal.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
 
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

double temp1 = 2.35534;
double temp2 = 12.5;
int out1 = 40;
int out2 = 94;
double set1 = 2.35534;
double set2 = 12.5;

double sensor_rh = 9.00;
double sensor_temp = 12.12;

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

int pos = 0;

int menu;
int max_menu = 2;

int info1 = 0;
int info2 = 1;
int info3 = 2;

void read_LCD_buttons()  {
  
  prev_right = right;
  prev_up = up;
  prev_down = down;
  prev_left = left;
  prev_select = select;
  
  adc_key_in = analogRead(0);
  
  right = false;
  up = false;
  down = false;
  left = false;
  select = false;
   
  if (adc_key_in > 1000) return;
   
  if (adc_key_in < 50) {
    right = true;
    return;
  }
  if (adc_key_in < 250) {
    up = true;
    return;
  }
  if (adc_key_in < 450) {
    down = true;
    return;
  }
  if (adc_key_in < 650) {
    left = true;
    return;
  }
  if (adc_key_in < 850) {
    select = true;
    return;
  }
}
 
void setup(){
   lcd.begin(16, 2);
}
  
void loop() {
  
  read_LCD_buttons();

  if (menu == info1) {

    if (!edit) {
      
      lcd.setCursor(0,0);
      lcd.print("Sample  T: ");
      if (temp1 < 10) {
        lcd.print(" ");
      }
      lcd.print(temp1);
  
      lcd.setCursor(0,1);
      lcd.print(out1);
      lcd.print("%");
  
      lcd.setCursor(8,1);
      lcd.print("S: ");
      if (set1 < 10) {
        lcd.print(" ");
      }
      lcd.print(set1);

      if (down && !prev_down) menuUp();
      else if (up && !prev_up) menuDown();
    }
    else if (!freeze) {

      lcd.setCursor(0,0);
      lcd.print("Sample setpoint");
  
      lcd.setCursor(0,1);
      if (set1 < 10) {
        lcd.print(0);
      }
      lcd.print(set1);

      freeze = true;
      lcd.setCursor(4,1);
      pos = 4;
    }
    else if (freeze) {

      if (left && !prev_left) cursorLeft();
      else if (right && !prev_right) cursorRight();
      else if (down && !prev_down) cursorDown();
      else if (up && !prev_up) cursorUp();
    }
    
    if (select && !prev_select) {
      menuEdit();
    }
  }
  else if (menu == info2) {
  
    lcd.setCursor(0,0);
    lcd.print("Chamber T: ");
    if (temp2 < 10) {
      lcd.print(" ");
    }
    lcd.print(temp2);

    lcd.setCursor(0,1);
    lcd.print(out2);
    lcd.print("%");

    lcd.setCursor(8,1);
    lcd.print("S: ");
    if (set2 < 10) {
      lcd.print(" ");
    }
    lcd.print(set2);

    if (down && !prev_down && !edit) {
      menuUp();
    }
    else if (up && !prev_up && !edit) {
      menuDown();
    }
    else if (select && !select) {
      edit = true;
    }
  }
  else if (menu == info3) {
  
    lcd.setCursor(0,0);
    lcd.print("Sensor  T: ");
    if (sensor_temp < 10) {
      lcd.print(" ");
    }
    lcd.print(sensor_temp);
  
    lcd.setCursor(5,1);
    lcd.print("RH: ");
    if (sensor_rh < 100) {
      lcd.print(" ");
    }
    if (sensor_rh < 10) {
      lcd.print(" ");
    }
    lcd.print(sensor_rh);
    lcd.print("%");

    if (down && !prev_down) {
      menuUp();
    }
    else if (up && !prev_up) {
      menuDown();
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

  if (edit) {

    lcd.blink();
  }
  else {

    freeze = false;
  }
}

void menuClear() {
  
  lcd.clear();
  lcd.noBlink();
}

void cursorLeft() {

  pos--;
  if (pos < 0) pos = 0;
  else if (pos == 2) pos = 1;
  lcd.setCursor(pos,1);
}

void cursorRight() {

  pos++;
  if (pos > 4) pos = 4;
  else if (pos == 2) pos = 3;
  lcd.setCursor(pos,1);
}

void cursorDown() {

  lcd.setCursor(0,1);

  if (menu == info1) {

    if (pos == 0) {
      if (temp1 >= 10) temp1 -= 10;
      if (temp1 < 10) lcd.print(0);
    }
    lcd.print(temp1);
  }
  
  lcd.setCursor(pos,1);
}

void cursorUp() {

  lcd.setCursor(0,1);

  if (menu == info1) {
      
    if (temp1 < 90) temp1 += 10;
    if (temp1 < 10) lcd.print(0);
    lcd.print(temp1);
  }

  lcd.setCursor(pos,1);
}

