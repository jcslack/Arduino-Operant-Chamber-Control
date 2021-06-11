
// input/output Arduino pin constants 


// trial arrays
const int rd[2] = {1000,2000};                      //reward delivery duration (ms)
const int iti[6] ={3750,4000,4250,4500,4750,5000};  //inter-trial interval time
const int stimType[6]={1,2,3,4,5,6};                //type of stimulus to be delivered (1=no stim,2=200ms,etc.) 
byte pickRD;
byte pickITI;

// event counters


// functions
int trialType(){

  
}
int rewardDeliv(){
  digitalWrite(
}
int interTrial(){
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
