#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Preferences.h>

#define PINO_BOTAO 2
#define PINO_LED 8

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);

Preferences memoriaNVS;

volatile int flagBotaoPressionado = 0;
volatile int flagBotaoSolto = 0;
int estado = 0;
unsigned long tempoAnterior = 0;

int contador = 0;
int atualizarTela = 1;

void IRAM_ATTR isrBotao() {
  if (digitalRead(PINO_BOTAO) == LOW) {
    flagBotaoPressionado = 1;
  } else {
    flagBotaoSolto = 1;
  }
}

void gravaContadorNaFlash() {
  memoriaNVS.begin("armazenamento", false);
  memoriaNVS.putInt("contagem", contador);
  memoriaNVS.end();

  Serial.print("Valor ");
  Serial.print(contador);
  Serial.println(" gravado na memoria");

  digitalWrite(PINO_LED, LOW);
}

void executaMaquinaDeEstados() {
  if (estado == 0) {
    if (flagBotaoPressionado == 1) {
      tempoAnterior = millis();
      estado = 1;
    }
  } else if (estado == 1) {
    if (millis() - tempoAnterior >= 50) {
      if (digitalRead(PINO_BOTAO) == LOW) {
        digitalWrite(PINO_LED, HIGH);
        estado = 2;
      } else {
        flagBotaoPressionado = 0;
        estado = 0;
      }
    }
  } else if (estado == 2) {
    contador++;
    if (contador > 9) {
      contador = 0;
    }

    Serial.print("apertados: ");
    Serial.println(contador);

    atualizarTela = 1;
    flagBotaoSolto = 0;
    estado = 3;
  } else if (estado == 3) {
    if (flagBotaoSolto == 1) {
      tempoAnterior = millis();
      estado = 4;
    }
  } else if (estado == 4) {
    if (millis() - tempoAnterior >= 50) {
      if (digitalRead(PINO_BOTAO) == HIGH) {
        gravaContadorNaFlash();

        flagBotaoPressionado = 0;
        flagBotaoSolto = 0;
        estado = 0;
      } else {
        flagBotaoSolto = 0;
        estado = 3;
      }
    }
  }
}

void gerenciaDisplay() {
  if (atualizarTela == 1) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);

    u8g2.drawStr(2, 12, "Contador:");

    String numStr = String(contador);
    u8g2.drawStr(2, 30, numStr.c_str());

    u8g2.sendBuffer();

    atualizarTela = 0;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, HIGH);

  u8g2.begin();

  memoriaNVS.begin("armazenamento", false);
  contador = memoriaNVS.getInt("contagem", 0);
  memoriaNVS.end();

  Serial.print("sistema iniciado, valor lido na memória: ");
  Serial.println(contador);

  attachInterrupt(digitalPinToInterrupt(PINO_BOTAO), isrBotao, CHANGE);
}

void loop() {
  executaMaquinaDeEstados();
  gerenciaDisplay();
}
