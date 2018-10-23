/* Draft Syergie Main Control 
----------------------------------------
Main Control:
on   : input from joystick
off  : manual input from Pixhawk
----------------------------------------
Speed Control:
> 4 unit with switch (and LED indicator)
> input manual (belum pakai joystick)
----------------------------------------
Steer Control:
> 4 unit with switch (and LED indicator)
> input manual (belum pakai joystick)
----------------------------------------
*/

/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Joystick
#define joy_speed1 A0       //input joystick speed (kanan)
#define joy_speed2 A1
#define joy_speed3 A2
#define joy_speed4 A3

#define joy_steer1 A4       //input joystick steer (atas)
#define joy_steer2 A5
#define joy_steer3 A6
#define joy_steer4 A7

// Pixhawk
const int pixhawk = 23;
double pulse_pixhawk;

// Initialize state joystick
int speed1 = 0;
int speed2 = 0;
int speed3 = 0;
int speed4 = 0;

int steer1 = 0;
int steer2 = 0;
int steer3 = 0;
int steer4 = 0;

// Pin switch
const int switch_main = 22; // input swicth main

const int switch_speed1 = 26;  // input switch speed
const int switch_speed2 = 27;
const int switch_speed3 = 28;
const int switch_speed4 = 29;

const int switch_steer1 = 32;  //input switch steer
const int switch_steer2 = 33;
const int switch_steer3 = 34;
const int switch_steer4 = 35;

// Initialize state switch
int state_main = 0;       //state main

int state_speed1 = 0;     //state speed
int state_speed2 = 0;
int state_speed3 = 0;
int state_speed4 = 0;

int state_steer1 = 0;     //state steer
int state_steer2 = 0;
int state_steer3 = 0;
int state_steer4 = 0;

// LED indikator
//int led_main;     //output speed

int led_speed1;   //output speed
int led_speed2;
int led_speed3;
int led_speed4;

int led_steer1;   //output steer
int led_steer2;
int led_steer3;
int led_steer4;

// pin LED
const int led_main = 13;
//const int outLed1 = 13;
//const int outLed2 = 12;

double pulse_speed_in;
double pulse_steer_in;

// Input pulse speed and steer
const int a = 100;
const int b = 300;
const int c = 500;
const int d = 650;
const int e = 865;

/* Declare IP Address */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(123, 45, 0, 9);        //IP for arduino
IPAddress server(123, 45, 0, 10);   //IP for raspi/pc

// Callback function
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

void setup() {
  // Serial begin
  Serial.begin(57600);

  // Switch
  pinMode(switch_main, INPUT);       //input switch main
  digitalWrite(switch_main, HIGH);
  
  pinMode(switch_speed1, INPUT);     //input switch speed
  pinMode(switch_speed2, INPUT);
  pinMode(switch_speed3, INPUT);
  pinMode(switch_speed4, INPUT);
  digitalWrite(switch_speed1, HIGH);
  digitalWrite(switch_speed2, HIGH);
  digitalWrite(switch_speed3, HIGH);
  digitalWrite(switch_speed4, HIGH);

  pinMode(switch_steer1, INPUT);     //input switch steer
  pinMode(switch_steer2, INPUT);
  pinMode(switch_steer3, INPUT);
  pinMode(switch_steer4, INPUT);
  digitalWrite(switch_steer1, HIGH);
  digitalWrite(switch_steer2, HIGH);
  digitalWrite(switch_steer3, HIGH);
  digitalWrite(switch_steer4, HIGH);

  pinMode(led_main, OUTPUT);

  // Ethernet
  client.setServer(server, 1883);
  Ethernet.begin(mac, ip);
  client.setCallback(callback);  

  delay(1500);
}

