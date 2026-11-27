//MUDAR A LINHA 149 COM O NOME DO DISPOSITIVO AQUI E NO MIT APP INVENTOR.
// O CÓDIGO ABAIXO RECEBE "0" - PARA APAGAR O LED ONBOERD (PINO 8) OU "1" - PARA ACENDER O LED onboard usando Bluetooth BLE esp32c3 supermini. Necessário o aplicativo de celular para conectar ao esp.
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Wire.h> //Biblioteca utilizada gerenciar a comunicação entre dispositivos através do protocolo I2C
#include <LiquidCrystal_I2C.h> //Biblioteca controlar display 16x2 através do I2C

LiquidCrystal_I2C lcd(0x3F,16,2);

// ======================================================
// LED
// ======================================================

//#define LED_PIN 8

//#define PINO_MOTORH 5
//#define PINO_MOTORA 6


#define LED_ON  LOW
#define LED_OFF HIGH




// ======================================================
// UUIDs BLE
// ======================================================

#define SERVICE_UUID \
"4fafc201-1fb5-459e-8fcc-c5c9c331914b"

#define CHARACTERISTIC_UUID \
"beb5483e-36e1-4688-b7f5-ea07361b26a8"


// ======================================================
// VARIÁVEIS
// ======================================================

BLECharacteristic *pCharacteristic;

bool dispositivoConectado = false;


// ======================================================
// CALLBACK DA CARACTERÍSTICA
// ======================================================

class MyCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) override {

    String valor = pCharacteristic->getValue().c_str();

    Serial.println();
    Serial.println("------------------------------");
    Serial.print("Dado recebido: [");
    Serial.print(valor);
    Serial.println("]");
    if (valor == "1") {

      //digitalWrite(LED_PIN, LED_ON);
      analogWrite(5, 125);
      analogWrite(6, 0);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GIRANDO HORAR");

      Serial.println("Comando: LIGAR LED");
      Serial.println("LED: LIGADO");
    }

    else if (valor == "0") {

      //digitalWrite(LED_PIN, LED_OFF);
      analogWrite(5, 0);
      analogWrite(6, 125);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GIRANDO ANTI");

      Serial.println("Comando: DESLIGAR LED");
      Serial.println("LED: DESLIGADO");
    }
    else if (valor == "2") {

      //digitalWrite(LED_PIN, LED_OFF);
      analogWrite(5, 0);
      analogWrite(6, 0);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PARADO");

      Serial.println("Comando: PARAR MOTOR");
      Serial.println("LED: DESLIGADO");
    }

    else {

      Serial.println("Comando desconhecido!");
    }

    Serial.println("------------------------------");
  }
};


// ======================================================
// CALLBACK DO SERVIDOR
// ======================================================

class MyServerCallbacks : public BLEServerCallbacks {

  void onConnect(BLEServer *pServer) override {

    dispositivoConectado = true;

    Serial.println();
    Serial.println("==============================");
    Serial.println("CELULAR CONECTADO!");
    Serial.println("==============================");
  }


  void onDisconnect(BLEServer *pServer) override {

    dispositivoConectado = false;

    Serial.println();
    Serial.println("==============================");
    Serial.println("CELULAR DESCONECTADO!");
    Serial.println("==============================");

    delay(500);

    pServer->startAdvertising();

    Serial.println("Advertising reiniciado.");
    Serial.println("Aguardando nova conexao...");
  }
};


// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);

  delay(1500);

  Serial.println();
  Serial.println("================================");
  Serial.println(" ESP32-C3 - TESTE BLE");
  Serial.println("================================");

  lcd.init();
  lcd.clear();
  lcd.backlight();


  // ====================================================
  // LED
  // ====================================================

  //pinMode(LED_PIN, OUTPUT);

  //digitalWrite(LED_PIN, LED_OFF);

  //Serial.println("LED configurado no GPIO 8");


  // ====================================================
  // INICIALIZA BLE
  // ====================================================

  Serial.println();
  Serial.println("Inicializando BLE...");

  BLEDevice::init("skz32"); //ATENÇÃO !!!!!!!!!!!!!!!!!!!!!!!!!!!   MUDAR PARA CADA ESP

  Serial.println("BLE inicializado");


  // ====================================================
  // MOSTRA ENDERECO BLE
  // ====================================================

  Serial.print("Endereco BLE: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());


  // ====================================================
  // SERVIDOR
  // ====================================================

  BLEServer *pServer = BLEDevice::createServer();

  pServer->setCallbacks(new MyServerCallbacks());

  Serial.println("Servidor BLE criado");


  // ====================================================
  // SERVICO
  // ====================================================

  BLEService *pService =
    pServer->createService(SERVICE_UUID);

  Serial.println("Servico BLE criado");


  // ====================================================
  // CARACTERISTICA
  // ====================================================

  pCharacteristic =
    pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_WRITE_NR
    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  Serial.println("Caracteristica BLE criada");


  // ====================================================
  // INICIA SERVICO
  // ====================================================

  pService->start();

  Serial.println("Servico iniciado");


  // ====================================================
  // ADVERTISING
  // ====================================================

  Serial.println();
  Serial.println("Configurando advertising...");

  BLEAdvertising *pAdvertising =
    pServer->getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);

  pAdvertising->setScanResponse(true);

  Serial.println("Service UUID adicionado ao advertising");

  Serial.println("Iniciando advertising...");

  pServer->startAdvertising();

  Serial.println("Advertising iniciado");


  // ====================================================
  // INFORMAÇÕES FINAIS
  // ====================================================

  Serial.println();
  Serial.println("================================");
  Serial.println(" BLE PRONTO!");
  Serial.println("================================");

  Serial.println("Nome: ESP32C3_LED");

  Serial.print("Servico: ");
  Serial.println(SERVICE_UUID);

  Serial.print("Caracteristica: ");
  Serial.println(CHARACTERISTIC_UUID);

  Serial.println();
  Serial.println("Aguardando celular...");
  Serial.println();
}


// ======================================================
// LOOP
// ======================================================

void loop() {

  delay(100);
}
