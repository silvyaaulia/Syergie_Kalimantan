//test_speed_function 
//07/11/2018

#define joy3 A3
#define joy5 A5

int potensio3 = 0;
int potensio5 = 0;

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
        Serial.print("   A5: ");
        potensio5 = analogRead(joy5);
        Serial.print(potensio5);
        pulse_speed_in_left = pulse_speed(potensio5);

        //speed kanan
        Serial.print("    A3: ");
        potensio3 = analogRead(joy3);
        Serial.println(potensio3);
        pulse_speed_in_right = pulse_speed(potensio3);


        Serial.print("   ");
        Serial.print(pulse_speed_in_left);
        Serial.print("      ");
        Serial.println(pulse_speed_in_right);


        delay (1000);

}
