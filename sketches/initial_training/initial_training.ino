#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

const int solL = 7;
const int solR = 6;
const int senL = 3;
const int senR = 2;
const int toneS = 5;
const int toneL = 4;
const int light = 4;
int senStateL = 0;
int senStateR = 0;
unsigned long startWait;
int unresponsive = 0;

//-----------------functions-----------------------------
void setPINS(){
  pinMode(solL,OUTPUT);
  pinMode(solR,OUTPUT);
  pinMode(senL,INPUT_PULLUP);
  pinMode(senR,INPUT_PULLUP);
  pinMode(toneS,OUTPUT);
  pinMode(toneL,OUTPUT);
  digitalWrite(solL,HIGH);
  digitalWrite(solR,HIGH);
  digitalWrite(toneS,HIGH);
  digitalWrite(light,HIGH);
}

void setLCD(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Session Start");
 
}

//void deliverReward(int pinNum, int duration){
  

  
//}

//------------------------------main------------------------------
void setup() {
  setLCD();
  setPINS();
 
}

void loop() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Begin Trial");
  digitalWrite(light,LOW);
  delay(1000);
  digitalWrite(light,HIGH);
  lcd.clear();
  startWait=millis();
  lcd.setCursor(2,0);
  lcd.print("Waiting for Nose Poke");
  while(digitalRead(senL) == HIGH && digitalRead(senR) == HIGH){
    
    if(millis()-startWait >= 20000){
      unresponsive = 1;
      break;
    }
  }
  lcd.clear();
  senStateL = digitalRead(senL);
  senStateR = digitalRead(senR);
  if(unresponsive == 1){
    lcd.setCursor(2,0);
    lcd.print("No Response");
    digitalWrite(toneS,LOW);
    delay(250);
    digitalWrite(toneS,HIGH);
    unresponsive = 0;
  }
  else if(senStateL == LOW){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Left Port Poke");
    digitalWrite(toneS,LOW);
    delay(100);
    digitalWrite(toneS,HIGH);
    digitalWrite(solL,LOW);
    delay(50);
    digitalWrite(solL,HIGH);
  }
  else if(senStateR == LOW){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Right Port Poke");
    digitalWrite(toneS,LOW);
    delay(100);
    digitalWrite(toneS,HIGH);
    digitalWrite(solR,LOW);
    delay(50);
    digitalWrite(solR,HIGH);
  }
    
    delay(2000);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Next Trial");
    delay(2000);
}
