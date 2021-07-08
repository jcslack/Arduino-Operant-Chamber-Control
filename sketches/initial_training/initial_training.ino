#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

const int solL = 7; //left solenoid (output)
const int solR = 6; //right solenoid (output)
const int senL = 3; //left sensor (input)
const int senR = 2; //right sensor (input)
const int toneS = 5; //3.5kHz tone (output)
const int toneL = 4;
const int light = 4; //houselight (output)
int senStateL = 0;  //initialize sensor states (high or low)
int senStateR = 0;
unsigned long startSession; //initialize zero point for session
unsigned long startWait;  //initialize zero point for trial
const unsigned long runTime = 3600000; //length of session (msec)
int unresponsive = 0; //initialize check for non-response trial
int U = 0; //number of non-responses
int R = 0; //number of right port responses
int L = 0; //number of left port responses
int T = 0; //total responses (L+R)
int N = 0; //total trials (L+R+U)

//-----------------functions-----------------------------
//intializes pin modes and states
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

//initializes LCD display
void setLCD(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Session Start");
 
}

//when called, displays trial counters on LCD screen given N (numTrials), 
//T (numResponses), and U (numNonresponses) inputs
void dispTrialCounters(int numTrials,int numResponses, int numNonresponses,unsigned long t0){
  lcd.clear();
  lcd.setCursor(0,0);
  unsigned long msec = millis()-t0;
  unsigned long sec = msec/1000;
  unsigned long mins = sec/60;

  lcd.print("Elapsed Time: ");
  lcd.setCursor(14,0);
  if(sec < 60){
    lcd.print(sec);
    lcd.setCursor(17,0);
    lcd.print("sec");
  }
  else{
    lcd.print(mins);
    lcd.setCursor(17,0);
    lcd.print("min");
  }
  //lcd.print("---Trial Counters---");
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
  delay(3000);
}

//when called (i.e. when session time reaches runTime), 
//initiates empty endless while loop  
void endSession(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Session Time Reached");
  while(1){
  }
}

//displays elapsed time on LCD since startSession was set
/*void dispElapsedTime(unsigned long t0){
  lcd.setCursor(0,1);
  lcd.print("Elapsed Time: ");
  lcd.setCursor(14,1);
  lcd.autoscroll();
  float t;
  t = (millis()-t0)/1000;
  lcd.print(t);
  lcd.print(" sec");
  lcd.noAutoscroll();
  
}*/

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
  lcd.print("Waiting for Response");
  while(digitalRead(senL) == HIGH && digitalRead(senR) == HIGH){
    if(millis()-startWait >= 5000){
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
    delay(400);
    digitalWrite(toneS,HIGH);
    unresponsive = 0;
    U++;
  }
  else if(senStateL == LOW){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Right Port Poke");
    digitalWrite(solL,LOW);
    delay(10);
    digitalWrite(solL,HIGH);
    digitalWrite(toneS,LOW);
    delay(100);
    digitalWrite(toneS,HIGH);
    L++;
  }
  else if(senStateR == LOW){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Left Port Poke");
    digitalWrite(solR,LOW);
    delay(15);
    digitalWrite(solR,HIGH);
    digitalWrite(toneS,LOW);
    delay(100);
    digitalWrite(toneS,HIGH);
    R++;
  }
  T=L+R;
  N=L+R+U;
  delay(1000);
  digitalWrite(light,HIGH);
  dispTrialCounters(N,T,U,startSession);
  //lcd.clear();
  lcd.setCursor(0,0);
  //dispElapsedTime(startSession);
  //delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-----Next Trial-----");
  delay(1000);
}
