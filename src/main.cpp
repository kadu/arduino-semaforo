#include <Arduino.h>
#include "OneButton.h"

enum estados {
  SINAL_VERMELHOCURTO = 1,
  SINAL_VERMELHOLONGO = 2,
  SINAL_AMARELO = 3,
  SINAL_VERDE =4
};

#define sCarroVermelho    13
#define sCarroAmarelo     12
#define sCarroVerde       11
#define sPedestreVerde    10
#define sPedestreVermelho 9
#define botao             7   //se voce estiver usando arduino UNO, mudar para 3

#define TEMPO_VERMELHO 5000
#define TEMPO_AMARELO  3000
#define TEMPO_VERDE    2000

int estadoAtual;
int proximoEstado;
unsigned long esperaAte = 0;
unsigned long piscaAmarelo;

volatile bool botaoPedestrePressionado = false;
OneButton button(botao, true);

void pedestreChamando() {  
  botaoPedestrePressionado = true;
  Serial.println("pedestreChamando");
}



void setup() {
  pinMode(sCarroVermelho, OUTPUT);
  pinMode(sCarroAmarelo, OUTPUT);
  pinMode(sCarroVerde, OUTPUT);
  pinMode(sPedestreVermelho, OUTPUT);
  pinMode(sPedestreVerde, OUTPUT);  
  // pinMode(botao, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(botao), pedestreChamando, RISING);
  

  Serial.begin(115200);
  
  Serial.print("Starting");
  for (size_t i = 0; i < 10; i++) {
    Serial.print(".");
    delay(50);
  }
  
  botaoPedestrePressionado = false;
  estadoAtual = SINAL_VERMELHOCURTO;
  button.attachClick(pedestreChamando);

  Serial.println("\nReady!");
}

void acende(enum estados COR) {
  digitalWrite(sCarroVermelho    , LOW);
  digitalWrite(sCarroAmarelo     , LOW);
  digitalWrite(sCarroVerde       , LOW);
  digitalWrite(sPedestreVermelho , LOW);
  digitalWrite(sPedestreVerde    , LOW);

  switch (COR)
  {
    case SINAL_VERMELHOLONGO:
    case SINAL_VERMELHOCURTO:
      digitalWrite(sCarroVermelho, HIGH);
      digitalWrite(sPedestreVerde, HIGH);
      break;

    case SINAL_AMARELO:
      digitalWrite(sCarroAmarelo, HIGH);
      digitalWrite(sPedestreVermelho, HIGH);
      break;

    case SINAL_VERDE:
      digitalWrite(sCarroVerde, HIGH);
      digitalWrite(sPedestreVermelho, HIGH);
      break;
  }
}

void loop() {
  button.tick();
  if (millis() > esperaAte ) {
    Serial.print("Debug: ");
    Serial.println(botaoPedestrePressionado);
    switch (estadoAtual) {
      case SINAL_VERMELHOCURTO:
        acende(SINAL_VERMELHOCURTO);
        esperaAte = millis() + TEMPO_VERMELHO;
        estadoAtual = SINAL_VERDE;
        Serial.println("Estou no vermelho curto");
        break;
      case SINAL_VERMELHOLONGO:
        acende(SINAL_VERMELHOLONGO);
        esperaAte = millis() + TEMPO_VERMELHO*2;
        estadoAtual = SINAL_VERDE;
        Serial.println("Estou no vermelho longo");
        botaoPedestrePressionado = false;
        break;
      case SINAL_AMARELO:
        acende(SINAL_AMARELO);
        esperaAte = millis() + TEMPO_AMARELO;                
        estadoAtual = !botaoPedestrePressionado ? SINAL_VERMELHOCURTO: SINAL_VERMELHOLONGO;
        Serial.println("Estou no amarelo");
        break;
      case SINAL_VERDE:
        acende(SINAL_VERDE);
        esperaAte = millis() + TEMPO_VERDE;
        estadoAtual = SINAL_AMARELO;
        Serial.println("Estou no verde");
        break;
    }
  }

  // if(estadoAtual == SINAL_VERDE) {
  //   if(millis() > (esperaAte - TEMPO_AMARELO)) {
  //     digitalWrite(sPedestreVermelho, HIGH);
  //     delay(500);
  //     digitalWrite(sPedestreVermelho, LOW);
  //     delay(500);
  //   }
  // }

}