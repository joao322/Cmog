#include <Servo.h>

#define motorESQ_1 6   // Pino N1 do motor esquerdo
#define motorESQ_2 11  // Pino N2 do motor esquerdo
#define motorDIR_1 3   // Pino N3 do motor direito
#define motorDIR_2 5   // Pino N4 do motor direito

#define led_info 2
#define leitorbateria A0

#define echo 12
#define trig 13

Servo servo;

void setup() {
  servo.attach(4);
  Serial.begin(9600);

  pinMode(motorESQ_1, OUTPUT);
  pinMode(motorESQ_2, OUTPUT);

  pinMode(motorDIR_1, OUTPUT);
  pinMode(motorDIR_2, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

  servo.write(0);
}
void assincrono(int espera){
  // tempo minimo e de 20 ms
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);

  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long m = pulseIn(echo, HIGH, 20000);
  Serial.println(m/58);
}



void loop() {
 assincrono(2);

}
