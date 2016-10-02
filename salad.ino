// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int pos = 0;
int milli_count_prev = 0;
int milli_count = 0;
int tick = 0;
int led_count = 0;
int lives = 3;
int enemy_lives = 3;
int missiles[16];
int l_button = 0;
int r_button = 0;
int command = -1;
int recharge = 0;

byte ship[8] = {
  B00000,
  B00000,
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111
};

byte missile0[8] = {
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte missile1[8] = {
  B00000,
  B00000,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte missile2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00000,
  B00000,
  B00000
};

byte missile3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00000
};

void setup() {
  for(int d = 8; d <= 12; d++) {
    pinMode(d, INPUT);
    digitalWrite(d, HIGH);
  }
  for(int m = 0; m < 16; m++) missiles[m] = -1;
  lcd.createChar(0, ship);
  lcd.createChar(1, missile3);
  lcd.createChar(2, missile2);
  lcd.createChar(3, missile1);
  lcd.createChar(4, missile0);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  if(tick % 100 == 0) do_tick();
  //button pressing
  if(!digitalRead(8)) r_button = 1;
  else r_button = 0;
  if(!digitalRead(12)) l_button = 1;
  else l_button = 0;
  
  //increment timer;
  if(tick >= 100) tick = 0;
  if(millis() - milli_count_prev > 20) {
    tick++;
    milli_count_prev = milli_count;
  }
  delay(1);
}

void do_tick() {
  //input handling
  if(command >= 0 && command < 16) {
    missiles[command] = 4;
    command = -1;
  }
  if(l_button && !r_button && pos > 0) pos--;
  if(r_button && !l_button && pos < 15) pos++;
  
  //screen handling
  lcd.clear();
  lcd.setCursor(0,0);
  for(int x = 0; x < 16; x++) {
    if(missiles[x] != -1) {
      lcd.setCursor(x,0);
      if(missiles[x] > 0) lcd.write(missiles[x]);
      missiles[x]--;                                //plus some game logic
    }
    lcd.setCursor(x,1);
    if(missiles[x] == 0) lcd.print("*");
    if(x == pos) lcd.write((uint8_t) 0);
    else lcd.print(" ");
  }
  if(recharge > 0) recharge--;
  if(l_button && r_button && recharge == 0) {
    lcd.setCursor(pos-1,1);
    lcd.print("#^#"); //SEND_MISSILE;
    Serial.write(15-pos);
    recharge = 3;
  }
  if(Serial.available()) {
    lcd.setCursor(0,0);
    command = Serial.read();
  }
}
