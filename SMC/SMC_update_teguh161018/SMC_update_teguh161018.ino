/* Draft Syergie Main Control 
----------------------------------------
NN:
on   : publish from laptop
off  : publish from main control
----------------------------------------
Main Control:
on   : input from joystick
off  : manual input from Pixhawk
----------------------------------------
Speed Control:
> 2 unit with switch (and LED indicator)
> speed control left for propeller front left and back left
> speed control right for propeller front right and back right
> input manual from slider joystick 
----------------------------------------
Steer Control:
> 2 unit with switch (and LED indicator)
> steer control left for propeller front left and back left
> steer control right for propeller front right and back right
> input manual from slider joystick 
----------------------------------------
Tunning:
> 4 unit with switch for each propeller
> Tunning for kp, ki, kd 
> Maximum value for plus 0.1
> Minimum value for minus 0.1
----------------------------------------
*/

/* Library math */
#include <math.h>

/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Joystick 
#define joy_speed_left A0       //input main speed-left (front and back)
#define joy_speed_right A1      //input main speed-right (front and back)

#define joy_steer_left A2       //input main steer-left (front and back)
#define joy_steer_right A3      //input main steer-right (front and back)

#define joy_tunning1 A4         //tunning kp
#define joy_tunning2 A5         //tunning ki
#define joy_tunning3 A6         //tunning kd

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

// Initialize state joystick
int speed_left = 0;             
int speed_right = 0;

int steer_left = 0;
int steer_right = 0;

int tunning1 = 0;
int tunning2 = 0;
int tunning3 = 0;

// Pin Switch Back Up (input)
const int switch_nn = 40;
const int switch_manual = 22;  // input swicth main
const int switch_tunning = 23; // input swicth main

const int switch_steer_left = 26;  //front left
const int switch_steer_right = 27;  //back left

const int switch_speed_left = 26;  //front left
const int switch_speed_right = 27;  //back left

const int switch_steer1 = 30;  //front left
const int switch_steer2 = 31;  //back left
const int switch_steer3 = 32;  //front right
const int switch_steer4 = 33;  //back right

// Initialize State Switch
int state_nn = 0;             //state nn
int state_manual = 0;         //state manual
int state_tunning = 0;        //state tunning

int state_steer_left = 0;  //front left
int state_steer_right = 0;  //back left

int state_speed_left = 0;  //front left
int state_speed_right = 0;  //back left

int state_steer1 = 0;         //state steer
int state_steer2 = 0;
int state_steer3 = 0;
int state_steer4 = 0;

// LED Indikator
//int led_nn;                  //output nn
//int led_main;                //output manual or pixhawk
//int led_tunning;             //output tunning

int led_steer1;                //output tunning steer
int led_steer2;
int led_steer3;
int led_steer4;

// Pin LED Indikator
const int led_nn;                  //output nn
const int led_main;                //output manual or pixhawk
const int led_tunning;             //output tunning

//Pulse
double pulse_speed_in;
double pulse_steer_in;
double nn_speed_in;

// Input pulse for testing
const int a = 100;
const int b = 300;
const int c = 500;
const int d = 650;
const int e = 865;


/* Declare IP Address */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(123, 45, 0, 9);        //IP for arduino
IPAddress server(123, 45, 0, 10);   //IP for raspi/pc

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
}

EthernetClient ethClient;
PubSubClient client(ethClient);

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

