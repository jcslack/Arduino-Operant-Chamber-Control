
// input/output Arduino pin constants 
const int solL = 12;  // left solenoid
const int solR;       // right solenoid
const int senL = 8;   // left IR sensor
const int senR;       // right IR sensor
const int tone1 = 4;  // 3.5kHz tone
const int tone2;      // 1.8kHz tone
const int light;      // house light
int senState1 = 0;    // 
int senState2 = 0;

// outputs to plexon (ripple)
int TB;  // house light on, trial begins (TB)
int SB;  // stim begins (SB)
int NRT; // no response trial (NRT)
int RD;  // reward delivered (RD)
int FT;  // forced trial (FT)
int LPT; // left port trial (LPT)
int RPT; // right port trial (RPT)
int LP;  // left port press
int RP;  // right port press

// trial arrays and states
const int rd[2] = {1000,2000};                      //reward delivery duration (ms)
const int iti[6] ={3750,4000,4250,4500,4750,5000};  //inter-trial interval time
const int stimType[6]={1,2,3,4,5,6};                //type of stimulus to be delivered (1=no stim,2=200ms,etc.)
int maxError = 2;  // number of sequential incorrect trials before forced trial
int selectedTrial;
byte pickRD;
byte pickITI;
int senStateL = 0; // initialize left sensor state 
int senStateR = 0; // initialize right sensor state


// event counters
int T = 0;   // elapsed time of experiment (min)
int N = 0;   // number of completed trials (rewards received)
int C = 0;   // number of correct trials
int F = 0;   // number of forced trials
int U = 0;   // number of unresponsive trials
float P = 0; // percent of correct trials
int V = 0;   // reward value


// functions
int chooseStim(){
  selectedTrial=stimType[randi(5)];
  return selectedTrial;
}
int rewardDeliv(selectedTrial){
  // no stimulation, left door will deliver reward 
  if (selectedTrial == 1){  
    digitalWrite(
  }
  
}
int interTrial(){
  delay(iti[random(5)]);
}

// setup loop
void setup() {
  // pin mode declaraction
  pinMode(solL,OUTPUT);
  pinMode(solR,OUTPUT);
  pinMode(tone1,OUTPUT);
  pinMode(tone2,OUTPUT);
  pinMode(houseLight,OUTPUT);
  pinMode(senL,INPUT);
  pinMode(senR,INPUT);
  
  // start IR sensors
  digitalWrite(senL,HIGH);
  digitalWrite(senR,HIGH);
}

// main loop
void loop() {
  pickRD = random(2);
  senState1 = digitalRead(senL);
  if (senState1 == LOW){
    digitalWrite(tone1,HIGH);
    delay(100);
    digitalWrite(tone1,LOW);
    digitalWrite(solL,HIGH);
    delay(rd[pickRD]);
    digitalWrite(solL,LOW);
  }
  else{
    digitalWrite(solL,LOW);
  }
 
}
