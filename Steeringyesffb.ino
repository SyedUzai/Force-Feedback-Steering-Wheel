
//Steering wheel and throttle.
#include <Joystick.h> //Download Joystic library from here: https://github.com/MHeironimus/ArduinoJoystickLibrary

//Note: it's not working when I'm using: JOYSTICK_TYPE_MULTI_AXIS. It's not recognized as Gaming Device
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
3,0, //how many buttons
false, true, false, 
false, true, 
false, true, true); //Only using Steering, Brake and Throttle

const int Button1 = 8;
const int Button2 = 9;
const int Button3 = 10;
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
int LastButtonState1 = 0; 
int LastButtonState2 = 0; 
int LastButtonState3 = 0; 

void setup() {

//Serial.begin(9600); //you can disable serial. it's just to check if it's working
pinMode(Button1, INPUT_PULLUP);
pinMode(Button2, INPUT_PULLUP);
pinMode(Button3, INPUT_PULLUP);
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
  encoderCheck();
  pedalCheck();
  buttonCheck();
  
  if (encoder/22 >= 5){
    while(encoder/22 >= 5){
    encoderCheck();
    pedalCheck();
    buttonCheck();  
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 25);
    }
  }
    else if (encoder/22 <= -5){
      while(encoder/22 <= -5){
      encoderCheck();
      pedalCheck();
      buttonCheck();
      analogWrite(R_PWM, 25);
      analogWrite(L_PWM, 0);
      }
    } 
  else{
    encoderCheck();
    pedalCheck();
    buttonCheck();
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
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

void buttonCheck(){
  int CurrentButtonState1 = digitalRead(Button1);
  if(CurrentButtonState1 != LastButtonState1){
    Joystick.setButton(0, CurrentButtonState1);
    LastButtonState1 = CurrentButtonState1;
    //Serial.println("Balls 1");
  }

  int CurrentButtonState2 = digitalRead(Button2);
  if(CurrentButtonState2 != LastButtonState2){
    Joystick.setButton(1, CurrentButtonState2);
    LastButtonState2 = CurrentButtonState2;
    //Serial.println("Balls 2");
  }

  int CurrentButtonState3 = digitalRead(Button3);
  if(CurrentButtonState3 != LastButtonState3){
    Joystick.setButton(2, CurrentButtonState3);
    LastButtonState3 = CurrentButtonState3;
    //Serial.println("Balls 3");
  }
}

void pedalCheck(){
  int ThrottlePot = analogRead(A0);
  Joystick.setYAxis(ThrottlePot * 8);
  //Serial.println(ThrottlePot);

  int BrakePot = analogRead(A5);
  Joystick.setThrottle(BrakePot * 12);
  //Serial.println(BrakePot);

}

void encoderCheck(){
  if (encoder_save != encoder){
    encoder_save = encoder;
    Joystick.setSteering(encoder/3);
    //Serial.println(encoder/22);
  }
}
