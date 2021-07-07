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
unsigned long startSession;
unsigned long startWait;
const unsigned long runTime = 3600000;
int unresponsive = 0;
int U = 0; //number of non-responses
int R = 0; //number of right port responses
int L = 0; //number of left port responses
int T = 0; //total responses (L+R)
int N = 0; //total trials (L+R+U)

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
  delay(2500);
}

void setLCD(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Session Start");
 
}

void dispTrialCounters(int numTrials,int numResponses, int numNonresponses){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("---Trial Counters---"
  lcd.setCursor(0,1);
  lcd.print("Non-Responses: ");
  lcd.setCursor(15,1);
  lcd.print(numNonresponses);
  lcd.setCursor(0,2);
  lcd.print("Responses: ");
  lcd.setCursor(15,2);
  lcd.print(numResponses);
  lcd.setCursor(0,3);
  lcd.print("Total Trials: ");
  lcd.setCursor(15,3);
  lcd.print(numTrials);
  delay(6000);
}

void endSession(){


  
}
//------------------------------MAIN------------------------------
void setup() {
  Serial.begin(9600);
  setLCD();
  setPINS();
  startSession = millis();
}

void loop() {
  if(millis()-startSession > runTime){
    endSession();
  }

//-------------------------TRIAL BEGINS---------------------
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Begin Trial");
  digitalWrite(light,LOW);
  delay(1000);
  lcd.clear();

//------------------------WAIT FOR SENSOR INPUT--------------
  startWait=millis();
  lcd.setCursor(0,0);
  lcd.print("Wait for Response");
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
    delay(10);
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
    delay(10);
    digitalWrite(solR,HIGH);
  }
    
    delay(2000);
    digitalWrite(light,HIGH);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Next Trial");
    delay(2000);
}
