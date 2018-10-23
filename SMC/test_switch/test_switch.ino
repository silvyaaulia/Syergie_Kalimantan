//Test_switch 
//18/10/18


//Pin Switch
const int switch_nn = 22;
const int switch_manual = 47;           // input swicth main
const int switch_tunning = 49;          // input swicth main

const int switch_speed_left = 51;  
const int switch_speed_right = 28; 
 
const int switch_steer_left = 53;  
const int switch_steer_right = 30;  

const int switch_steer1 = 36;           //tunning steer 1
const int switch_steer2 = 37;           //tunning steer 2
const int switch_steer3 = 38;           //tunning steer 3
const int switch_steer4 = 39;           //tunning steer 4


// Initialize State Switch
int state_nn = 0;                      //state nn
int state_manual = 0;                  //state manual
int state_tunning = 0;                 //state tunning

int state_steer_left = 0;              //front left
int state_steer_right = 0;             //back left

int state_speed_left = 0;              //front left
int state_speed_right = 0;             //back left

int state_steer1 = 0;                  //state steer
int state_steer2 = 0;
int state_steer3 = 0;
int state_steer4 = 0;


void setup() {
  // Serial begin
  Serial.begin(57600);

  pinMode(switch_speed_left, INPUT);              //switch speed
  pinMode(switch_speed_right, INPUT);
  digitalWrite(switch_speed_left, HIGH);
  digitalWrite(switch_speed_right, HIGH);

  pinMode(switch_steer_left, INPUT);              //switch steer
  pinMode(switch_steer_right, INPUT);
  digitalWrite(switch_steer_left, HIGH);
  digitalWrite(switch_steer_left, HIGH);

  pinMode(switch_steer1, INPUT);                  //tunning PID
  pinMode(switch_steer2, INPUT);
  pinMode(switch_steer3, INPUT);
  pinMode(switch_steer4, INPUT);
  digitalWrite(switch_steer1, HIGH);
  digitalWrite(switch_steer2, HIGH);
  digitalWrite(switch_steer3, HIGH);
  digitalWrite(switch_steer4, HIGH);
}

void loop() {
      state_nn = digitalRead(switch_nn);
      state_manual = digitalRead(switch_manual);
      state_tunning = digitalRead(switch_tunning);
      Serial.print("nn:");
      Serial.print(state_nn);
      Serial.print("  manual:");
      Serial.print(state_manual);
      Serial.print("  tunning:");
      Serial.print(state_tunning);

  
      state_steer1 = digitalRead(switch_steer1);
      state_steer2 = digitalRead(switch_steer2);
      state_steer3 = digitalRead(switch_steer3);
      state_steer4 = digitalRead(switch_steer4);
      Serial.print("     1:");
      Serial.print(state_steer1);
      Serial.print("  2:");
      Serial.print(state_steer2);
      Serial.print("  3:");
      Serial.print(state_steer3);
      Serial.print("  4:");
      Serial.print(state_steer4);
      

      state_steer_left = digitalRead(switch_steer_left);
      state_speed_left = digitalRead(switch_speed_left);
      Serial.print("     Steer left:");
      Serial.print(state_steer_left);
      Serial.print("  Speed left");
      Serial.print(state_speed_left);

      
      state_steer_right = digitalRead(switch_steer_right);
      state_speed_right = digitalRead(switch_speed_right);
      Serial.print("     Steer right:");
      Serial.print(state_steer_right);
      Serial.print("  Speed right");
      Serial.println(state_speed_right);
      
      delay(1000);
}
