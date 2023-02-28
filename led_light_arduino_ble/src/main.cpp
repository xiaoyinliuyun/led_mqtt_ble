#include <Arduino.h>

/*
 * 接收端：接收蓝牙串口输入：触发两个led灯按要求闪烁
 */
const int rLedPin = 6;  //引脚R
const int gLedPin = 5;  //引脚G
const int bLedPin = 3;  //引脚B


const int rLedPin2 = 11;  //引脚R
const int gLedPin2 = 10;  //引脚G
const int bLedPin2 = 9;   //引脚B

const int btnPin = 2;  // 引脚2


int ledR = 255;  //R Led 亮度
int ledG = 255;  //G Led 亮度
int ledB = 0;    //B Led 亮度

void setup() {
  pinMode(rLedPin, OUTPUT);
  pinMode(gLedPin, OUTPUT);
  pinMode(bLedPin, OUTPUT);


  pinMode(rLedPin2, OUTPUT);
  pinMode(gLedPin2, OUTPUT);
  pinMode(bLedPin2, OUTPUT);

  pinMode(btnPin, INPUT_PULLUP);

  Serial.begin(9600);
}


int delayTime = 1970;

void increase(bool r, bool g, bool b, bool r2, bool g2, bool b2){
  for (int i = 0; i < 256; i++) {
    analogWrite(rLedPin, r?i:255);
    analogWrite(gLedPin, g?i:255);
    analogWrite(bLedPin, b?i:255);

    analogWrite(rLedPin2, r2?i:255);
    analogWrite(gLedPin2, g2?i:255);
    analogWrite(bLedPin2, b2?i:255);

    delayMicroseconds(delayTime);
  }
}

void decrease(bool r, bool g, bool b, bool r2, bool g2, bool b2){
  for (int i = 255; i >= 0; i--) {
    analogWrite(rLedPin, r?i:255);
    analogWrite(gLedPin, g?i:255);
    analogWrite(bLedPin, b?i:255);

    analogWrite(rLedPin2, r2?i:255);
    analogWrite(gLedPin2, g2?i:255);
    analogWrite(bLedPin2, b2?i:255);

    delayMicroseconds(delayTime);
  }
}

void loop() {
  // 按钮
  // int value = digitalRead(btnPin);
  decrease(true, false, false, false, true, false);
  increase(true, false, false, false, true, false);
  decrease(false, true, false, false, false, true);
  increase(false, true, false, false, false, true);
  decrease(false, false, true, true, false, false);
  increase(false, false, true, true, false, false);

  // if (Serial.available()>0) {
  //   char serialCmdChar = Serial.read();
  //   serialCmd(serialCmdChar);
  // }
}

void serialCmd(char serialCmdChar) {  //r128g100b20
  switch (serialCmdChar) {
    case 'r':
      ledR = Serial.parseInt();
      break;

    case 'g':
      ledG = Serial.parseInt();
      break;

    case 'b':
      ledB = Serial.parseInt();
      break;

    case 'c':
      ledR = 0;
      ledG = 0;
      ledB = 0;
  }

  analogWrite(rLedPin, ledR);
  delay(100);
  analogWrite(gLedPin, ledG);
  delay(100);
  analogWrite(bLedPin, ledB);
  delay(100);

  Serial.print("Red Value = ");
  Serial.println(ledR);
  Serial.print("Green Value = ");
  Serial.println(ledG);
  Serial.print("Blue Value = ");
  Serial.println(ledB);
  Serial.println("-------------");
}