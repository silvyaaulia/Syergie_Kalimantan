//Test winch
//01/11/18

#define potensio1 A10         
#define potensio2 A11        
#define potensio3 A12         
#define potensio4 A13            

int winch1 = 0;
int winch2 = 0;
int winch3 = 0;
int winch4 = 0;
      

/* Winch Function */
double pulse_winch(int potensio) {
  double winch;
  if (potensio <=150){
    winch = 1100;
   }
  else if (potensio >=850){
    winch = 1900;
   }
   else{
    winch = 0;
   }
  return winch;
}


void setup() {
  // Serial begin
  Serial.begin(57600);
    delay(1500);
}

void loop() {      
  double w1, w2, w3, w4;

// switch tunning 1 on
//      if (state_steer1 == LOW && state_steer2 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH) {
        winch1 = analogRead(potensio1);
        winch2 = analogRead(potensio2);
        winch3 = analogRead(potensio3);
        winch4 = analogRead(potensio4);

        w1 = pulse_winch (winch1);
        w2 = pulse_winch (winch2);
        w3 = pulse_winch (winch3);
        w4 = pulse_winch (winch4);
    
        Serial.print(" winch1: ");
        Serial.print(w1);
        Serial.print(" winch2: ");
        Serial.print(w2);
        Serial.print(" winch3: ");
        Serial.print(w3);
        Serial.print(" winch4: ");
        Serial.println(w4);
        
       delay(1000);
 } 
