#include <LiquidCrystal.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
 
// define some values used by the panel and buttons
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
double set1 = 2.35534;
double set2 = 12.5;

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

int menu;
int max_menu = 2;

int temp_overview = 0;
int set_temp1 = 1;
int set_temp2 = 2;
 
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

String getLCDNumberFormat(double in) {

  return "jou";
}
 
void setup(){
   lcd.begin(16, 2);               // start the library
}
  
void loop(){
  
  read_LCD_buttons();

  if (menu == temp_overview) {
  
    lcd.setCursor(0,0);
    lcd.print("Ts");
    if (temp1 < 10) {
      lcd.print(0);
    }
    lcd.print(temp1);
    
    lcd.setCursor(9,0);
    lcd.print("Tc");
    if (temp2 < 10) {
      lcd.print(0);
    }
    lcd.print(temp2);
    
    lcd.setCursor(0,1);
    lcd.print("Ss");
    if (set1 < 10) {
      lcd.print(0);
    }
    lcd.print(set1);
    
    lcd.setCursor(9,1);
    lcd.print("Sc");
    if (set2 < 10) {
      lcd.print(0);
    }
    lcd.print(set2);

    if (down && !prev_down) {
      lcd.clear();
      menu++;
    }
    else if (up && !prev_up) {
      lcd.clear();
      menu--;
    }
  }
  else if (menu == set_temp1) {

    if (!freeze) {
          
      lcd.setCursor(0,0);
      lcd.print("Setpoint sample");
      lcd.setCursor(0,1);
      if (set1 < 10) {
        lcd.print(0);
      }
      lcd.print(set1);
      lcd.setCursor(0,1);

      freeze = true;
    }

    if (down && !prev_down) {
      lcd.clear();
      freeze = false;
      menu++;
    }
    else if (up && !prev_up) {
      lcd.clear();
      freeze = false;
      menu--;
    }
    else if (!edit && select && !prev_select) {
      lcd.blink();
    }
  }
  else if (menu == set_temp2) {
        
    lcd.setCursor(0,0);
    lcd.print("Setpoint chamber");
    lcd.setCursor(0,1);
    if (set2 < 10) {
      lcd.print(0);
    }
    lcd.print(set2);

    if (down && !prev_down) {
      lcd.clear();
      menu++;
    }
    else if (up && !prev_up) {
      lcd.clear();
      menu--;
    }
    else if (!edit && select && !prev_select) {
      lcd.print("jou");
      lcd.cursor();
      lcd.blink();
    }
  }

  if (menu < 0)             menu = max_menu;
  else if (menu > max_menu) menu = 0;
}
