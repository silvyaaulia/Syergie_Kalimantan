/* Syergie Main Control (v.10)
 * 12/12/2018
 * Silvya Aulia
========================================
Main Control:
on   : input from joystick
off  : manual input from Pixhawk

Speed Control:
> 2 unit with switch (and LED indicator)
> speed control left for propeller front left and back left
> speed control right for propeller front right and back right
> input manual from slider joystick 

Steer Control:
> Full steer control:
  main joystick for control all propeller
  tunning joystick for control selected propeller
> Left-Right steer control
  steer control based on left and right side propeller
----------------------------------------
Winch: (not active)
> winch per propeller 
> turn to left for down (minimum value)
> turn to right for up (maximum value)
----------------------------------------
Tunning:
> 4 unit with switch for each propeller
> Tunning for kp, ki, kd 
> Maximum value for plus 0.1
> Minimum value for minus 0.1
----------------------------------------
Delay with time sampling
========================================
*/

/* Library math */
#include <math.h>

/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Joystick 
#define joy_speed_left A8       //input main speed-left (front and back)
#define joy_speed_right A9      //input main speed-right (front and back)

#define joy_steer_leftx A6       
#define joy_steer_lefty A7       
#define joy_steer_rightx A0      
#define joy_steer_righty A1      

#define joy_tunning1 A5         //
#define joy_tunning2 A4         
#define joy_tunning3 A3         
#define joy_tunning4 A2        

#define joy_winch1 A10         
#define joy_winch2 A11         
#define joy_winch3 A12         
#define joy_winch4 A13

// Initialize state joystick
int speed_left = 0;             
int speed_right = 0;

int steer_leftx = 0;
int steer_lefty = 0;
int steer_rightx = 0;
int steer_righty = 0;

int tunning1 = 0;
int tunning2 = 0;
int tunning3 = 0;
int tunning4 = 0;

int winch1 = 0;
int winch2 = 0;
int winch3 = 0;
int winch4 = 0;

// Pin Switch
const int switch_manual = 5;
const int switch_kendali = 4;           // input swicth main
const int switch_steer = 8;          // input swicth main

const int switch_speed_left = 6;  
const int switch_speed_right = 28; 
const int switch_steer_left = 7;  
const int switch_steer_right = 29;  

const int switch_steer1 = 24;           //tunning steer 1
const int switch_steer2 = 25;           //tunning steer 2
const int switch_steer3 = 26;           //tunning steer 3
const int switch_steer4 = 27;           //tunning steer 4

// Initialize State Switch
int state_manual = 0;
int state_kendali = 0;           // input swicth main
int state_steer = 0;          // input swicth main

int state_steer_left = 0;              //front left
int state_steer_right = 0;             //back left
int state_speed_left = 0;              //front left
int state_speed_right = 0;             //back left

int state_steer1 = 0;                  //state steer
int state_steer2 = 0;
int state_steer3 = 0;
int state_steer4 = 0;


/* Control State */
int state_control;

/* TIme Sampling*/
int time_sampling;

//LED 
int led_steer1;                //output tunning steer
int led_steer2;
int led_steer3;
int led_steer4;

// Pin LED Indikator
const int led_manual = 13;                //output manual or pixhawk
//const int led_kendali;                  //output nn
//const int led_steer;             //output tunning

// Pixhawk ,edited 16/10/2018 , Teguh
const int pixhawk_1 = 23;
const int pixhawk_2 = 24;
const int pixhawk_3 = 25;
const int pixhawk_4 = 26;
double pulse_pixhawk_1;
double pulse_pixhawk_2;
double pulse_pixhawk_3;
double pulse_pixhawk_4;
double azimuth;




/* =============== Declare IP Address =============== */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
/*IPAddress ip(10, 48, 20, 37);        //IP for arduino
IPAddress server(10, 48, 20, 36);   //IP for raspi/pc
*/
IPAddress ip(123, 45, 0, 9);        //IP for arduino
IPAddress server(123, 45, 0, 10);   //IP for raspi/pc

EthernetClient ethClient;
PubSubClient client(ethClient);


/* =============== Declare Function =============== */

/* --------------- Call Back Function --------------- */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }

  if(strcmp(topic,"MainControl") == 0){
    client.publish("ControlBox","connected");
  }
}

/* --------------- Reconnect Function --------------- */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("MainArduino")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("MainControl");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* --------------- Speed Function --------------- */
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
  else if (joyspeed <=920){
    pulse_speed_in = 1800;
   }
  else{
    pulse_speed_in = 1900; 
   }
  return pulse_speed_in;
}

