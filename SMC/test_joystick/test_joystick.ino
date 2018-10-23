//Test_Joystick


#define joy0 A0         
#define joy1 A1         
#define joy2 A2         
#define joy3 A3         
#define joy4 A4         
#define joy5 A5
#define joy6 A6         
#define joy7 A7

int potensio0 = 0;
int potensio1 = 0;
int potensio2 = 0;
int potensio3 = 0;
int potensio4 = 0;
int potensio5 = 0;
int potensio6 = 0;
int potensio7 = 0;


void setup() {
  // Serial begin
  Serial.begin(57600);
  delay(1500);
}

void loop() {
  double potensio0, potensio1, potensio2, potensio3, potensio4, potensio5, potensio6, potensio7;
   

        Serial.print("   A0: ");
        potensio0 = analogRead(joy0);
        Serial.print(potensio0);
        //delay (500);
        Serial.print("   A1: ");
        potensio1 = analogRead(joy1);
        Serial.print(potensio1);
        //delay (500);

        Serial.print("   A2: ");
        potensio2 = analogRead(joy2);
        Serial.print(potensio2);
        //delay (500);
        Serial.print("   A3: ");
        potensio3 = analogRead(joy3);
        Serial.print(potensio3);
        //delay (500);

        Serial.print("   A4: ");
        potensio4 = analogRead(joy4);
        Serial.print(potensio4);
        //delay (500);
        Serial.print("   A5: ");
        potensio5 = analogRead(joy5);
        Serial.print(potensio5);
        //delay (500);

        Serial.print("   A6: ");
        potensio6 = analogRead(joy6);
        Serial.print(potensio6);
   //     delay (500);
        Serial.print("   A7: ");
        potensio7 = analogRead(joy7);
        Serial.println(potensio7);
 //       delay (500);


        delay(1000);
}
