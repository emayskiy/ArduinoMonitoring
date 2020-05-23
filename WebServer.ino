/*
  Контроль темепературы в серверной
*/

#include <SPI.h>
#include <Ethernet2.h>
#include "DHT.h" // Работа с температурой
#include <ArduinoJson.h>

#define DHTPIN 4          // Номер пина, к которому подсоединен датчик температуры
#define DHTTYPE DHT11     // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE); // Инициализация сенсора DHT

// MAC-Адрес устройства
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// IP-Адрес устройства
IPAddress ip(192, 168, 1, 199);

// Инициализация Веб-сервера
EthernetServer server(80);

void setup() {
  // Откроем порт и дождемся инициализации
  Serial.begin(9600);
  
  //Инициализация датчика влажности
  dht.begin();
  
  // Инициализация Ethernet сервера
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server IP: ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Ждем подключения клиента
  EthernetClient client = server.available();
  if (!client) return;

  Serial.println(F("Клиент подключен"));

  // Read the request (we ignore the content in this example)
  while (client.available()) client.read();

  // Allocate a temporary JsonDocument
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<500> doc;

  // Создаем массив значений
  JsonArray sensorValues = doc.createNestedArray("sensors");
  
  //Добавлением значение сенсора
  JsonObject nested = sensorValues.createNestedObject();
  nested["SensorID"]="sensor1";
  nested["SensorType"]="Temp";
  nested["Temperature"]=dht.readTemperature();
  nested["Humidity"]=dht.readHumidity();
  nested["HeatIndex"]=dht.computeHeatIndex();
  
  //Выводим результат клиенту
  Serial.print(F("Отправка: "));
  serializeJson(doc, Serial);
  Serial.println();

  client.println(F("HTTP/1.0 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection: close"));
  client.print(F("Content-Length: "));
  client.println(measureJsonPretty(doc));
  client.println();

  serializeJsonPretty(doc, client);

  client.stop();
}
