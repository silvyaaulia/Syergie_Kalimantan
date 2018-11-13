//test_speed_function 
//07/11/2018

#define joyl A14
#define joyr A15

int potensiol = 0;
int potensior = 0;

/* Speed Function */
double pulse_speed(int joyspeed) {
  double pulse_speed_in;
  
  if (joyspeed <=200){
    pulse_speed_in = 1500;
   }
  else if (joyspeed <=400){
    pulse_speed_in = 1600;
   }
  else if (joyspeed <=600){
    pulse_speed_in = 1700;
   }
  else if (joyspeed <=800){
    pulse_speed_in = 1800;
   }
  else{
    pulse_speed_in = 1900; 
   }
  return pulse_speed_in;
}

void setup() {
  // Serial begin
  Serial.begin(57600);
  delay(1500);
}

void loop() {
  int pulse_speed_in_left, pulse_speed_in_right;

        //speed kiri
        Serial.print("   A8: ");
        potensiol = analogRead(joyl);
        Serial.print(potensiol);
        pulse_speed_in_left = pulse_speed(potensiol);

        //speed kanan
        Serial.print("    A9: ");
        potensior = analogRead(joyr);
        Serial.println(potensior);
        pulse_speed_in_right = pulse_speed(potensior);


        Serial.print("   ");
        Serial.print(pulse_speed_in_left);
        Serial.print("      ");
        Serial.println(pulse_speed_in_right);

        delay (1000);

}
