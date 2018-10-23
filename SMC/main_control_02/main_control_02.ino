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
*/

/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// joystick
#define joy_speed1 A0       //input joystick speed
#define joy_speed2 A1
#define joy_speed3 A2
#define joy_speed4 A3

// pin switch
const int switch_main = 22; // input swicth main
const int pixhawk = 23;

const int switch_speed1 = 26;  // input switch speed
const int switch_speed2 = 27;
const int switch_speed3 = 28;
const int switch_speed4 = 29;

// initialize state
int state_main = 0;     //state main

int state_speed1 = 0;     //state speed
int state_speed2 = 0;
int state_speed3 = 0;
int state_speed4 = 0;

// LED indikator
//int led_main;     //output speed

int led_speed1;   //output speed
int led_speed2;
int led_speed3;
int led_speed4;

// pin LED
const int led_main = 13;
//const int outLed1 = 13;
//const int outLed2 = 12;

double pulse_speed_in;

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

  pinMode(led_main, OUTPUT);

    // Ethernet
  client.setServer(server, 1883);
  Ethernet.begin(mac, ip);
  client.setCallback(callback);  

  delay(1500);
}

void loop() {
  int pulse_speed_in1, pulse_speed_in2, pulse_speed_in3, pulse_speed_in4;
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
      // int speed1 = analogRead(joy_speed1);
      // switch on, indicator on:
      Serial.print(" Speed1: ");
      //pulse_speed_in1 = pulse_speed(speed1);
      pulse_speed_in1 = pulse_speed(e);
      Serial.print(pulse_speed_in1);
      client.publish("1pulse_spc",dtostrf(pulse_speed_in1, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed1, LOW);
        }

    if (state_speed2 == LOW) {
      digitalWrite(led_speed2, HIGH);
      // int speed2 = analogRead(joy_speed2);
      // switch on, indicator on:
      Serial.print(" Speed2: ");
      //pulse_speed_in2 = pulse_speed(speed2);
      pulse_speed_in2 = pulse_speed(a);
      Serial.print(pulse_speed_in2);
      client.publish("2pulse_spc",dtostrf(pulse_speed_in2, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed2, LOW);
        }

    if (state_speed3 == LOW) {
      digitalWrite(led_speed3, HIGH);
      // int speed3 = analogRead(joy_speed3);
      // switch on, indicator on:
      Serial.print(" Speed3: ");
      //pulse_speed_in3 = pulse_speed(speed3);
      pulse_speed_in3 = pulse_speed(d);
      Serial.print(pulse_speed_in3);
      client.publish("3pulse_spc",dtostrf(pulse_speed_in3, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed3, LOW);
        }

    if (state_speed4 == LOW) {
      digitalWrite(led_speed4, HIGH);
      // int speed4 = analogRead(joy_speed4);
      // switch on, indicator on:
      Serial.print(" Speed4: ");
      //pulse_speed_in4 = pulse_speed(speed4);
      pulse_speed_in4 = pulse_speed(b);
      Serial.print(pulse_speed_in4);
      client.publish("4pulse_spc",dtostrf(pulse_speed_in4, 5, 0, msgBuffer));
      }
      else {
        digitalWrite(led_speed4, LOW);
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
