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
char modo = "R";


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
  Serial.println(" para fazer teste do hardware [T]\n para receber os valores da leitura [A]");

}
void mensagem() {
  Serial.println("--------------------------");
  Serial.println("(S) mais o numero para mover o servo (s90)\n M fazer os motores roda de -250 a 250 (M150)");
  Serial.println(" Md para dora o motor direito -250 a 250 (Md150)\n Me para rodar o motor esquerdo -250 a 250(Me150)");
  Serial.println(" L para deltura do hc (L1) uma leitura (Ll) leitura loop");
}
void teste() {
  String entrada;
  mensagem();
  while (true) {
    if (Serial.available() > 0) {
      entrada = Serial.readStringUntil('\n');

      Serial.println("--------------------------");


      if (entrada[0] == 'S' && entrada.length() > 1) {

        entrada = entrada.substring(1);
        int numero = entrada.toInt();
        if (numero > 180 || numero < 0) {
          numero = -numero;
          Serial.println("Erro o rada vai de 0 a 180 apenas");
        } else {
          Serial.print("Indo para a rotacao ");
          Serial.println(numero);
          radar.write(numero);
        }
      } else if (entrada[0] == 'L' && entrada.length() > 1) {

        if (entrada[1] == 'l') {
          while (entrada != "p") {
            if (Serial.available() > 0) {
              entrada = Serial.readStringUntil('\n');
            }
            leitura(0);
            Serial.println(distancia_obistaculos[0]);
            Serial.println("--------------------------");
            Serial.println("digite p para pára ");
            Serial.println("--------------------------");
            delay(200);
          }
        }
        entrada = entrada.substring(1);
        int numero = entrada.toInt();

        for (int i = 0; i < numero; i++) {
          leitura(0);
          Serial.print(i);
          Serial.print(" ");

          Serial.println(distancia_obistaculos[0]);
          Serial.println("--------------------------");
        }
      } else if (entrada[0] == 'M' && entrada[1] == 'e' && entrada.length() > 2) {
        entrada = entrada.substring(2);
        int numero = entrada.toInt();

        if (numero > 0) {
          analogWrite(motorDIR_1, LOW);
          analogWrite(motorDIR_2, numero);

          Serial.println("roda esqueda indo para frente a ");
          Serial.println(numero);
        } else if (numero < 0) {
          numero = -numero;

          analogWrite(motorESQ_1, numero);
          analogWrite(motorESQ_2, LOW);

          Serial.println("roda esqueda indo para traz a ");
          Serial.println(numero);
        } else {
          analogWrite(motorESQ_1, LOW);
          analogWrite(motorESQ_2, LOW);

          analogWrite(motorDIR_1, LOW);
          analogWrite(motorDIR_2, LOW);
        }

      } else if (entrada[0] == 'M' && entrada[1] == 'd' && entrada.length() > 2) {
        entrada = entrada.substring(2);
        int numero = entrada.toInt();
        if (numero > 0) {

          analogWrite(motorDIR_1, LOW);
          analogWrite(motorDIR_2, numero);
          Serial.println("roda direita indo para frente a ");
          Serial.println(numero);

        } else if (numero < 0) {
          numero = -numero;

          analogWrite(motorDIR_1, numero);
          analogWrite(motorDIR_2, LOW);
          Serial.println("roda direita indo para traz a ");
          Serial.println(numero);
        } else {
          analogWrite(motorESQ_1, LOW);
          analogWrite(motorESQ_2, LOW);

          analogWrite(motorDIR_1, LOW);
          analogWrite(motorDIR_2, LOW);
        }

      } else if (entrada[0] == 'M' && entrada.length() > 1) {
        entrada = entrada.substring(1);
        int numero = entrada.toInt();

        if (numero > 0) {
          analogWrite(motorESQ_1, LOW);
          analogWrite(motorESQ_2, numero);

          analogWrite(motorDIR_1, LOW);
          analogWrite(motorDIR_2, numero);

          Serial.println("rodas indo para frente a ");
          Serial.println(numero);
        } else if (numero < 0) {
          numero = -numero;

          analogWrite(motorESQ_1, numero);
          analogWrite(motorESQ_2, LOW);

          analogWrite(motorDIR_1, numero);
          analogWrite(motorDIR_2, LOW);

          Serial.println("rodas indo para traz a ");
          Serial.println(numero);
        } else {
          analogWrite(motorESQ_1, LOW);
          analogWrite(motorESQ_2, LOW);

          analogWrite(motorDIR_1, LOW);
          analogWrite(motorDIR_2, LOW);
        }
      } else {
        mensagem();
      }
    }
  }
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

void sona() {
  for (int i = 0; i < 9; i++) {
    radar.write(lista[i]);
    delay(60);
    leitura(i);
    if (i == 1 && radar.read() > 50){
      analogWrite(motorESQ_1, 255);
      analogWrite(motorESQ_2, LOW);

      analogWrite(motorDIR_1, 255);
      analogWrite(motorDIR_2, LOW);
      delay(200);
    }else if (i == 8 && radar.read() < 130){
      analogWrite(motorESQ_1, 255);
      analogWrite(motorESQ_2, LOW);

      analogWrite(motorDIR_1, 255);
      analogWrite(motorDIR_2, LOW);
      delay(200);
    }
  }
}

void algoritimo() {
  byte distanciaMinima = 25;
  // mostra os dados da leitura no monitor serial
  if (modo == 'A') {
    Serial.print("dados da leitura ");

    for (int i = 0; i < 7; i++) {
      Serial.print(distancia_obistaculos[i]);
      Serial.print("  ");
    }
    Serial.println("  ");
  }



  if (distancia_obistaculos[4] < distanciaMinima) {  // Ser tive um obistaculo na frente

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
    movimentar(90);
  } else if (distancia_obistaculos[8] < distanciaMinima) {  // leve correcao para esquerda
    movimentar(-90);
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

    delay(-espera);         // inverte de negativo para positivo e espera
  } else if (espera > 0) {  // direita
    analogWrite(motorESQ_1, LOW);
    analogWrite(motorESQ_2, motor_a);

    analogWrite(motorDIR_1, motor_b);
    analogWrite(motorDIR_2, LOW);

    delay(espera);
  }

  // padrao ele sempre vai para frente
  analogWrite(motorESQ_1, LOW);
  analogWrite(motorESQ_2, motor_a);

  analogWrite(motorDIR_1, LOW);
  analogWrite(motorDIR_2, motor_b);
}

void loop() {

  if (Serial.available() > 0) {

    modo = Serial.readStringUntil('\n')[0];
    if (modo == 'T') {
      teste();
    }
  }

  sona();
  algoritimo();
  movimentar(0);

  }
