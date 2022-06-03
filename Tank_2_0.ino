#include <SoftwareSerial.h>
#include <using_servo.h>
SoftwareSerial Bluetooth(10, 11);
#define PIN_ENA 6 // Вывод управления скоростью вращения мотора №1
#define PIN_ENB 5 // Вывод управления скоростью вращения мотора №2
#define PIN_IN1 2 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 A4 // Вывод управления направлением вращения мотора №1
#define PIN_IN3 4 // Вывод управления направлением вращения мотора №2
#define PIN_IN4 7 // Вывод управления направлением вращения мотора №2
#define Laser A0
#define LED 13
#define BEEP_PIN 3
int servoPin1 = 8;
int servoPin2 = 9;
ServoMotor Servo1(servoPin1);
ServoMotor Servo2(servoPin2);
int power, s1speed, s2speed, angle, strength;
int button, mode = 1,s1=0, s2=0;
      void setup() {
Serial.begin(9600);
Bluetooth.begin(9600);
   
          pinMode(PIN_ENA, OUTPUT);
          pinMode(PIN_ENB, OUTPUT);
          pinMode(PIN_IN1, OUTPUT);
          pinMode(PIN_IN2, OUTPUT);
          pinMode(PIN_IN3, OUTPUT);
          pinMode(PIN_IN4, OUTPUT);
        pinMode(Laser, OUTPUT);
        pinMode(LED, OUTPUT);
          // Команда остановки двум моторам
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
        digitalWrite(Laser, LOW);
        digitalWrite(LED, LOW);
  
        Servo1.write(0);
        Servo2.write(0);
        delay(3000);
}
void loop() {
    if(Bluetooth.available()>0)
     {
     String value = Bluetooth.readStringUntil('#');
              
    if(value.length()==7)
              {
              String Sangle = value.substring(0, 3);
              String Sstrength = value.substring(3, 6);
              String Sbutton = value.substring(6, 8);
              angle = Sangle.toInt();
              strength = Sstrength.toInt();
              button = Sbutton.toInt();
         if (button == 1 || mode == 1)
              {
                ServoUpdate();
                mode = 1;
            digitalWrite(LED, LOW);
            Servo1.write(0); //убираем "ствол"
                
                if(angle <=180){
                  analogWrite(PIN_ENA, 1.275*strength*(1+cos(angle/56.0))); // Устанавливаем скорость 1-го мотора
                  analogWrite(PIN_ENB, 1.275*strength*(1-cos(angle/56.0))); // Устанавливаем скорость 2-го мотора
                  // Задаём направление для 1-го мотора
                  digitalWrite(PIN_IN1, HIGH);
                  digitalWrite(PIN_IN2, LOW);
                  // Задаём направление для 2-го мотора
                  digitalWrite(PIN_IN3, HIGH);
                  digitalWrite(PIN_IN4, LOW);
                }
                else{
                  analogWrite(PIN_ENA, 1.275*strength*(1+cos(angle/56))); // Устанавливаем скорость 1-го мотора
                  analogWrite(PIN_ENB, 1.275*strength*(1-cos(angle/56))); // Устанавливаем скорость 2-го мотора
                  // Задаём направление для 1-го мотора
                  digitalWrite(PIN_IN1, LOW);
                  digitalWrite(PIN_IN2, HIGH);
                  // Задаём направление для 2-го мотора
                  digitalWrite(PIN_IN3, LOW);
                  digitalWrite(PIN_IN4, HIGH);
                }
              }
                      if (button == 2 || mode == 2)
                      { 
                        ServoUpdate();
                        mode = 2;
                        digitalWrite(LED, HIGH);// бошку зажигаем
                        s1speed = 2.55*strength*sin(angle/56.0);// считаем скорость вращения серв
                        s2speed = 2.55*strength*cos(angle/56.0);
                        s1 += s1speed/100;
                        s2 += s2speed/100;
                        Servo1.write(s1);
                        Servo1.writeMicroseconds(5); 
                        Servo2.write(s2);
                        Servo2.writeMicroseconds(5);
                         // вот тут хз, мб будет пропускать пакет
                             if (button == 3)
                             {
                              digitalWrite(Laser, HIGH);
                                for(int i = 5000; i>=100; i-=20){
                                  tone(BEEP_PIN, i, 5);
                                  delay(BEEP_PIN);
                                }
                                noTone(3);
                              //delay(500);
                              digitalWrite(Laser, LOW);
                             }
                      }

              
Serial.print("angle: ");Serial.print(angle);Serial.print('\t');
   Serial.print("strength: ");Serial.print(strength);Serial.print('\t');
  Serial.print("button: ");Serial.print(button);Serial.println("");
   Serial.flush();
              value="";
              }
              }
}
