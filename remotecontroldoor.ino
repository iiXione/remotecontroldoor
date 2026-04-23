/*
    IR Remote

    Print out the received IR value. Turn on the red LED if received a specific code.
*/

#include <Arduino.h>
#include <Servo.h>
#include <IRremote.hpp>
#include <math.h>
const int IR_RECEIVE_PIN = 2;
Servo door;
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(11, OUTPUT);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  door.attach(9); 
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData);
    //IrReceiver.printIRResultShort(&Serial);
    int x = 0;
    int y = 180;
    float z = 1.04;
    float calibrate = 1;
    float val = analogRead(A0);
    if (val < 650){
      calibrate = ((843 - val)/843) + .77;
    }
    float degree = (val/843) * 180;
    Serial.print("degree = "); 
    Serial.println(degree);
    float calibrated = (degree * calibrate) + 5;
    if(calibrated > 160){
      calibrated = calibrated * 1.05;
    }
    if(calibrated > 170){
      calibrated = calibrated * 1.05;
    }
    if(calibrated > 180){
      calibrated = calibrated * 1.05;
    }
    Serial.print("calibrate = "); Serial.println(calibrate);
    Serial.print("val = "); Serial.println(val);
    Serial.print("calibrated degree = "); Serial.println(calibrated);
    switch (IrReceiver.decodedIRData.decodedRawData) {
      case 3125149440:
        //Serial.println("close");
        if(calibrated > 6 && 0 > val < 700) {
          digitalWrite(11, HIGH);
          while (calibrated > 1) {
            float newdegree = calibrated - pow(z,x) + 1;
            calibrated = newdegree;
            door.write(y - calibrated);
            delay(40);
            x += 1;
            Serial.println(calibrated);
            Serial.println("firstone");
          }
          delay(1200);
          digitalWrite(11, LOW);
          break;
        }
        if(val > 699){
          digitalWrite(11, HIGH);
          door.write(5);
          delay(600);
          digitalWrite(11,LOW);
          delay(1500);
          digitalWrite(11, HIGH);
          float val = analogRead(A0);
            if (val < 650){
              calibrate = ((843 - val)/843) + .77;
            }
            float degree = (val/843) * 180;
            Serial.print("degree = "); 
            Serial.println(degree);
            float calibrated = (degree * calibrate) + 5;
            if(calibrated > 160){
              calibrated = calibrated * 1.05;
            }
            if(calibrated > 170){
              calibrated = calibrated * 1.05;
            }
            if(calibrated > 180){
              calibrated = calibrated * 1.05;
            }
          while (calibrated > 1) {
            float newdegree = calibrated - pow(z,x) + 1;
            calibrated = newdegree;
            door.write(180 - calibrated);
            delay(40);
            x += 1;
            Serial.println(calibrated);
          }
          delay(1200);
          digitalWrite(11, LOW);     
        }
        break;
      case 3910598400:
        digitalWrite(11, HIGH);
        while (calibrated < 130) {
          float newdegree = calibrated + pow(1.02,x) - 1;
          calibrated = newdegree;
          door.write(180 - calibrated);
          delay(40);
          x += 1;
          Serial.println(calibrated);
        }
        delay(1000);
        while (calibrated > 0) {
          float newdegree = calibrated - pow(1.035,x) + 1;
          calibrated = newdegree;
          door.write(180 - calibrated);
          delay(40);
          x += 1;
          Serial.println(calibrated);
        }
        delay(2000);
        digitalWrite(11, LOW);
        break;         
      default:
        break;
    }
    IrReceiver.resume();  // Enable receiving of the next value
    delay(500);
  }
}