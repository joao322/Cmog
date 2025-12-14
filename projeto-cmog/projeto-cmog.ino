#include <Servo.h>

#define motorESQ_1 6   // Pino N1 do motor esquerdo
#define motorESQ_2 11  // Pino N2 do motor esquerdo
#define motorDIR_1 3   // Pino N3 do motor direito
#define motorDIR_2 5   // Pino N4 do motor direito

#define echo 12
#define trig 13

Servo radar;

byte poicao_inicial = 90;

int lista[9] = { 30, 45, 60, 75, 90, 105, 120, 135, 150 };
int distancia_obistaculos[9] = {};
char modo = "r";


void setup() {
  pinMode(motorESQ_1, OUTPUT);
  pinMode(motorESQ_2, OUTPUT);

  pinMode(motorDIR_1, OUTPUT);
  pinMode(motorDIR_2, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

  radar.attach(4);
  radar.write(poicao_inicial);

  Serial.begin(9600);
}

void leitura(int pos) {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH);
  distancia_obistaculos[pos] = duracao / 58;
}

void sona(byte minimo, byte maximo) {
  for (int i = 0; i < 9; i++) {
    radar.write(lista[i]);
    delay(60);
    leitura(i);
  }
}

void algoritimo() {
  byte distanciaMinima = 25;
  // mostra os dados da leitura no monitor serial

  Serial.print("dados da leitura ");

  for (int i = 0; i < 7; i++) {
    Serial.print(distancia_obistaculos[i]);
    Serial.print("  ");
  }
  Serial.println("  ");



  if (distancia_obistaculos[4] < distanciaMinima) {  // Ser tive um obistaculo na fente

    Serial.println("obistaculo a fente desviar para");

    if (distanciaMinima < distancia_obistaculos[3] && distanciaMinima < distancia_obistaculos[1]) {  //se a direita tive livre, ir para la
      movimentar(450);
    } else if (distanciaMinima < distancia_obistaculos[5] && distanciaMinima < distancia_obistaculos[7]) {  // se a esquerda tive live, ir para la
      movimentar(-450);
    } else {
      movimentar(1150);
    }
  } else if (distancia_obistaculos[2] < distanciaMinima || distancia_obistaculos[1] < distanciaMinima) {  // se tive obistaculo na direita ir para a esquerda
    movimentar(-250);
  } else if (distancia_obistaculos[6] < distanciaMinima || distancia_obistaculos[7] < distanciaMinima) {  // se tive obistaculo na esquerda ir para a direita
    movimentar(250);
  } else if (distancia_obistaculos[0] < distanciaMinima) {  // leve correcao para a direita
    movimentar(70);
  } else if (distancia_obistaculos[8] < distanciaMinima) {  // leve correcao para esquerda
    movimentar(-70);
  }
}

void movimentar(int espera) {
  int motor_a = 130, motor_b = 150;
  delay(40);
  radar.write(poicao_inicial);
  
  if (espera < 0) {  // esquerda
    analogWrite(motorESQ_1, motor_a);
    analogWrite(motorESQ_2, LOW);

    analogWrite(motorDIR_1, LOW);
    analogWrite(motorDIR_2, motor_b);
    Serial.println("esquerda");
    delay(-espera);         // inverte de negativo para positivo e espera
  } else if (espera > 0) {  // direita
    analogWrite(motorESQ_1, LOW);
    analogWrite(motorESQ_2, motor_a);

    analogWrite(motorDIR_1, motor_b);
    analogWrite(motorDIR_2, LOW);

    Serial.println("direita");
    delay(espera);
  }

  // padrao ele sempre vai para frente
  analogWrite(motorESQ_1, LOW);
  analogWrite(motorESQ_2, motor_a);

  analogWrite(motorDIR_1, LOW);
  analogWrite(motorDIR_2, motor_b);

  Serial.println("Indo em frente");
}

void loop() {
  sona(30, 140);
  algoritimo();
  movimentar(0);
}
