#include <Stepper.h>
const int passosporvolta = 2048;
Stepper meuMotor (passosporvolta, 5, 7, 6, 8);
//int pinoMotorHorario = 5;
//int pinoMotorAnti = 6;
int pinoBotao = 3;
int pinoPot = 4;

void setup() {
  Serial.begin(115200);
  //pinMode(pinoMotorHorario, OUTPUT);
  //pinMode(pinoMotorAnti, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);
  pinMode(pinoPot, INPUT);
  
}
int estado = 0;

void loop() {
  int valor_lido = analogRead(pinoPot);
  int velocidade = map(valor_lido,1,4096,1,255);
  meuMotor.setSpeed(velocidade);
  int botao = digitalRead(pinoBotao);
  Serial.println(velocidade);
  switch(estado){
    case 0:
    //analogWrite(pinoMotorHorario, velocidade);
    //analogWrite(pinoMotorAnti, 0);
    meuMotor.step(10);

      if(botao == 0){
        Serial.println("case 0");
        estado = 1;
        delay(500);
        
      }
      break;
    case 1:
        Serial.println("case 1");
      if(botao == 1){
        estado = 2;
        delay(500);
      }
      break;
    case 2:
        //analogWrite(pinoMotorHorario, 0);
        //analogWrite(pinoMotorAnti, 0);
        meuMotor.step(1);

        Serial.println("case 2");
     if(botao == 0){
        estado = 3;
        delay(500);
      }
      break;
    case 3:
      Serial.println("case 3");
      if(botao == 1){
        estado = 4;
        delay(500);
      }
      break;
    case 4:
      //analogWrite(pinoMotorHorario, 0);
      //analogWrite(pinoMotorAnti, velocidade);
      meuMotor.step(-10);

      Serial.println("case 4");
      if (botao == 0){
        estado = 5;
        delay(500);
      }
      break;
    case 5:
      //analogWrite(pinoMotorHorario, 0);
      //analogWrite(pinoMotorAnti, 0);
      meuMotor.step(1);

      Serial.println("case 5");
      if (botao == 1){
        estado = 6;
        delay(500);
      }
      break;
    case 6:
    Serial.println("case 6");
    if(botao == 0){
      estado = 0;
      delay(500);
    }
    break;
  }

}