/* --------------- Steer Function --------------- */
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
      else if (y >= 500){
           pulse_steer_in = 0;
           //tengah;
      }
      else{
          pulse_steer_in = 1100;
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

/* --------------- Tunning Function (KP) --------------- */
double pulse_tunning_p(int joytune_1) {
  double tunning_1;
  if (joytune_1 <=100){
    tunning_1 = -0.1;
   }
  else if (joytune_1 >=900){
    tunning_1 = 0.1;
   }
   else{
    tunning_1 = 0;
   }
  return tunning_1;
}

/* --------------- Tunning Function (KI) --------------- */
double pulse_tunning_i(int joytune_2) {
  double tunning_2;
  if (joytune_2 <=100){
    tunning_2 = -0.0001;
   }
  else if (joytune_2 >=900){
    tunning_2 = 0.0001;
   }
   else{
    tunning_2 = 0;
   }
  return tunning_2;
}

/* --------------- Tunning Function (KD) --------------- */
double pulse_tunning_d(int joytune_3) {
  double tunning_3;
  if (joytune_3 <=100){
    tunning_3 = -0.1;
   }
  else if (joytune_3 >=900){
    tunning_3 = 0.1;
   }
  else{
    tunning_3 = 0;
   }
  return tunning_3;
}

/* --------------- Winch Function --------------- */
double pulse_winch(int joy_winch) {
  double winch;
  if (joy_winch <=200){
    winch = 1100;
   }
  else if (joy_winch >=800){
    winch = 1900;
   }
   else{
    winch = 1500;
   }
  return winch;
}


/* =============== MAIN PROGRAM (SETUP) =============== */
void setup() {
  // Serial begin
  Serial.begin(57600);

  // Switch
  pinMode(switch_manual ,INPUT);                  //switch UTAMA
  analogWrite(switch_manual ,255);
  pinMode(switch_kendali ,INPUT);
  analogWrite(switch_kendali ,255);
  pinMode(switch_steer ,INPUT);
  analogWrite(switch_steer ,255);

  pinMode(switch_speed_left, INPUT);              //switch speed
  pinMode(switch_speed_right, INPUT);
  analogWrite(switch_speed_left, 255);
  analogWrite(switch_speed_right, 255);

  pinMode(switch_steer_left, INPUT);              //switch steer
  pinMode(switch_steer_right, INPUT);
  analogWrite(switch_steer_left, 255);
  analogWrite(switch_steer_right, 255);
  
  pinMode(switch_steer1, INPUT);                  //tunning PID
  pinMode(switch_steer2, INPUT);
  pinMode(switch_steer3, INPUT);
  pinMode(switch_steer4, INPUT);
  analogWrite(switch_steer1, 255);
  analogWrite(switch_steer2, 255);
  analogWrite(switch_steer3, 255);
  analogWrite(switch_steer4, 255);

  //pinMode(led_winch, OUTPUT);
  pinMode(led_manual, OUTPUT);

  // Ethernet
  client.setServer(server, 1883);
  Ethernet.begin(mac, ip);
  client.setCallback(callback);  

  delay(1500);
}

/* =============== MAIN PROGRAM (LOOP) =============== */
void loop() {
  char msgBuffer[20];
  int pulse_speed_in_left, pulse_speed_in_right;
  int pulse_steer_in_left, pulse_steer_in_right;
  int pulse_steer_in_all, pulse_steer_in_sub;
  int pulse_winch1, pulse_winch2, pulse_winch3, pulse_winch4;
  int pulse_tunning1, pulse_tunning2;
  double kp1, kd1, kp2, kd2,kp3, kd3, kp4, kd4, ki1, ki2, ki3,ki4, xx;
  double w1, w2, w3, w4;
  
 if (!client.connected()) {
   reconnect();
  }
  client.publish("ControlBox","connected");

/*  
/*---------------Winch Control---------------*
    state_winch = digitalRead (switch_winch);
    if (state_winch == LOW){
      Serial.println(" Winch:on");
      winch1 = analogRead(joy_winch1);
      winch2 = analogRead(joy_winch2);
      winch3 = analogRead(joy_winch3);
      winch4 = analogRead(joy_winch4);
      pulse_winch1 = pulse_winch(winch1);
      pulse_winch2 = pulse_winch(winch2);
      pulse_winch3 = pulse_winch(winch3);
      pulse_winch4 = pulse_winch(winch4);
      Serial.print(" winch1: "); Serial.print(pulse_winch1);
      Serial.print(" winch2: "); Serial.print(pulse_winch2);
      Serial.print(" winch3: "); Serial.print(pulse_winch3);
      Serial.print(" winch4: "); Serial.println(pulse_winch4);    
      client.publish("spc_winch1",dtostrf(pulse_winch1, 4, 0, msgBuffer));
      client.publish("spc_winch2",dtostrf(pulse_winch2, 4, 0, msgBuffer));
      client.publish("spc_winch3",dtostrf(pulse_winch3, 4, 0, msgBuffer));
      client.publish("spc_winch4",dtostrf(pulse_winch4, 4, 0, msgBuffer));
    }
*/

/*---------------MANUAL---------------*/
 state_manual = digitalRead (switch_manual);

  //Switch Main : on
  if (state_manual == LOW) {                        
    // manual switch on, indicator on:
    digitalWrite(led_manual, HIGH);
    Serial.println();
    Serial.println("Manual Mode");
    client.publish("Mode", "manual");

    /*------Speed Control (main)------*/
    state_speed_left = digitalRead(switch_speed_left);          //edited
    state_speed_right = digitalRead(switch_speed_right);        //edited
    //Serial.print(state_speed_left);
    //Serial.print(state_speed_right); 
    //Speed Control Left
    if (state_speed_left == LOW) {
      speed_left = analogRead(joy_speed_left);
      //speed_left = analogRead(joy_tunning1);      //testing
      delay(100);
      Serial.print(" Speed left: ");
      pulse_speed_in_left = pulse_speed(speed_left);
      Serial.print(analogRead(joy_speed_left));
      client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 4, 0, msgBuffer));
    }
    //Speed Control Right
    if (state_speed_right == LOW) {
      speed_right = analogRead(joy_speed_right);
      //speed_right = analogRead(joy_tunning3);      //testing
      delay(100);
      Serial.print(" Speed right: ");
      pulse_speed_in_right = pulse_speed(speed_right);
      Serial.println(pulse_speed_in_right);
      client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 4, 0, msgBuffer));
    }


