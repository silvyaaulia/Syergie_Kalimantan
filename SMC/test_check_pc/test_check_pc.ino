
/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int check_pc, pc =0;

/* Declare IP Address */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(10, 48, 20, 25);        //IP for arduino
IPAddress server(10, 48, 20, 36);   //IP for raspi/pc

EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  //char data_in[2] = [o,k];
  char data_in[100];
  
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    data_in[i] = (char)payload[i];
  }

  if(strcmp(topic,"MainControl") == 0){
    client.publish("ControlBox","connected");
  }

  if(strcmp(topic,"cek") == 0){
  //  for (int i=0;i<length;i++) {
  //    data_in[i] != (char)payload[i];
  //  }
  pc = 1;
  }
  
}


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
      client.subscribe("cek");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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
  if (!client.connected()) {
   reconnect();
  }

  check_pc = pc;
  
  if (check_pc==1){
    Serial.println("pc aktif");
  }
  else{
    Serial.println("joystik aktif");
  }
    

  client.loop();
  delay(5000); 
}