void loop() {
  int pulse_speed_in1, pulse_speed_in2, pulse_speed_in3, pulse_speed_in4;
  int pulse_steer_in1, pulse_steer_in2, pulse_steer_in3, pulse_steer_in4;
  char msgBuffer[20];

  if (!client.connected()) {
    reconnect();
  }
 
/* Main Control */
  state_main = digitalRead (switch_main);
  
  if (state_main == LOW) {
    digitalWrite(led_main, HIGH);
    
    // main switch on, indicator on:
    Serial.println("SMC on");

    /*Speed Control*/
    int state_speed1 = digitalRead(switch_speed1);
    int state_speed2 = digitalRead(switch_speed2);
    int state_speed3 = digitalRead(switch_speed3);
    int state_speed4 = digitalRead(switch_speed4);
  
    Serial.print(state_speed1);
    Serial.print(state_speed2);
    Serial.print(state_speed3);
    Serial.print(state_speed4);

    if (state_speed1 == LOW) {
      digitalWrite(led_speed1, HIGH);
      speed1 = analogRead(joy_speed1);
      delay(100);
      // switch on, indicator on:
      Serial.print(" Speed1: ");
      pulse_speed_in1 = pulse_speed(speed1);
      //pulse_speed_in1 = pulse_speed(e);
      Serial.print(pulse_speed_in1);
      client.publish("spc_speed1",dtostrf(pulse_speed_in1, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed1, LOW);
        }

    if (state_speed2 == LOW) {
      digitalWrite(led_speed2, HIGH);
      speed2 = analogRead(joy_speed2);
      delay(100);
      // switch on, indicator on:
      Serial.print(" Speed2: ");
      pulse_speed_in2 = pulse_speed(speed2);
      //pulse_speed_in2 = pulse_speed(a);
      Serial.print(pulse_speed_in2);
      client.publish("spc_speed2",dtostrf(pulse_speed_in2, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed2, LOW);
        }

    if (state_speed3 == LOW) {
      digitalWrite(led_speed3, HIGH);
      speed3 = analogRead(joy_speed3);
      delay(100);
      // switch on, indicator on:
      Serial.print(" Speed3: ");
      pulse_speed_in3 = pulse_speed(speed3);
      //pulse_speed_in3 = pulse_speed(d);
      Serial.print(pulse_speed_in3);
      client.publish("spc_speed3",dtostrf(pulse_speed_in3, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed3, LOW);
        }

    if (state_speed4 == LOW) {
      digitalWrite(led_speed4, HIGH);
      speed4 = analogRead(joy_speed4);
      delay(100);
      // switch on, indicator on:
      Serial.print(" Speed4: ");
      pulse_speed_in4 = pulse_speed(speed4);
      //pulse_speed_in4 = pulse_speed(b);
      Serial.print(pulse_speed_in4);
      client.publish("spc_speed4",dtostrf(pulse_speed_in4, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed4, LOW);
      }
  
    Serial.println();

    /*Steer Control*/
    int state_steer1 = digitalRead(switch_steer1);
    int state_steer2 = digitalRead(switch_steer2);
    int state_steer3 = digitalRead(switch_steer3);
    int state_steer4 = digitalRead(switch_steer4);
  
    Serial.print(state_steer1);
    Serial.print(state_steer2);
    Serial.print(state_steer3);
    Serial.print(state_steer4);

    if (state_steer1 == LOW) {
      digitalWrite(led_steer1, HIGH);
      steer1 = analogRead(joy_steer1);
      delay(100);
      // switch on, indicator on:
      Serial.print(" Steer1: ");
      pulse_steer_in1 = pulse_steer(steer1);
      //pulse_steer_in1 = pulse_steer(e);
      Serial.print(pulse_steer_in1);
      client.publish("spc_steer1",dtostrf(pulse_steer_in1, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer1, LOW);
        }

    if (state_steer2 == LOW) {
      digitalWrite(led_steer2, HIGH);
      steer2 = analogRead(joy_steer2);
      // switch on, indicator on:
      Serial.print(" Steer2: ");
      pulse_steer_in2 = pulse_steer(steer2);
      //pulse_steer_in2 = pulse_steer(a);
      Serial.print(pulse_steer_in2);
      client.publish("spc_steer2",dtostrf(pulse_steer_in2, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer2, LOW);
        }

    if (state_steer3 == LOW) {
      digitalWrite(led_steer3, HIGH);
      steer3 = analogRead(joy_steer3);
      // switch on, indicator on:
      Serial.print(" Steer3: ");
      pulse_steer_in3 = pulse_steer(steer3);
      //pulse_steer_in3 = pulse_steer(d);
      Serial.print(pulse_steer_in3);
      client.publish("spc_steer3",dtostrf(pulse_steer_in3, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer3, LOW);
        }

    if (state_steer4 == LOW) {
      digitalWrite(led_steer4, HIGH);
      steer4 = analogRead(joy_steer4);
      // switch on, indicator on:
      Serial.print(" Steer4: ");
      pulse_steer_in4 = pulse_steer(steer4);
      //pulse_steer_in4 = pulse_steer(b);
      Serial.print(pulse_steer_in4);
      client.publish("spc_steer4",dtostrf(pulse_steer_in4, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer4, LOW);
      }
  
    Serial.println();
    
  }
  else {
    Serial.println("SMC off");
    pulse_pixhawk = pulseIn(pixhawk, HIGH);
    digitalWrite(led_main, LOW);
  }
      
  client.loop();
  delay(500); 

}
