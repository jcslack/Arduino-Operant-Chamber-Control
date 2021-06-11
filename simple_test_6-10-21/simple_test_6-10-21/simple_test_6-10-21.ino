
// input/output pin variables 
int solL = 12;
int solR;
int senL = 8;
int senR;
int tone1 = 4;
int tone2;
int senState1 = 0;
int senState2 = 0;
int light;

// trial arrays
int rd[2] = {1000,2000};                      //reward delivery duration (ms)
int iti[6] ={3750,4000,4250,4500,4750,5000};  //inter-trial interval time
byte pickRD;
byte pickITI;

// event counters


// functions
void trialType(){

  
}
void interTrial(){
  delay(iti[random(5)]);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(solL,OUTPUT);
  pinMode(tone1,OUTPUT);
  pinMode(senL,INPUT);
  digitalWrite(senL,HIGH);
  
}
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
