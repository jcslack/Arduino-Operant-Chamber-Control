#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

// input/output Arduino pin constants 
const int solL = 13;  // left solenoid
const int solR = 12;       // right solenoid
const int senL = 9;   // left IR sensor
const int senR = 8;       // right IR sensor
const int tone1 = 11;  // 3.5kHz tone
const int tone2 = 10;      // 1.8kHz tone
const int light = 7;      // house light
int senStateL = 0;    // 
int senStateR = 0;

// outputs to plexon (ripple)
const int TB = 6;  // house light on, trial begins (TB)
const int SB = 5;  // stim begins (SB)
const int NRT = 4; // no response trial (NRT)
const int RD = 14;  // reward delivered (RD)
const int FT = 15;  // forced trial (FT)
const int LPT = 16; // left port trial (LPT)
const int RPT = 17; // right port trial (RPT)
const int LP = 18;  // left port press
const int RP = 19;  // right port press

// trial arrays and states
int rd[2] = {30,50};                      //reward delivery duration (ms)
const int nRD = sizeof(rd)/sizeof(rd[0]);
int iti[6] ={3750,4000,4250,4500,4750,5000};  //inter-trial interval time
int nITI = sizeof(iti)/sizeof(iti[0]);
int trialType[4]={1,1,2,2};                          //type of stimulus to be delivered (1=no stim,2=200ms,etc.)
const int nT = sizeof(trialType)/sizeof(trialType[0]); //length of stimType (for randomizing w/o replacement)
int maxError = 2;  // number of sequential incorrect trials before forced trial
int selectedTrial;
byte pickRD;
byte pickITI;
int entryRD = -1;
int entryITI = -1;
int entryT = -1;
unsigned long startWait;
unsigned long T0;
const unsigned long run_time = 5400000;
int unresponsive = 0;


// event counters
int T = 0;   // elapsed time of experiment (min)
int N = 0;   // number of completed trials (rewards received)
int C = 0;   // number of correct trials
int F = 0;   // number of forced trials
int U = 0;   // number of unresponsive trials
float P = 0; // percent of correct trials
int V = 0;   // reward value
int E = 0;   // number of errors since last trial (consecutive errors)


// functions
void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n ){
    // Use a different seed value so that we don't get same
    // result each time we run this program
    randomSeed(analogRead(A0));
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--){
        long j = random(0, n);
        swap(&arr[i], &arr[j]);
    }   
}

// set up arduino pins
void setPins(){
  // pin mode declaration
  pinMode(solL,OUTPUT);
  pinMode(solR,OUTPUT);
  pinMode(tone1,OUTPUT);
  pinMode(tone2,OUTPUT);
  pinMode(light,OUTPUT);
  pinMode(senL,INPUT);
  pinMode(senR,INPUT);

  // event outputs to plexon (ripple)
  pinMode(TB,OUTPUT);
  pinMode(SB,OUTPUT);
  pinMode(NRT,OUTPUT);
  pinMode(RD,OUTPUT);
  pinMode(FT,OUTPUT);
  pinMode(LPT,OUTPUT);
  pinMode(RPT,OUTPUT);
  pinMode(LP,OUTPUT);
  pinMode(RP,OUTPUT);

  //pull up resistors for sensor pins
  pinMode(senL,INPUT_PULLUP);
  pinMode(senR,INPUT_PULLUP);
}

void shortTone(){
  digitalWrite(tone1,HIGH);
  delay(150);
  digitalWrite(tone1,LOW);
}

void longTone(){
  digitalWrite(tone2,HIGH);
  delay(1000);
  digitalWrite(tone2,LOW);
}

//-------------------------------------INITIALIZATION---------------------------------------------------------
// setup loop
void setup() {
  
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Beginning Session");
  Serial.begin(9600);
  Serial.print("A ");
  Serial.print("F ");
  Serial.print("B");
  setPins();
 
  randomize(iti,nITI);
  randomize(trialType,nT);
  randomize(rd,nRD);
  T0=millis();
}

