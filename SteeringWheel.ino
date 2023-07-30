
//Steering wheel and throttle.
#include <Joystick.h> //Download Joystic library from here: https://github.com/MHeironimus/ArduinoJoystickLibrary

//Note: it's not working when I'm using: JOYSTICK_TYPE_MULTI_AXIS. It's not recognized as Gaming Device
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
0,0, //how many buttons
false, true, false, 
false, false, 
false, false, true); //Only using steering axis


const int potVcc = 9; //using pin 9 to provide power to potentiometer
//------------------------------------------
//Note: not every pin is usable for interrupts!
//You need to connect the signal wires to pins which are usable for interrupts: Micro, Leonardo, other 32u4-based: 0, 1, 2, 3, 7
const int outA = 1; //yellow cable
const int outB = 7; //white cable
volatile long encoder = 0; // volatile is using RAM this is because we will use interrupts
long encoder_save = 0;
int R_EN = 2;
int R_PWM = 3;
int L_EN = 4;
int L_PWM = 5; 

void setup() {

Serial.begin(9600); //you can disable serial. it's just to check if it's working

 pinMode(R_EN, OUTPUT);
 pinMode(R_PWM, OUTPUT);
 pinMode(L_EN, OUTPUT);
 pinMode(L_PWM, OUTPUT);
 digitalWrite(R_EN, HIGH);
 digitalWrite(L_EN, HIGH); 
//potentiometer
  //pinMode(potVcc, OUTPUT);
  //digitalWrite(potVcc, HIGH); //power up potentiometer
  Joystick.setYAxisRange(0, 1023);
//--------------------- 
  
Joystick.begin();
Joystick.setSteeringRange(-1600, 1600);
pinMode(outA, INPUT_PULLUP); //Encoder pin
pinMode(outB, INPUT_PULLUP); //Encoder pin
attachInterrupt(digitalPinToInterrupt(outA),outAChange,CHANGE); //CHANGE to trigger the interrupt whenever the pin changes value
attachInterrupt(digitalPinToInterrupt(outB),outBChange,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (encoder_save != encoder) { // only print if new value
    encoder_save = encoder; 
    Joystick.setSteering(encoder/7);
    Serial.println(encoder/22);
  }
  delay(10);
  //potencjometr gaz----------- 
  int ThrottlePot = analogRead(A0);
  Joystick.setYAxis(ThrottlePot);

  if (encoder/22 >= 15 || encoder/22 <= -15){
    if(encoder/22>=15){
       while(encoder/22 >= 15){
        delay(50);
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, 20);
        Joystick.setSteering(encoder/7);
        Serial.println(encoder/22);
        if (encoder/22 ==-5){
        analogWrite(R_PWM, 20);
        analogWrite(L_PWM, 0);
        delay(50);
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, 0);
        Serial.println(encoder/22 + "yo2");
        break;
        }
      }
    }
      else if (encoder/22 <= -15){
        while(encoder/22 <= -15){
        delay(50);
        analogWrite(R_PWM, 20);
        analogWrite(L_PWM, 0);
        Joystick.setSteering(encoder/7);
        Serial.println(encoder/22);
        if(encoder/22 ==5){
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, 20);
        delay(50);
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, 0);
        Serial.println(encoder/22 + "yo4");
        break;
        }
      }
    } 
  }
  else if (encoder/22 >= -15 && encoder/22 <= 15){
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
    Joystick.setSteering(encoder/7);
  }  
}


void outAChange() {
  // when outA changes, outA==outB means negative direction
  encoder += digitalRead(outA) == digitalRead(outB) ? -1 : 1; 
}

void outBChange() {
  // when outB changes, outA==outB means positive direction
  encoder += digitalRead(outA) == digitalRead(outB) ? 1 : -1;
}
