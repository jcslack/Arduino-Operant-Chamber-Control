
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
int TB = 6;  // house light on, trial begins (TB)
int SB = 5;  // stim begins (SB)
int NRT = 4; // no response trial (NRT)
int RD = 14;  // reward delivered (RD)
int FT = 15;  // forced trial (FT)
int LPT = 16; // left port trial (LPT)
int RPT = 17; // right port trial (RPT)
int LP = 18;  // left port press
int RP = 19;  // right port press

// trial arrays and states
int rd[2] = {1000,2000};                      //reward delivery duration (ms)
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



//-------------------------------------INITIALIZATION---------------------------------------------------------
// setup loop
void setup() {
  // pin mode declaraction
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
  
  // start IR sensors
  //digitalWrite(senL,HIGH);
  //digitalWrite(senR,HIGH);
  randomize(iti,nITI);
  randomize(trialType,nT);
  randomize(rd,nRD);
}

//---------------------------------------------------MAIN LOOP--------------------------------------------------
// main loop
void loop() {
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
  if (E == maxError){
    ++F;
    digitalWrite(FT,HIGH);
    digitalWrite(SB,LOW);
  //S7
    digitalWrite(senL,HIGH);
    digitalWrite(senR,HIGH);
    while(digitalRead(senL) == HIGH && digitalRead(senR) == HIGH){  //wait for beam break
    }
    senStateL=digitalRead(senL);
    senStateR=digitalRead(senR);
    ++N;
    E=0;
    digitalWrite(FT,LOW);
    digitalWrite(RD,HIGH);
    if (senStateL == LOW){
      digitalWrite(LP,HIGH);
      digitalWrite(solL,HIGH);
      delay(rd[entryRD]);
    }
    else{
      digitalWrite(RP,HIGH);
      digitalWrite(solR,HIGH);
      delay(rd[entryRD]);
    }
    delay(6000);
   
  }
  //--------------------------------------UNFORCED TRIAL----------------------------------
  //S11
  else{ 
    digitalWrite(SB,LOW);
    startWait=millis();
    while(digitalRead(senL) == HIGH && digitalRead(senR) == HIGH){
      if (millis()-startWait >=10000){
        unresponsive=1;
        break;
      }
    }
    senStateL=digitalRead(senL);
    senStateR=digitalRead(senR);
    if (unresponsive == 1){
      digitalWrite(NRT,HIGH);
      ++E;
      ++U;
      ++N;
      delay(rd[entryRD]);
    }
    else if(trialType[entryT]==1 && senStateL == LOW){
      digitalWrite(LP,HIGH);
      ++C;
      E=0;
      digitalWrite(solL,HIGH);
    }
    else if (trialType[entryT]==1 && senStateR == LOW){
      digitalWrite(RP,HIGH);
      ++E;
    }
  }
 
 
}