//---------------------------------------------------MAIN LOOP--------------------------------------------------
// main loop
void loop() {
  if(millis()-T0 > run_time){
    Serial.println("End of Run");
    lcd.setCursor(2,0);
    lcd.print("Run Time Reached")
    lcd.setCursor(2,1);
    lcd,print("---End of Session---");
    while(1){
    }
  }
  //S2  set randomization of inter-trial interval
  if (E==0){
    delay(50);
    pickITI = random(0,nITI-1); 
    ++entryT;
    if (entryT > nT-1){ // resests random selection without replacement if all values chosen
      entryT = 0;
      randomize(trialType,nT);
    }
  }

  //S3 trial is selected and sent to plexon (ripple)
  delay(iti[pickITI]);
  Serial.print(trialType[entryT]); //print trial type (1 or 2) to serial monitor
  Serial.print(" ");
  ++entryRD;
  ++entryITI;
  if (entryRD > nRD-1){ //resets random selection without replacement if all values chosen
    entryRD = 0;
    randomize(rd,nRD);
  }
  if (entryITI > nITI-1){
    entryITI = 0;
    randomize(iti,nITI);
  }
  if (trialType[entryT]==1){ 
    digitalWrite(LPT,HIGH);  
  }
  else{
    digitalWrite(RPT,HIGH);
  }
  
  //S4 trial begins
  delay(1000);
  digitalWrite(light,HIGH);
  digitalWrite(LPT,LOW);
  digitalWrite(RPT,LOW);
  digitalWrite(TB,HIGH);

  //S5 stimulation begins
  delay(1000);
  digitalWrite(SB,HIGH);
  digitalWrite(TB,LOW);
  digitalWrite(light,LOW);

  //-------------------------------------FORCED TRIAL-----------------------------------------
  //S6
  delay(2000);
  digitalWrite(SB,LOW);
  if (E == maxError){
    ++F;
    digitalWrite(FT,HIGH);
    Serial.print(1);       //print forced (1) or unforced (0) to serial monitor
    Serial.print(" ");
  //S7
    //digitalWrite(senL,HIGH);
    //digitalWrite(senR,HIGH);
    while(digitalRead(senL) == HIGH && digitalRead(senR) == HIGH){  //wait for beam break
    }
    senStateL=digitalRead(senL);
    senStateR=digitalRead(senR);
    ++N;
    E=0;
    digitalWrite(FT,LOW);
    digitalWrite(RD,HIGH);
    if (senStateL == LOW){
      Serial.println(1);    //print left port press (1) to serial monitor
      digitalWrite(LP,HIGH);
      digitalWrite(solL,HIGH);
      delay(rd[entryRD]);
    }
    else if (senStateR == LOW){
      Serial.println(2);         //print right port press (2) to serial monitor
      digitalWrite(RP,HIGH);
      digitalWrite(solR,HIGH);
      delay(rd[entryRD]);
    }
    delay(6000);
   
  }
  //--------------------------------------UNFORCED TRIAL----------------------------------
  //S11 & S12
  else{ 
    Serial.print(0);  //print unforced (0) to serial monitor
    Serial.print(" ");
    startWait=millis();
    while(digitalRead(senL) == HIGH && digitalRead(senR) == HIGH){
      if (millis()-startWait >= 10000){
        unresponsive=1;
        break;
      }
    }
    senStateL=digitalRead(senL);
    senStateR=digitalRead(senR);
    
    //Unresponsive
    if (unresponsive == 1){
      Serial.println(5);    //print unresponsive (5) to serial monitor
      digitalWrite(NRT,HIGH);
      ++E;
      ++U;
      ++N;
      longTone();
      delay(rd[entryRD]);
      digitalWrite(NRT,LOW);
    }
    //trial type 1 is correct
    else if (trialType[entryT] == 1){ 
      //correct port chosen
      if (senStateL == LOW){
        Serial.println(1);
        digitalWrite(LP,HIGH);
        ++C;
        E=0;
        ++N;
        shortTone();
        digitalWrite(solL,HIGH);
        digitalWrite(RD,HIGH);
        delay(rd[entryRD]);
        digitalWrite(solL,LOW);
        digitalWrite(RD,LOW);
        digitalWrite(LP,LOW);
      }
      //incorrect port chosen
      else if(senStateR == LOW){
        Serial.println(2);
        digitalWrite(RP,HIGH);
        ++E; 
        ++N;
        longTone();
        delay(rd[entryRD]);
        digitalWrite(RP,LOW);
      }
    }
    // trial type 2 is correct
    else if (trialType[entryT] == 2){
      //incorrect port chosen
      if (senStateL == LOW){
        Serial.println(1);
        digitalWrite(LP,HIGH);
        ++E;
        ++N;
        longTone();
        delay(rd[entryRD]);
        digitalWrite(LP,LOW);
      }
      //correct port chosen
      else if (senStateR == LOW){
        Serial.println(2);
        digitalWrite(RP,HIGH);
        ++C;
        E=0;
        ++N;
        shortTone();
        digitalWrite(solR,HIGH);
        digitalWrite(RD,HIGH);
        delay(rd[entryRD]);
        digitalWrite(solR,LOW);
        digitalWrite(RD,LOW);
        digitalWrite(RP,LOW);
      }
    }
  }
  delay(1);
  if(N == U+F){
    P=0;
  }
  else{
    P=C/(N-U-F);
  }
  delay(1);
  /* display stuff here
   *  
   *  
   *  
 */
  delay(1);
}
