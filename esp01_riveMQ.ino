#include <ESP8266WiFi.h>
#include <WiFiManager.h>  // Biblioteca WiFiManager
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// Definições dos pinos
#define latchPin 2 // ST_CP - Latch
#define clockPin 0 // SH_CP - Clock
#define dataPin 1  // DS - Data

byte saida1 = 0x00; // Inicialização das saídas do shift register

const char* mqttServer = "hivemq.cloud"; // URL do broker MQTT
const int mqttPort = 8883; // Porta SSL para o HiveMQ Cloud
const char* mqttUser = "user"; // Substitua pelo seu usuário do HiveMQ
const char* mqttPassword = "password";

WiFiClientSecure wifiClient; // Cliente Wi-Fi seguro
PubSubClient mqttClient(wifiClient); // Cliente MQTT baseado no cliente Wi-Fi seguro

unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);

  // Configuração do WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP_ConfigPortal"); // Nome do portal de configuração
  Serial.println("WiFi conectado!");

  // Configura cliente MQTT
  wifiClient.setInsecure(); // Ignora verificação do certificado SSL
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);

  // Conecta ao broker MQTT
  connectToMQTT();

  // Configura pinos
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Configuração do LED interno
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Apaga o LED inicialmente
}

void loop() {
  // Verifica conexão MQTT
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();

  // Publica o RSSI periodicamente
  if (millis() - lastTime > 2500) {
    lastTime = millis();
    String rssi = "RSSI: " + String(WiFi.RSSI());
    mqttClient.publish("saida/rssi", rssi.c_str());
  }
}

void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT... ");
    digitalWrite(LED_BUILTIN, LOW); // Liga o LED para indicar tentativa de conexão ao MQTT
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH); // Apaga o LED durante a espera
    delay(1000);

    if (mqttClient.connect("ESP01_Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao broker MQTT!");
      digitalWrite(LED_BUILTIN, HIGH); // Apaga o LED ao conectar
      mqttClient.subscribe("saida/update");
      mqttClient.subscribe("saida/request");
      publishState();
    }
  }
}
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (String(topic) == "saida/update") { // Atualiza as saídas
    if (!error) {
      byte newSaida = 0x00;
      for (int i = 0; i < 8; i++) {
        if (doc[String(i)] == 1) {
          newSaida |= (1 << i); // Define o bit
        }
      }

      if (newSaida != saida1) {
        saida1 = newSaida;
        updateShiftRegister(); // Atualiza o shift register se houve alteração
        publishState(); // Publica estado atualizado
      }
    }
  } else if (String(topic) == "saida/request") { // Solicitação de status
    publishState(); // Envia o status das saídas quando solicitado
  }
}

void publishState() {
  StaticJsonDocument<200> doc;
  for (int i = 0; i < 8; i++) {
    doc[String(i)] = (saida1 >> i) & 1; // Adiciona o estado atual das saídas
  }
  doc["RSSI"] = WiFi.RSSI(); // Adiciona a força do sinal Wi-Fi

  String jsonString;
  serializeJson(doc, jsonString);

  mqttClient.publish("saida/state", jsonString.c_str()); // Publica o estado no tópico MQTT
}

void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, saida1);
  digitalWrite(latchPin, HIGH);
}
