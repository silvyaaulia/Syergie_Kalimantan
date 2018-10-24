#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define rxPin 5
#define txPin 4

// set up a new serial port
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

// Update these with values suitable for your network.

const char* ssid = "Syergie Main Control";
const char* password = "1234567890";
//const char* mqtt_server = "syergie-control.local";

IPAddress mqtt_server(123, 45, 0, 10);
WiFiClient espClient;
PubSubClient client(espClient);

//DEFINITIONS FOR SERIAL COMMUNICATION ESP ID
#define rpm_engine_ID 'R'
#define rpm_propeller_ID 'P'
#define rpm_pump_ID 'M'
#define tangki_ID 'T'
#define steer_ID 'R'
#define azimuth_ID 'A'
#define speed_ID 'S'
#define Kp_ID 'P'
#define Ki_ID 'I'
#define Kd_ID 'D'

// Fuel Tank Level, Steer, Depth, and PWM
int level_tanki = 0;
int steer = 0;
int depth = 0;

int pwm1;
int pwm2;

//Buffers for Publishing via MQTT
char buf[10];

//Buffers for Serial Communication ESP
char st[10];

// RPM Measurement
int rpm_1 = 0;
int rpm_2 = 0;
int rpm_3 = 0;

int azimuth = 0;
int Speed = 1500;

//PID CONSTANTS
double P = 2.5209;//nambah 0.01
double I = 0.0000293; //0.0001
double D = 0.2353;//0.01
double Kp_baru,Ki_baru,Kd_baru;

double Buff; // posisi baru
double Last = 0; // posisi lama
double PTerm;
double ITerm;
double DTerm;
double PIDTerm;
double Ts = 1;

double cepat = 0;
double position_out = 0 ;
double position_in = 500;
double Error; // perbedaan feedback dengan input
double Sum = 0; // hasil integral error

char buf_Rx[100],buf_Tx[100];
int i=0,idx=0;
//END OF PID CONSTANTS

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  mySerial.println();
  mySerial.print("Connecting to ");
  mySerial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  mySerial.println("");
  mySerial.println("WiFi connected");
  mySerial.println("IP address: ");
  mySerial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  int value = 0;
  char data_in[100];//Untuk Kp,Ki,Kd
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i=0;i<length;i++) { //storing payload to integer value
    data_in[i] = (char)payload[i];
    Serial.print((char)payload[i]);
    value = value*10;
    value = value+(payload[i]-48);
  }
  Serial.println();
  
  if(strcmp(topic,"spc_speed1") == 0){
    client.publish("confirm_speed2","Received");
    Speed = value;
    sprintf(buf_Tx,"\t%dS\t\n",Speed);
    mySerial.print(buf_Tx);
  } 
  if(strcmp(topic,"spc_steer1") == 0){
    client.publish("confirm_steer2","Received");
    steer = value;
    sprintf(buf_Tx,"\t%dR\t\n",steer);
    Serial.println(buf_Tx);
    mySerial.print(buf_Tx);
  }
  if(strcmp(topic,"steer1_kp") == 0){
    client.publish("confirm_kp2","Received");
    Kp_baru = (String(String(data_in[0])+String(data_in[1])+String(data_in[2])+String(data_in[3]))).toFloat();
    
    //sprintf(buf_Tx,"\t%fP\t\n",Kp_baru);
    //Serial.println(buf_Tx);
    mySerial.print("\t");mySerial.print(Kp_baru);mySerial.print(Kp_ID);mySerial.println("\t");
  }
  if(strcmp(topic,"steer1_ki") == 0){
    client.publish("confirm_ki2","Received");
    Ki_baru = (String(String(data_in[0])+String(data_in[1])+String(data_in[2])+String(data_in[3])+String(data_in[4])+String(data_in[5])+String(data_in[6]))).toFloat();
    //sprintf(buf_Tx,"\t%fI\t\n",Ki_baru);
    //mySerial.print(buf_Tx);
    //Serial.println(float(Ki_baru),4);
    mySerial.print("\t");mySerial.print(float(Ki_baru),5);mySerial.print(Ki_ID);mySerial.println("\t");
  }
  if(strcmp(topic,"steer1_kd") == 0){
    client.publish("confirm_kd2","Received");
    Kd_baru = (String(String(data_in[0])+String(data_in[1])+String(data_in[2])+String(data_in[3]))).toFloat();
    sprintf(buf_Tx,"\t%fD\t\n",Kd_baru);
    //mySerial.print(buf_Tx);
    Serial.println(float(Kd_baru),4);
    mySerial.print("\t");mySerial.print(float(Kd_baru),5);mySerial.print(Kd_ID);mySerial.println("\t");
  }
  else if(topic[0]=='M'){
    client.publish("1EngineSpeed",dtostrf(rpm_1, 4, 0, buf));
    client.publish("1PropellerSpeed",dtostrf(rpm_2, 4, 0, buf));
    client.publish("1PumpSpeed",dtostrf(rpm_3, 4, 0, buf));
    client.publish("1Fuel",dtostrf(level_tanki, 2, 0, buf));
    client.publish("1Azimuth",dtostrf(azimuth, 2, 0, buf));
    client.publish("1Box","connected");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_server);
    Serial.print(" : ");
    mySerial.println();
    mySerial.print("Attempting MQTT connection to ");
    mySerial.print(mqtt_server);
    mySerial.println(" : ");
    // Attempt to connect
    if (client.connect("ESP8266Client2")) {
      Serial.println(" connected");
      mySerial.println(" connected");
      // Once connected, publish an announcement...
      // ... or not...
      // ... and resubscribe
      client.subscribe("spc_speed1");
      client.subscribe("spc_steer1");
      client.subscribe("steer1_kp");
      client.subscribe("steer1_ki");
      client.subscribe("steer1_kd");
      client.subscribe("MainControl");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      mySerial.print("failed, rc=");
      mySerial.print(client.state());
      mySerial.println(" try again in 1 seconds");
      // Wait 1 seconds before retrying
      delay(1000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if (Serial.available()) {
    buf_Rx[i] = Serial.read();
    if(buf_Rx[i]=='\n'){
      i=-1;
      Serial.print("Serial Message arrived ");
      Serial.print(buf_Rx);
    }
    else if(buf_Rx[i]=='\t'){
        idx=0;
        dtostrf(0, 10, 0, st);
    }
    else if(buf_Rx[i]==rpm_engine_ID){
        Serial.print("RPM Engine = ");
        rpm_1 = atoi(st);
        Serial.println(rpm_1);
    }
    else if(buf_Rx[i]==rpm_propeller_ID){
        Serial.print("RPM Propeller = ");
        rpm_2 = atoi(st);
        Serial.println(rpm_2);
    }
    else if(buf_Rx[i]==rpm_pump_ID){
        Serial.print("RPM Pump = ");
        rpm_3 = atoi(st);
        Serial.println(rpm_3);
    }
    else if(buf_Rx[i]==tangki_ID){
        Serial.print("Level Tangki = ");
        level_tanki  = atoi(st);
        Serial.println(level_tanki);
    }
    else if(buf_Rx[i]==azimuth_ID){
        Serial.print("Azimuth = ");
        azimuth = atoi(st);
        Serial.println(azimuth);
    }
    st[idx]=buf_Rx[i];
    i++;
    idx++;
  } 
  client.loop();
}
