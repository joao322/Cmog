#include <Server.h>

#define motorESQ_1 6   // Pino N1 do motor esquerdo
#define motorESQ_2 11  // Pino N2 do motor esquerdo
#define motorDIR_1 3   // Pino N3 do motor direito
#define motorDIR_2 5   // Pino N4 do motor direito

#define echo 12
#define trig 13

Servo servo;

void setup() {
  servo.attach(4);

  pinMode(motorESQ_1, OUTPUT);
  pinMode(motorESQ_2, OUTPUT);

  pinMode(motorDIR_1, OUTPUT);
  pinMode(motorDIR_2, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

}
void t(){
  


}


void loop() {
  // put your main code here, to run repeatedly:

}
