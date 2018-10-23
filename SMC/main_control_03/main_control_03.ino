/* Draft Syergie Main Control 
----------------------------------------
Main Control:
on   : input from joystick
off  : manual input from Pixhawk
----------------------------------------
steer Control:
> 4 unit with switch (and LED indicator)
> input manual (belum pakai joystick)
----------------------------------------
*/

/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// joystick
#define joy_steer1 A4       //input joystick steer
#define joy_steer2 A5
#define joy_steer3 A6
#define joy_steer4 A7

// pin switch
const int switch_main = 22; // input swicth main
const int pixhawk = 23;

const int switch_steer1 = 32;  //input switch steer
const int switch_steer2 = 33;
const int switch_steer3 = 34;
const int switch_steer4 = 35;

// initialize state
int state_main = 0;     //state main

int state_steer1 = 0;     //state steer
int state_steer2 = 0;
int state_steer3 = 0;
int state_steer4 = 0;

// LED indikator
//int led_main;     //output main

int led_steer1;   //output steer
int led_steer2;
int led_steer3;
int led_steer4;

// pin LED
const int led_main = 13;
//const int outLed1 = 13;
//const int outLed2 = 12;

double pulse_steer_in;

double pulse_pixhawk;


// input pulse speed and steer
const int a = 100;
const int b = 300;
const int c = 500;
const int d = 650;
const int e = 865;

/* Declare IP Address */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(123, 205, 76, 90);       //IP for arduino
IPAddress server(123, 205, 76, 65);   //IP for raspi/pc

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
      // int steer1 = analogRead(joy_steer1);
      // switch on, indicator on:
      Serial.print(" Steer1: ");
      //pulse_steer_in1 = pulse_steer(steer1);
      pulse_steer_in1 = pulse_steer(e);
      Serial.print(pulse_steer_in1);
      client.publish("1pulse_spc",dtostrf(pulse_steer_in1, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer1, LOW);
        }

    if (state_steer2 == LOW) {
      digitalWrite(led_steer2, HIGH);
      // int steer2 = analogRead(joy_steer2);
      // switch on, indicator on:
      Serial.print(" Steer2: ");
      //pulse_steer_in2 = pulse_steer(steer2);
      pulse_steer_in2 = pulse_steer(a);
      Serial.print(pulse_steer_in2);
      client.publish("2pulse_spc",dtostrf(pulse_steer_in2, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer2, LOW);
        }

    if (state_steer3 == LOW) {
      digitalWrite(led_steer3, HIGH);
      // int steer3 = analogRead(joy_steer3);
      // switch on, indicator on:
      Serial.print(" Steer3: ");
      //pulse_steer_in3 = pulse_steer(steer3);
      pulse_steer_in3 = pulse_steer(d);
      Serial.print(pulse_steer_in3);
      client.publish("3pulse_spc",dtostrf(pulse_steer_in3, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_steer3, LOW);
        }

    if (state_steer4 == LOW) {
      digitalWrite(led_steer4, HIGH);
      // int steer4 = analogRead(joy_steer4);
      // switch on, indicator on:
      Serial.print(" Steer4: ");
      //pulse_steer_in4 = pulse_steer(steer4);
      pulse_steer_in4 = pulse_steer(b);
      Serial.print(pulse_steer_in4);
      client.publish("4pulse_spc",dtostrf(pulse_steer_in4, 5, 0, msgBuffer));
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
