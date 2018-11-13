//Test_switch 
//07/11/18


//Pin Switch
const int switch_winch = 5;
const int switch_manual = 4;           // input swicth main
const int switch_tunning = 9;          // input swicth main

const int switch_speed_left = 6;  
const int switch_speed_right = 28; 
 
const int switch_steer_left = 7;  
const int switch_steer_right = 29;  

const int switch_steer1 = 24;           //tunning steer 1
const int switch_steer2 = 25;           //tunning steer 2
const int switch_steer3 = 26;           //tunning steer 3
const int switch_steer4 = 27;           //tunning steer 4


// Initialize State Switch
int state_winch = 0;                      //state nn
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
  analogWrite(switch_speed_left, 255);
  digitalWrite(switch_speed_right, HIGH);

  pinMode(switch_steer_left, INPUT);              //switch steer
  pinMode(switch_steer_right, INPUT);
  digitalWrite(switch_steer_left, HIGH);
  digitalWrite(switch_steer_right, HIGH);

  pinMode(switch_winch ,INPUT);
  analogWrite(switch_winch ,255);
  pinMode(switch_manual ,INPUT);
  analogWrite(switch_manual ,255);
  pinMode(switch_tunning ,INPUT);
  analogWrite(switch_tunning ,255);
  pinMode(switch_manual ,INPUT);
  analogWrite(switch_manual ,255);
  
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
      state_winch = digitalRead(switch_winch);
      state_manual = digitalRead(switch_manual);
      state_tunning = digitalRead(switch_tunning);
      Serial.print("winch:");
      Serial.print(state_winch);
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
