
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "Syergie.h"

#include <Arduino_FreeRTOS.h>
#include <croutine.h>
#include <event_groups.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOSVariant.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <portmacro.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <stack_macros.h>
#include <task.h>
#include <timers.h>

#define  pwm_right 3
#define  pwm_left 5

/*-----( Deklarasi Konstanta )-----*/
/*-----( Deklarasi Objek )-----*/
// Konfigurasi LCD address ke 0x3F untuk tampilan 20 karakter dan 4 baris
// Atur pin pada I2C chip untuk koneksi ke LCD:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x37, 20, 4);  // Atur LCD I2C address

// define two tasks for Blink & AnalogRead
void TaskRpmMeasurement( void *pvParameters );
void TaskMQTT( void *pvParameters );
void TaskPID( void *pvParameters );
//void TaskLCD( void *pvParameters );

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
IPAddress server(192, 168, 1, 2);

EthernetClient ethClient;
PubSubClient client(ethClient);


QueueHandle_t xQueue;

/* Fuel Tank Level*/
int level_tanki;
char send_rpm_engine[10];
char send_rpm_propeller[10];
char send_tanki[10];
char send_Speed[10];

/* RPM Measurement */
Proximity speed_1;
Proximity speed_2;
int rpm_1, rpm_2;
volatile int Speed;

//PID CONSTANTS
double P = 3.0209;
double I = 0.0000293;
double D = 0.2353;

int LPWM=12;
int RPWM=5;
int RPWM1 = 3;
int LPWM1 = 9;
int RPWM2=2;
int LPWM2=4;
int RPWM3 = 10;
int LPWM3 = 12;
int L_EN=8;
int R_EN=7;