// Speed Function
double pulse_speed(int joyspeed) {
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

//Steer Function
double pulse_steer (int joysteer) {
  const int normal = 500; 
  if (joysteer <= normal){
    pulse_steer_in = map(joysteer, 0, 500, 1100, 1500);
    }
  else {
    pulse_steer_in = map(joysteer, 501, 1023, 1501, 1900);
  }
  return pulse_steer_in;
}

//Tunning Function
double pulse_tunning(int joytune, double tunning) {
  if (joytune <=200){
    tunning += 0.1;
   }
  else if (joytune >=800){
    tunning -= 0.1;
   }
  return tunning;
}

void setup() {
  // Serial begin
  Serial.begin(57600);

  // Switch
  pinMode(switch_nn, INPUT);       //input switch nn
  digitalWrite(switch_nn, HIGH);
  
  pinMode(switch_manual, INPUT);       //input switch manual
  digitalWrite(switch_manual, HIGH);

  pinMode(switch_steer_left, INPUT);
  digitalWrite(switch_steer_left, HIGH);
  
  pinMode(switch_speed_right, INPUT);
  digitalWrite(switch_speed_right, HIGH);

  pinMode(switch_steer1, INPUT);     //input switch steer
  pinMode(switch_steer2, INPUT);
  pinMode(switch_steer3, INPUT);
  pinMode(switch_steer4, INPUT);
  digitalWrite(switch_steer1, HIGH);
  digitalWrite(switch_steer2, HIGH);
  digitalWrite(switch_steer3, HIGH);
  digitalWrite(switch_steer4, HIGH);

  //pinMode(led_nn, OUTPUT);
  pinMode(led_main, OUTPUT);

  // Ethernet
  client.setServer(server, 1883);
  Ethernet.begin(mac, ip);
  client.setCallback(callback);  

  delay(1500);
}

void loop() {
  int pulse_speed_in_left, pulse_speed_in_right;
  int pulse_steer_in_left, pulse_steer_in_right;
  int pulse_tunning1, pulse_tunning2;
  double kp1, ki1, kd1, kp2, ki2, kd2, kp3, ki3, kd3, kp4, ki4, kd4;
  char msgBuffer[20];

/* if (!client.connected()) {
   reconnect();
  }
*/

 /*NN*/
  state_nn = digitalRead (switch_nn);
  if (state_nn == LOW){
    //LED indikator blink 
    digitalWrite(led_main, HIGH);     // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(led_main, LOW);      // turn the LED off by making the voltage LOW
    delay(500);                       // wait for a second
    // switch on, indicator on:
    // client.subscribe("switch_nn");
    client.publish("switch_nn", "NN_on");
    Serial.println(" ");
    Serial.println(" NN:on");
  }
  else{
    client.publish("switch_nn", "NN_off");
    Serial.println(" ");
    Serial.println(" NN:off");
  }

  /* Manual*/
  state_manual = digitalRead (switch_manual);

  if (state_manual == LOW) {                        
    // manual switch on, indicator on:
    digitalWrite(led_main, HIGH);
    Serial.println(" manual on");

    /*Speed Control*/
    int state_speed_left = digitalRead(switch_speed_left);
    int state_speed_right = digitalRead(switch_speed_right);
  
    Serial.print(state_speed_left);
    Serial.print(state_speed_right);
    
    //Speed Control Left
    if (state_speed_left == LOW) {
    speed_left = analogRead(joy_speed_left);
    delay(100);
    Serial.print(" Speed left: ");
    pulse_speed_in_left = pulse_speed(speed_left);
    //pulse_speed_in_left = pulse_speed(e);
    Serial.print(pulse_speed_in_left);
    client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 5, 0, msgBuffer));
    }

    //Speed Control Right
    if (state_speed_right == LOW) {
    speed_right = analogRead(joy_speed_right);
    delay(100);
    Serial.print(" Speed right: ");
    pulse_speed_in_right = pulse_speed(speed_right);
    //pulse_speed_in_right = pulse_speed(e);
    Serial.println(pulse_speed_in_right);
    client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 5, 0, msgBuffer));
    }

    /*Steer Control (Main)*/
    int state_steer_left = digitalRead(switch_steer_left);
    int state_steer_right = digitalRead(switch_steer_right);
  
    Serial.print(state_steer_left);
    Serial.print(state_steer_right);
    
    //Steer Left
    if (state_steer_left == LOW) {
    steer_left = analogRead(joy_steer_left);
    delay(100);
    Serial.print(" Steer left: ");
    pulse_steer_in_left = pulse_steer(steer_left);
    //pulse_steer_in_left = pulse_steer(e);
    Serial.println(pulse_steer_in_left);
    client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 5, 0, msgBuffer));
    }
    
    //Steer Right
    if (state_steer_right == LOW) {
    steer_right = analogRead(joy_steer_right);
    delay(100);
    Serial.print(" Steer right: ");
    pulse_steer_in_right = pulse_steer(steer_right);
    //pulse_steer_in_right = pulse_steer(e);
    Serial.println(pulse_steer_in_right);
    client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 5, 0, msgBuffer));
    }

    //Tunning
    int state_tunning = digitalRead(switch_tunning); // input swicth main
    if (state_tunning == LOW) {
      Serial.print("Tunning on");
                  
      int state_steer1 = digitalRead(switch_steer1);
      int state_steer2 = digitalRead(switch_steer2);
      int state_steer3 = digitalRead(switch_steer3);
      int state_steer4 = digitalRead(switch_steer4);
  
      Serial.print(state_steer1);
      Serial.print(state_steer2);
      Serial.print(state_steer3);
      Serial.print(state_steer4);

      // switch tunning 1 on
      if (state_steer1 == LOW && state_steer2 == HIGH && state_steer2 == HIGH && state_steer2 == HIGH) {
        tunning1 = analogRead(joy_tunning1);
        tunning2 = analogRead(joy_tunning2);
        tunning3 = analogRead(joy_tunning3);
        delay(500);
        Serial.println(" T.STEER1: ");
        kp1 = pulse_tunning(tunning1, kp1);
        ki1 = pulse_tunning(tunning2, ki1);
        kd1 = pulse_tunning(tunning3, kd1);
        //pulse_tunning_in = pulse_steer(e);
       Serial.print(" kp:");
       Serial.print(kp1);
       Serial.print(" ki:");
       Serial.print(ki1);
       Serial.print(" kd:");
       Serial.print(kd1);
        client.publish("steer1_kp",dtostrf(kp1, 5, 0, msgBuffer));
        client.publish("steer1_ki",dtostrf(ki1, 5, 0, msgBuffer));
        client.publish("steer1_kd",dtostrf(kd1, 5, 0, msgBuffer));
        } 

      // switch tunning 2 on
      if (state_steer1 == HIGH && state_steer2 == LOW && state_steer3 == HIGH && state_steer4 == HIGH) {
        tunning1 = analogRead(joy_tunning1);
        tunning2 = analogRead(joy_tunning2);
        tunning3 = analogRead(joy_tunning3);
        delay(500);
        Serial.println(" T.STEER2: ");
        kp2 = pulse_tunning(tunning1, kp2);
        ki2 = pulse_tunning(tunning2, ki2);
        kd2 = pulse_tunning(tunning3, kd2);
        Serial.print(" kp:");
        Serial.print(kp2);
        Serial.print(" ki:");
        Serial.print(ki2);
        Serial.print(" kd:");
        Serial.print(kd2);
        client.publish("steer2_kp",dtostrf(kp2, 5, 0, msgBuffer));
        client.publish("steer2_ki",dtostrf(ki2, 5, 0, msgBuffer));
        client.publish("steer2_kd",dtostrf(kd2, 5, 0, msgBuffer));
        }

     // switch tunning 3 on
     if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer3 == LOW && state_steer4 == HIGH) {
       tunning1 = analogRead(joy_tunning1);
       tunning2 = analogRead(joy_tunning2);
       tunning3 = analogRead(joy_tunning3);
       delay(500);
       Serial.println(" T.STEER3: ");
       kp3 = pulse_tunning(tunning1, kp3);
       ki3 = pulse_tunning(tunning2, ki3);
       kd3 = pulse_tunning(tunning3, kd3);      
       Serial.print(" kp:");
       Serial.print(kp3);
       Serial.print(" ki:");
       Serial.print(ki3);
       Serial.print(" kd:");
       Serial.print(kd3);
       client.publish("steer3_kp",dtostrf(kp3, 5, 0, msgBuffer));
       client.publish("steer3_ki",dtostrf(ki3, 5, 0, msgBuffer));
       client.publish("steer3_kd",dtostrf(kd3, 5, 0, msgBuffer));
       }
 
    // switch tunning 4 on
    if (state_steer1 == HIGH && state_steer2 == HIGH && state_steer3 == HIGH && state_steer4 == LOW) {
      tunning1 = analogRead(joy_tunning1);
      tunning2 = analogRead(joy_tunning2);
      tunning3 = analogRead(joy_tunning3);
      delay(500);
      Serial.println(" T.STEER4: ");
      kp4 = pulse_tunning(tunning1, kp4);
      ki4 = pulse_tunning(tunning2, ki4);
      kd4 = pulse_tunning(tunning3, kd4);
      //pulse_tunning_in = pulse_steer(e);
      Serial.print(" kp:");
      Serial.print(kp4);
      Serial.print(" ki:");
      Serial.print(ki4);
      Serial.print(" kd:");
      Serial.print(kd4);
      client.publish("steer3_kp",dtostrf(kp4, 5, 0, msgBuffer));
      client.publish("steer3_ki",dtostrf(ki4, 5, 0, msgBuffer));
      client.publish("steer3_kd",dtostrf(kd4, 5, 0, msgBuffer));
      }

    Serial.println();
    
    }
  else {// Pixhawk process , Teguh, 16/10/2018
    digitalWrite(led_main, LOW);
    Serial.println(" SMC: off");
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
       client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 5, 0, msgBuffer));
       client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 5, 0, msgBuffer));
       client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 5, 0, msgBuffer));
       client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 5, 0, msgBuffer));
    }
    //Yaw Right
    else if ((pulse_pixhawk_4 > pulse_pixhawk_1 ) && (pulse_pixhawk_3 > pulse_pixhawk_2)){
       pulse_speed_in_right = 1100;
       pulse_speed_in_left = 1800;
       pulse_steer_in_right = 1500;
       pulse_steer_in_left = 1500;
       client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 5, 0, msgBuffer));
       client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 5, 0, msgBuffer));
       client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 5, 0, msgBuffer));
       client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 5, 0, msgBuffer));
    }
    //Another
    else {
       if ((pulse_pixhawk_4 == pulse_pixhawk_3)&&(pulse_pixhawk_3 == pulse_pixhawk_2) && (pulse_pixhawk_2 == pulse_pixhawk_1)){
        pulse_speed_in_right = 1500;
       pulse_speed_in_left = 1500;
       pulse_steer_in_right = 1500;
       pulse_steer_in_left = 1500;
       client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 5, 0, msgBuffer));
       client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 5, 0, msgBuffer));
       client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 5, 0, msgBuffer));
       client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 5, 0, msgBuffer));
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
       client.publish("spc_speed1",dtostrf(pulse_speed_in_left, 5, 0, msgBuffer));
       client.publish("spc_speed2",dtostrf(pulse_speed_in_right, 5, 0, msgBuffer));
       client.publish("spc_steer1",dtostrf(pulse_steer_in_left, 5, 0, msgBuffer));
       client.publish("spc_steer2",dtostrf(pulse_steer_in_right, 5, 0, msgBuffer));
       }
       
       }
    }
    //Serial.println(pulse_pixhawk);
    
  }
      
  client.loop();
  // Sampling Time diubah ke 1 detik , Teguh, 16/10/2018
  delay(1000); 

  }
}
