#include <Adafruit_SSD1306.h>
#include "Adafruit_VL53L0X.h"
//#include <analogWrite.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

//MOTOR A
int RB=16;
int RF=17;
int LF=5;
int LB=18;
int lColor = 34;
int rColor = 39;

const int freq = 4000;
const int RBC = 0;
const int RFC = 1;
const int LBC = 2;
const int LFC = 3;
const int resolution = 8;

const int TrigPin = 26;
const int EchoPin = 25;
const int PowerPin = 33;
const int ReadPin = 27;

float dist;

void setup() {
    ledcAttachPin(RB, RBC);
  ledcAttachPin(RF, RFC);
  ledcAttachPin(LB, LBC);
  ledcAttachPin(LF, LFC);
  ledcSetup(RBC, freq, resolution);
  ledcSetup(RFC, freq, resolution);
  ledcSetup(LBC, freq, resolution);
  ledcSetup(LFC, freq, resolution);
  // put your setup code here, to run once:
  pinMode(RF, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(LB, OUTPUT);
   pinMode(PowerPin, OUTPUT);
      pinMode(ReadPin, INPUT);
     pinMode(TrigPin, OUTPUT);
     // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
     pinMode(EchoPin, INPUT);
     //Serial.println( "Ultrasonic sensor:" );
// 初始化串口通信及连接SR04的引脚
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
   // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  if (!lox.begin()) {
    while(1);
  } 
}

void sensorInfo(){
  //int x = 8;
  VL53L0X_RangingMeasurementData_t measure;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  //display.print("R-color: ");
  //display.print(rColor);
  //display.setCursor(0, x);
  //display.print("L-color: ");
  //display.print(lColor);
  display.setCursor(0, 0);
  display.print("Dist-sensor-forward: ");
  display.print(Distance());
  display.setCursor(0, 16);
  display.print("Dist-sensor-right: ");
  display.setCursor(0, 24);
  display.print(dist);
  display.print(" cm");
  display.display();
}

int Distance(){
  VL53L0X_RangingMeasurementData_t measure;
  
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data 
    return (measure.RangeMilliMeter);} 
    else {
    return -1;}
  }

void forward(){
    ledcWrite(RBC, 0);
    ledcWrite(RFC, 180);
    ledcWrite(LBC, 0);
    ledcWrite(LFC, 170);
  }

void stopi(){
 ledcWrite(RBC, 0);
  ledcWrite(RFC, 0);
  ledcWrite(LBC, 0);
  ledcWrite(LFC, 0);
  }

void right(){
  ledcWrite(RBC, 180);
  ledcWrite(RFC, 0);
  ledcWrite(LBC, 0);
  ledcWrite(LFC, 180);  
  }
 void left(){
  ledcWrite(RBC, 0);
  ledcWrite(RFC, 180);
  ledcWrite(LBC, 180);
  ledcWrite(LFC, 0);     
  }
  void ultrasonic(){
   // 产生一个10us的高脉冲去触发TrigPin
         
         digitalWrite(TrigPin, LOW);
         delayMicroseconds(2);
         digitalWrite(TrigPin, HIGH);
         delayMicroseconds(10);
         digitalWrite(TrigPin, LOW);
     // 检测脉冲宽度，并计算出距离
         dist = pulseIn(EchoPin, HIGH)/58;
         Serial.print(dist);
         Serial.print( "cm" );
         Serial.println();
         //delay(1000);
}

void loop() {
   digitalWrite(PowerPin, HIGH);
    //digitalWrite(ReadPin, LOW);
     Serial.println(analogRead(ReadPin));
   //Serial.println();
  // put your main code here, to run repeatedly:
   sensorInfo();
   ultrasonic();
  if (Distance() > 220){
    forward();
    /*
    if(distance_left > 10&& distance_left<20){
      forward();
    }
    if(distance_left >=20){
       left();
       delay(30);
       forward();
    }
    if(distance_left<10 && distance_left>0){
      right();
      delay(30);
      forward();
    }
    */
 } 
  
  if(Distance()<=220 && dist > 18){
    
    stopi();
    delay(1000);
    right();
    delay(400);
    
  }

  if(Distance()<=220 && dist<18){
    stopi();
    delay(1000);
    left();
    delay(400);
   
  }
}
