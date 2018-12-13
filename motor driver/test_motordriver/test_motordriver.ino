
int RPWM = 5;
int LPWM = 6;

int R_EN = 7;
int L_EN = 8;

//#define PWMPin 3

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 13 || pin == 4 || pin == 12 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 13 || pin == 4) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 5 || pin == 2) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      //case 32: mode = 0x03; break;
      case 64: mode = 0x03; break;
      //case 128: mode = 0x05; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    TCCR3B = TCCR3B & 0b11111000 | mode;
  }
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(7, OUTPUT);
  //pinMode(8, OUTPUT);
  
  //pinMode(PWMPin, OUTPUT);
  //setPwmFrequency(PWMPin, 16 ); //frekuensi = 31250/16=1953.125 Hz 
  
  pinMode (RPWM,OUTPUT);
  pinMode (LPWM,OUTPUT);
  pinMode (R_EN,OUTPUT);
  pinMode (L_EN,OUTPUT);

 
  digitalWrite(RPWM,LOW);
  digitalWrite(LPWM,LOW);
  digitalWrite(R_EN,LOW);
  digitalWrite(L_EN,LOW);

  //setPwmFrequency(RPWM, 1024); //frekuensi = 31250/16=1953.125 Hz 
  setPwmFrequency(RPWM, 1024);
  delay(1000);
  Serial.begin(9600);  
}
  

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(7,HIGH);
  //digitalWrite(8,HIGH);
  //analogWrite(PWMPin, 200);
  
  digitalWrite(R_EN,HIGH);
  digitalWrite(L_EN,HIGH);
   Serial.println("EN High");
  delay(1000);

  analogWrite(RPWM,50);
  //analogWrite(LPWM,200);
  delay(1000);  
}
