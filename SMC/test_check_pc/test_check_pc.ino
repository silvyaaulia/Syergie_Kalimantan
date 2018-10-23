/*test pc*/



/* Library ethernet */
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int check_pc, pc;
int status_pc = 0;
int back=0;

/* Declare IP Address */
byte mac[]    = {  0xDA, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(123, 45, 0, 9);        //IP for arduino
IPAddress server(123, 45, 0, 10);   //IP for raspi/pc (sebagai broker)

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  char data_in[100];
//  int status_pc = 0;
     
  for (int i=0;i<length;i++) {
    data_in[i] = (char)payload[i];
    Serial.print((char)payload[i]);
    //Serial.println;
  }
  pc =0;
  while(strcmp(topic,"active") == 0){
    pc = 1;
    }
  
  while (strcmp(topic,"coba") == 0){
    pc = 2;
    }

}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("MainArduinoO")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("MainControlL");
      client.subscribe("active");
      client.subscribe("coba");
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
  int check_pc;
  char msgBuffer[20];
   
 if (!client.connected()) {
   reconnect();
  }
  check_pc=pc;
  //check_pc = status_pc;
  if (check_pc == 1){
    Serial.println(check_pc);
    Serial.println("ini dari pc"); 
    check_pc = 0;
    
  }
  else if (check_pc == 2){
    Serial.println("coba"); 
    
  }
  else{
    Serial.println("dari joystick");
  }
 Serial.println(check_pc);
 Serial.println();

  delay(1000);
  client.loop();
}