volatile double pwm; //???
volatile double pwm1;
volatile double pwm2;
volatile double pwm3;

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
double messi;
double Error; // perbedaan feedback dengan input
double Sum = 0; // hasil integral error
//PID CONSTANTS END

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  int value = 0;
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    value = value*10;
    value=value+(payload[i]-48);
  }
  Serial.println();
  
  if(topic[1]=='p'){
    client.publish("confirm_SPC1","Received");
//    Speed = value;
    xQueueSendToBack( xQueue, &value, 0 );
    Serial.print(F("value : "));Serial.println(value);
//    Serial.print(F("Speed : "));Serial.println(Speed);
    Serial.print(F("PWM 1 : "));Serial.println(pwm1);
    Serial.print(F("PWM 2 : "));Serial.println(pwm2);
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
  xQueue = xQueueCreate( 5, sizeof( int32_t ) );
  if(xQueue == NULL){
    Serial.println("Error creating the queue");
  }
  
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

//  lcd.begin(20,4);         // inisialisasi LCD dengan 20 karakter dan 4 baris
  lcd.init();
  
// ------- Melakukan blink 3 kali (Opsional) -------------
  for(int j = 0; j< 3; j++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // Diakhiri dengan backlight on  

////-------- Menulis/menampilkan karakter pada LCD ------------------
//  // CATATAN: Posisi Kursor: Baris dan Karakter mulai pada 0  
//  lcd.setCursor(3,0); //Huruf akan mulai pada karakter ke 4 dan baris 0 pada LCD
//  lcd.print("Hello, world!");
//  delay(1000);
//  lcd.setCursor(2,1);
//  lcd.print("From YourHand");
//  delay(1000);  
//  lcd.setCursor(0,2);
//  lcd.print("20 by 4 Line Display");
//  lcd.setCursor(0,3);
//  delay(2000);   
//  lcd.print("GOOD DAY");
//  delay(8000);
//  // Tunggu selama 8 detik untuk dapat mengetik karakter pada serial monitor 
//  lcd.setCursor(0,0); //Mulai dari Karakter 0 dan baris 0
//  lcd.print("Start Serial Monitor");
//  lcd.setCursor(0,1);
//  lcd.print("Type chars 2 display");
  
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

  xTaskCreate(
    TaskPID
    ,  (const portCHAR *) "PID"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
    
//  xTaskCreate(
//    TaskLCD
//    ,  (const portCHAR *) "LCD"
//    ,  128  // Stack size
//    ,  NULL
//    ,  1  // Priority
//    ,  NULL );


  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop(){} // Empty. Things are done in Tasks. 

/*---------------------- Tasks ---------------------*/
void TaskRpmMeasurement(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  int xSpeed;
  int countA = 0;
  int countB = 0;
  for (;;) // A Task shall never return or exit.
  {
    countB = millis() - countA;
    countA = millis();
    
    rpm_1 = speed_1.calcRPM();
    rpm_2 = speed_2.calcRPM();
    
//    Serial.print(F("RPM 1 : "));Serial.println(rpm_1);
//    Serial.print(F("RPM 2 : "));Serial.println(rpm_2);
    
    //Fuel Measurement
    level_tanki = map(analogRead(A1),0,1023,0,20);
//    Serial.print(F("Level Tangki : "));Serial.println(level_tanki);
    xQueueReceive( xQueue, &xSpeed, 0 );
    
    //Governor Control
    switch(xSpeed){
      case 1900: pwm1 = 200; pwm2 = 0; break; //Speed UP
      case 1500: pwm1 = 0; pwm2 = 0; break; //Steady
      case 1300: pwm1 = 0; pwm2 = 200; break; //Speed Down
    }
    Serial.print(F("Speed : "));Serial.println(xSpeed);
    Serial.print(F("PWM 1 : "));Serial.println(pwm1);
    Serial.print(F("PWM 2 : "));Serial.println(pwm2);
    analogWrite(pwm_right, pwm1);
    analogWrite(pwm_left, pwm2);
    
    Serial.print(F("Time TaskRPM: "));Serial.println(countB);
    vTaskDelay(1);
    
//    vTaskDelayUntil(   &xLastWakeTime, ( 120 / portTICK_PERIOD_MS ) ); // wait for one second
  }
}

void TaskMQTT(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  
  int countC = 0;
  int countD = 0;
  for (;;)
  {
    countD = millis() - countC;
    countC = millis();
    
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

//    for(int i=0; i<1000;i++){
//      countA++;
//    }
//    if(countA == 1000){
//      Serial.println(countB);
//      countB++;
//      countA = 0;
//    }

    Serial.print(F("Time TaskMQT: "));Serial.println(countD);
    vTaskDelay(1);
    
//    vTaskDelayUntil( &xLastWakeTime, ( 120 / portTICK_PERIOD_MS ) ); // wait for one second
  }
}

void TaskPID(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  double pidResult1, pidResult2;
  
  int countE = 0;
  int countF = 0;
  for (;;)
  {
    countF = millis() - countE;
    countE = millis();
    
    pidResult1 = rpm_1 * 2 * 3.14;
    pidResult2 = rpm_2 * 2 * 3.14;

    double position_out_1  = 0;
    double position_out = analogRead(A1);
    
    // Perhitungan PID
    Error = position_in - position_out;
    Buff = position_out; // untuk mencari derivatif
    Sum = Sum + Error; // hasil integral dari error
    PTerm = Error*P; // Proporsional
    //ITerm = Sum*I*Ts; // Integral
    DTerm = D*(Last - Buff)/Ts; // Derivatif
    PIDTerm = PTerm + DTerm; // total PID
    
    if (PIDTerm >= 255)
      PIDTerm = 255;
    if (PIDTerm <= -255)
      PIDTerm = -255;
    
    cepat = PIDTerm; // Hasil PID dijadikan data kecepatan
    
    
    if (pwm > 255){
      pwm = 255;
    }
    else if (pwm <-255){
      pwm = -255;
    }
    
    if (pwm2 > 255){
      pwm2 = 255;
    }
    else if (pwm <-255){
      pwm2 = -255;
    }
    
    
    if (pwm3 > 255){
      pwm3 = 255;
    }
    else if (pwm3 <-255){
      pwm3 = -255;
    }
    
    // Jika nilai kecepatan (-), stir berputar ke kanan
    if (cepat < 0){
      // putar kanan
      int reversePWM = -cepat;
      analogWrite(11, 0);
      analogWrite(12, reversePWM);
    }
    // Jika nilai kecepatan (+), stir berputar ke kanan
    else if (cepat >= 0)
    {
    // putar kiri
    int forwardPWM = cepat;
    analogWrite(11, forwardPWM);
    analogWrite(12, 0);
    }
    Last = position_out; // untuk mencari derivatif
    //delay(Ts*10);
    
//    Serial.print(F("pidResult1 1 : "));Serial.println((int)pidResult1);
//    Serial.print(F("pidResult1 2 : "));Serial.println((int)pidResult2);
    
//    client.loop();

    Serial.print(F("Time TaskPID: "));Serial.println(countF);
    vTaskDelay(Ts*1);
    
//    vTaskDelayUntil( &xLastWakeTime, ( 120 / portTICK_PERIOD_MS ) ); // wait for one second
  }
}

//void TaskLCD(void *pvParameters)  // This is a task.
//{
//  (void) pvParameters;
//  TickType_t xLastWakeTime;
//  xLastWakeTime = xTaskGetTickCount();
//  
//  int countG = 0;
//  int countH = 0;
//  for (;;)
//  {
//    countH = millis() - countG;
//    countG = millis();
//    
////    // ketika karakter datang dari serial port...
////    if (Serial.available()) {
////      // delay untuk menunggu pesan ditulis pada serial
////      delay(100);
////      // Menghapus layar
////      lcd.clear();
////      // Membaca semua karakter yang diketik pada serial monitor
////      while (Serial.available() > 0) {
////        // menampilkan setiap karakter ke lcd
////        lcd.write(Serial.read());
////      }
////    }
//
//    Serial.print(F("Time TaskLCD: "));Serial.println(countH);
//    vTaskDelay(1);
//    
////    vTaskDelayUntil( &xLastWakeTime, ( 120 / portTICK_PERIOD_MS ) ); // wait for one second
//  }
//}
