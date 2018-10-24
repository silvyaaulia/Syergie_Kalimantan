#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "Syergie.h"
//#include <FreeRTOS.h>

#define  pwm_right 3
#define  pwm_left 5

// define two tasks for Blink & AnalogRead
void TaskRpmMeasurement( void *pvParameters );
void TaskMQTT( void *pvParameters );

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
IPAddress server(192, 168, 1, 2);

EthernetClient ethClient;
PubSubClient client(ethClient);

/* Fuel Tank Level*/
int level_tanki;
char send_rpm_engine[10];
char send_rpm_propeller[10];
char send_tanki[10];

/* RPM Measurement */
Proximity speed_1;
Proximity speed_2;
int rpm_1, rpm_2;
int Speed = 1500;

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if(topic[1]=='p'){
    int i, value;
    client.publish("confirm_SPC1","Received");
    value = 0;
    for ( i = 0; i < length; i++) {
    value = value*10;
    value = value + (payload[i] - 48);//*dig;
    Speed = value;
    }
  }
    
  else if(topic[0]=='M'){
    client.publish("1EngineSpeed",dtostrf(rpm_1, 4, 0, send_rpm_engine));
    client.publish("1PropellerSpeed",dtostrf(rpm_2, 4, 0, send_rpm_propeller));
    client.publish("1Fuel",dtostrf(level_tanki, 2, 0, send_tanki));
    client.publish("1Box","connected");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection... "));
    // Attempt to connect
    if (client.connect("Box1")) {
      Serial.print(server);
      Serial.println(F(" connected"));
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
	  
      // ... and resubscribe
      //client.subscribe("inTopic");
      client.subscribe("1pulse_spc");
      client.subscribe("MainControl");
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  speed_1.pinRPM(A2);
  speed_2.pinRPM(A3);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
  
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskRpmMeasurement
    ,  (const portCHAR *) "RpmMeasurement"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater, 128
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskMQTT
    ,  (const portCHAR *) "MQTT"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
    
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskRpmMeasurement(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(8, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    
    rpm_1 = speed_1.calcRPM();
    rpm_2 = speed_2.calcRPM();
    
    Serial.print(F("RPM 1 : "));Serial.println(rpm_1);
    Serial.print(F("RPM 2 : "));Serial.println(rpm_2);
    
    //Fuel Measurement
    level_tanki = map(analogRead(A1),0,1023,0,20);
    Serial.print(F("Level Tangki : "));Serial.println(level_tanki);
    
    double pwm1, pwm2;
    //Governor Control
    switch(Speed){
      case 1900: pwm1 = 200; pwm2 = 0; break; //Speed UP
      case 1500: pwm1 = 0; pwm2 = 0; break; //Steady
      case 1300: pwm1 = 0; pwm2 = 200; break; //Speed Down
    }
    Serial.print(F("PWM 1 : "));Serial.println(pwm1);
    Serial.print(F("PWM 2 : "));Serial.println(pwm2);
    analogWrite(pwm_right, pwm1);
    analogWrite(pwm_left, pwm2);
    vTaskDelay(1);
    
    //vTaskDelayUntil( &xLastWakeTime, ( 20 / portTICK_PERIOD_MS ) ); // wait for one second
  }
}

void TaskMQTT(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  //TickType_t xLastWakeTime;
  //xLastWakeTime = xTaskGetTickCount();
  int i = 0;
  char cBuffer [10];
  
  for (;;)
  {
    if (i > 100) { i = 0; }
    if (!client.connected()) {
      reconnect();
    }
    itoa(i, cBuffer, 10);
    client.publish("outTopic",cBuffer);
    client.loop();
    i++;
    vTaskDelay(1);
    
    //vTaskDelayUntil( &xLastWakeTime, ( 20 / portTICK_PERIOD_MS ) ); // wait for one second
  }
}
