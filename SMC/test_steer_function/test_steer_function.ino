//Test_steer_function
//18/10/2018

#define joy0 A0         //steer kanan x
#define joy1 A1         //steer kanan y
#define joy2 A2         
#define joy3 A3         
#define joy4 A4         
#define joy5 A5
#define joy6 A6         //steer kiri x
#define joy7 A7         //steer kiri y

int potensio0 = 0;
int potensio1 = 0;
int potensio2 = 0;
int potensio3 = 0;
int potensio4 = 0;
int potensio5 = 0;
int potensio6 = 0;
int potensio7 = 0;

double pulse_steer_in;


// Steer Function
double pulse_steer(int x, int y) {
  double pulse_steer_in;
  if (x <= 400){
      if (y <= 350){
           pulse_steer_in = 1200;
           //derajat = 225; 
      }
      else if (y <= 800){
           pulse_steer_in = 1300;
          //derajat = 270;
      }
      else if (y > 800){
           pulse_steer_in = 1400;
          //derajat = 315;
      }
      else{
          pulse_steer_in = 111;
      }
  }
  else if (x <= 800){
      if (y > 1000){
           pulse_steer_in = 1500;
           //derajat = 0; 
      }
      else{
          pulse_steer_in = 0;
      }
  }
  else  if (x > 800){
      if (y <= 350){
           pulse_steer_in = 1800;
          //derajat = 45;
      }
      else if (y <= 800){
           pulse_steer_in = 1700;
          //derajat = 90;
      }
      else if (y > 800){
           pulse_steer_in = 1600;
           //derajat = 135; 
      }  
      else{
          pulse_steer_in = 77;
      }
  }
  else {
       pulse_steer_in = 99;
  }
  return pulse_steer_in; 
}

void setup() {
  // Serial begin
  Serial.begin(57600);
  delay(1500);
}

void loop() {
  double potensio0, potensio1, potensio2, potensio3, potensio4, potensio5, potensio6, potensio7;
  int pulse_steer_in_left, pulse_steer_in_right;
  
        Serial.print("   A0: ");
        potensio0 = analogRead(joy0);
        Serial.print(potensio0);
        //delay (100);
        Serial.print("   A1: ");
        potensio1 = analogRead(joy1);
        Serial.print(potensio1);
        //delay (100);
        pulse_steer_in_right = pulse_steer(potensio0, potensio1);
                 
/*
        Serial.print("      A2: ");
        potensio2 = analogRead(joy2);
        Serial.print(potensio2);
        //delay (100);
        Serial.print("   A3: ");
        potensio3 = analogRead(joy3);
        Serial.println(potensio3);
       // delay (100);  

        Serial.print("   A4: ");
        potensio4 = analogRead(joy4);
        Serial.print(potensio4);
        //delay (500);
        Serial.print("   A5: ");
        potensio5 = analogRead(joy5);
        Serial.print(potensio5);
        //delay (500);
*/

        Serial.print("   A6: ");
        potensio6 = analogRead(joy6);
        Serial.print(potensio6);
        //delay (500);
        Serial.print("   A7: ");
        potensio7 = analogRead(joy7);
        Serial.println(potensio7);
        //delay (500);
        pulse_steer_in_left = pulse_steer(potensio6, potensio7);


      Serial.print("   ");
      Serial.print(pulse_steer_in_right);
      Serial.print("              ");
      Serial.println(pulse_steer_in_left);

  
        delay(1000);
}
