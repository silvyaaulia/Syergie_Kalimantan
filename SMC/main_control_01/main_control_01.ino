/* Draft Syergie Main Control (SMC)
----------------------------------------
Speed control:
> for 4 unit propeller 
> without switch 
> manual input in program
----------------------------------------
*/

/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

/*
#define pinProp1 A0
#define pinProp2 A1
#define pinProp3 A2
#define pinProp4 A3
*/

double pulse_speed_in;

// input pulse speed
const int a = 100;
const int b = 300;
const int c = 600;
const int d = 1000;

/* Declare IP Address */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(123,205,76, 90);       //IP for arduino
IPAddress server(123, 205, 76, 67);   //IP for raspi/pc

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

double pulse_speed(int prop) {
  if (prop <=300){
    pulse_speed_in = 1100;
   }
  else if (prop <=600){
    pulse_speed_in = 1500;
   }
   else{
    pulse_speed_in = 1900;
   }
   return pulse_speed_in;
}


void setup() {
  // Serial begin
  Serial.begin(57600);

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
/*
  // read the input on analog pin 0:
  int prop1 = analogRead(pinProp1); 
  int prop2 = analogRead(pinProp2);
  int prop3 = analogRead(pinProp3);
  int prop4 = analogRead(pinProp4);
*/

 //without switch
  Serial.print("PWM_1 : ");
  //pulse_speed_in1 = pulse_speed(prop1);
  pulse_speed_in1 = pulse_speed(a);
  Serial.print(pulse_speed_in1);

  Serial.print("PWM_2 : ");
  //pulse_speed_in2 = pulse_speed(prop2);
  pulse_speed_in2 = pulse_speed(c);
  Serial.print(pulse_speed_in2);
 
  Serial.print("PWM_3 : ");
  //pulse_speed_in3 = pulse_speed(prop3);
  pulse_speed_in3 = pulse_speed(c);
  Serial.print(pulse_speed_in3);

  Serial.print("PWM_4 : ");
  //pulse_speed_in4 = pulse_speed(prop4);
  pulse_speed_in4 = pulse_speed(d);
  Serial.print(pulse_speed_in4);

 
  Serial.println();

  client.publish("1pulse_spc",dtostrf(pulse_speed_in1, 5, 0, msgBuffer));
  client.publish("2pulse_spc",dtostrf(pulse_speed_in2, 5, 0, msgBuffer));
  client.publish("3pulse_spc",dtostrf(pulse_speed_in3, 5, 0, msgBuffer));
  client.publish("4pulse_spc",dtostrf(pulse_speed_in4, 5, 0, msgBuffer));

  client.loop();
  delay(500); 

}