/*---------------KENDALI---------------*/
    int state_kendali = digitalRead(switch_kendali); // input swicth kendali
    if (state_kendali == LOW) {
      // Steer Control: on
      int state_steer = digitalRead(switch_steer); // input swicth kendali
      if (state_steer == LOW) {
        /*------Steer Control (Main joysick: 1 joystick for 4 propeller))------*/
        if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH) {
          //main joystick
          steer_rightx = analogRead(joy_steer_rightx);      //main joystick
          steer_righty = analogRead(joy_steer_righty);
          delay(100);
          pulse_steer_in_all = pulse_steer(steer_rightx, steer_righty);
          Serial.println(pulse_steer_in_all);
          Serial.print(" Main joystick: ");
          if (pulse_steer_in_all > 1000 ) {
            client.publish("spc_steer1",dtostrf(pulse_steer_in_all, 4, 0, msgBuffer));
            client.publish("spc_steer2",dtostrf(pulse_steer_in_all, 4, 0, msgBuffer));
            client.publish("spc_steer3",dtostrf(pulse_steer_in_all, 4, 0, msgBuffer));
            client.publish("spc_steer4",dtostrf(pulse_steer_in_all, 4, 0, msgBuffer));
          }
        }
        else{
          //tunning joystick
          steer_leftx = analogRead(joy_steer_leftx);        //tunning joystick
          steer_lefty = analogRead(joy_steer_lefty);
          delay(100);
          pulse_steer_in_sub = pulse_steer(steer_leftx, steer_lefty);
          Serial.println(pulse_steer_in_sub);
          Serial.print(" Tunning joystick: ");
          if (pulse_steer_in_all > 1000 ) {
              if (state_steer1 == LOW && state_steer2 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH) {
                client.publish("spc_steer1",dtostrf(pulse_steer_in_sub, 4, 0, msgBuffer));
              }
              if (state_steer1 == HIGH && state_steer2 == LOW && state_steer2 == HIGH && state_steer2 == HIGH) {
                client.publish("spc_steer2",dtostrf(pulse_steer_in_sub, 4, 0, msgBuffer));
              }
              if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer2 == LOW && state_steer2 == HIGH) {
                client.publish("spc_steer3",dtostrf(pulse_steer_in_sub, 4, 0, msgBuffer));
              }
              if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH && state_steer2 == LOW) {
                client.publish("spc_steer4",dtostrf(pulse_steer_in_sub, 4, 0, msgBuffer));
              }
          }
        }
      }
      else{
        /*------Steer Control (Left-Right)------*/
        state_steer_left = digitalRead(switch_steer_left);        
        state_steer_right = digitalRead(switch_steer_right);      
        //Serial.print(state_steer_left);
        //Serial.println(state_steer_right);

        //Steer Control Left
        if (state_steer_left == LOW) {
          steer_leftx = analogRead(joy_steer_leftx);
          steer_lefty = analogRead(joy_steer_lefty);
          delay(100);
          Serial.print(" Steer left: ");
          pulse_steer_in_left = pulse_steer(steer_leftx, steer_lefty);
          Serial.print(pulse_steer_in_left);
          if (pulse_steer_in_left > 1000 ) {
            client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 4, 0, msgBuffer));
          }
        }
        //Steer Control Right
        if (state_steer_right == LOW) {
          steer_rightx = analogRead(joy_steer_rightx);
          steer_righty = analogRead(joy_steer_righty);
          delay(100);
          Serial.print(" Steer right: ");
          pulse_steer_in_right = pulse_steer(steer_rightx, steer_righty);
          Serial.println(pulse_steer_in_right);
          if (pulse_steer_in_right > 1000 ) {
            client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 4, 0, msgBuffer));
          }
        }
      }//steer high     
    }     
     
    else{
      //state_kendali: HIGH (off)
      //Tunning PID: on
      Serial.print("Tunning On  ");
      tunning1 = analogRead(joy_tunning1);
      tunning2 = analogRead(joy_tunning2);
      tunning3 = analogRead(joy_tunning3);
      //tunning4 = analogRead(joy_tunning4);            
      state_steer1 = digitalRead(switch_steer1);
      state_steer2 = digitalRead(switch_steer2);
      state_steer3 = digitalRead(switch_steer3);
      state_steer4 = digitalRead(switch_steer4);
      Serial.print(state_steer1);
      Serial.print(state_steer2);
      Serial.print(state_steer3);
      Serial.println(state_steer4);

      // switch tunning 1 on
      if (state_steer1 == LOW && state_steer2 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH) {
        Serial.print(" T.Steer1: ");
        kp1 = pulse_tunning_p(tunning1);
        ki1 = pulse_tunning_i(tunning2);
        kd1 = pulse_tunning_d(tunning3);
        Serial.print(" kp:");
        Serial.print(kp1);
        Serial.print(" ki:");
        Serial.print(ki1);
        Serial.print(" kd:");
        Serial.println(kd1);
        client.publish("steer1_kp",dtostrf(kp1, 4, 2, msgBuffer));
        client.publish("steer1_ki",dtostrf(ki1, 6, 4, msgBuffer));
        client.publish("steer1_kd",dtostrf(kd1, 4, 2, msgBuffer));
      } 

      // switch tunning 2 on
      if (state_steer1 == HIGH && state_steer2 == LOW && state_steer3 == HIGH && state_steer4 == HIGH) {
        Serial.print(" T.Steer2: ");
        kp2 = pulse_tunning_p(tunning1);
        ki2 = pulse_tunning_i(tunning2);
        kd2 = pulse_tunning_d(tunning3);
        Serial.print(" kp:");
        Serial.print(kp2);
        Serial.print(" ki:");
        Serial.print(ki2);
        Serial.print(" kd:");
        Serial.println(kd2);
        client.publish("steer1_kp",dtostrf(kp2, 4, 2, msgBuffer));
        client.publish("steer1_ki",dtostrf(ki2, 5, 4, msgBuffer));
        client.publish("steer1_kd",dtostrf(kd2, 4, 2, msgBuffer));
      }

     // switch tunning 3 on
     if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer3 == LOW && state_steer4 == HIGH) {
       Serial.print(" T.Steer3: ");
       kp3 = pulse_tunning_p(tunning1);
       ki3 = pulse_tunning_i(tunning2);
       kd3 = pulse_tunning_d(tunning3);      
       Serial.print(" kp:");
       Serial.print(kp3);
       Serial.print(" ki:");
       Serial.print(ki3);
       Serial.print(" kd:");
       Serial.println(kd3);
       client.publish("steer1_kp",dtostrf(kp3, 4, 2, msgBuffer));
       client.publish("steer1_ki",dtostrf(ki3, 6, 4, msgBuffer));
       client.publish("steer1_kd",dtostrf(kd3, 4, 2, msgBuffer));
     }
 
     // switch tunning 4 on
     if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer3 == HIGH && state_steer4 == LOW) {
       Serial.print(" T.Steer4: ");
       kp4 = pulse_tunning_p(tunning1);
       ki4 = pulse_tunning_i(tunning2);
       kd4 = pulse_tunning_d(tunning3);
       Serial.print(" kp:");
       Serial.print(kp4);
       Serial.print(" ki:");
       Serial.print(ki4);
       Serial.print(" kd:");
       Serial.println(kd4);
       client.publish("steer1_kp",dtostrf(kp4, 4, 2, msgBuffer));
       client.publish("steer1_ki",dtostrf(ki4, 6, 4, msgBuffer));
       client.publish("steer1_kd",dtostrf(kd4, 4, 2, msgBuffer));
     }
     Serial.println();
   }
  }
  
  else {
    //Switch Main : off
    //Input smc dari pixhawk 
    digitalWrite(led_manual, LOW);
    Serial.println();  
    Serial.println("Pixhawk Mode");
      
    /*---------------Pixhawk---------------*/
    // Pixhawk process , Teguh, 16/10/2018
    pulse_pixhawk_1 = pulseIn(pixhawk_1, HIGH);
    pulse_pixhawk_2 = pulseIn(pixhawk_2, HIGH);
    pulse_pixhawk_3 = pulseIn(pixhawk_3, HIGH);
    pulse_pixhawk_4 = pulseIn(pixhawk_4, HIGH);
    //Yaw Left
      if ((pulse_pixhawk_4 > pulse_pixhawk_1 ) && (pulse_pixhawk_3 > pulse_pixhawk_2)){
        pulse_speed_in_right = 1800;
        pulse_speed_in_left = 1100;
        pulse_steer_in_right = 1500;
        pulse_steer_in_left = 1500;
        client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 4, 0, msgBuffer));
        client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 4, 0, msgBuffer));
        client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 4, 0, msgBuffer));
        client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 4, 0, msgBuffer));
      }
      //Yaw Right
      else if ((pulse_pixhawk_4 > pulse_pixhawk_1 ) && (pulse_pixhawk_3 > pulse_pixhawk_2)){
         pulse_speed_in_right = 1100;
         pulse_speed_in_left = 1800;
         pulse_steer_in_right = 1500;
         pulse_steer_in_left = 1500;
         client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 4, 0, msgBuffer));
         client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 4, 0, msgBuffer));
         client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 4, 0, msgBuffer));
         client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 4, 0, msgBuffer));
       } 
      //Another
      else {
         if ((pulse_pixhawk_4 == pulse_pixhawk_3)&&(pulse_pixhawk_3 == pulse_pixhawk_2) && (pulse_pixhawk_2 == pulse_pixhawk_1)){
            pulse_speed_in_right = 1500;
            pulse_speed_in_left = 1500;
            pulse_steer_in_right = 1500;
            pulse_steer_in_left = 1500;
            client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 4, 0, msgBuffer));
            client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 4, 0, msgBuffer));
            client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 4, 0, msgBuffer));
            client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 4, 0, msgBuffer));
          }  
       else{ 
          pulse_speed_in_right = 1700;
          pulse_speed_in_left = 1700;
          azimuth = atan2 (pulse_pixhawk_4 - pulse_pixhawk_1, pulse_pixhawk_2 - pulse_pixhawk_3) ;
          if ((azimuth >= 0) && (azimuth <= 3.14)){
            pulse_steer_in_right = map(azimuth, 0, 3.14, 1100, 1500);
            pulse_steer_in_left = pulse_steer_in_right;
          }
          else if ((azimuth > 3.14) && (azimuth <= 6.28)){
            pulse_steer_in_right = map(azimuth, 3.15, 6.28, 1500, 1900);
            pulse_steer_in_left = pulse_steer_in_right;
            client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 4, 0, msgBuffer));
            client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 4, 0, msgBuffer));
            client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 4, 0, msgBuffer));
          }
       }
     }
    //Serial.println(pulse_pixhawk);
    }
   
  client.loop();

  /*------Time Sampling)------*/
  if (state_control == 0){   ///manual
  time_sampling = 500;
  }
  if (state_control == 1){   //pixhawk
  time_sampling = 1000;
  }
  delay(time_sampling); 

  //delay(1000);
}
