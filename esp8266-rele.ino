#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);

const byte rele = 3; // Pin with RELE

void initWifi()
{
  WiFi.begin(ssid, password);
  WiFi.setOutputPower(20.5);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  digitalWrite(LED_BUILTIN, LOW);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("Riego", mqttUser, mqttPassword))
    {

      Serial.println("connected");
    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(topic);
}

void configureWifi()
{
  wifi_set_phy_mode(PHY_MODE_11B);
  WiFi.setOutputPower(20.5);
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP)
      wifi_set_sleep_type(NONE_SLEEP_T);
}

void configureOutPuts()
{
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}
void setup()
{
  configureOutPuts();
  Serial.begin(115200);
  configureWifi();
  initWifi();
}

void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  Serial.println((char)payload[0]);
  ((char)payload[0] == '1') ? turnOn() : turnOff();

  Serial.println();
  Serial.println("-----------------------");
}
void turnOn()
{
  Serial.println("Salida en LOW");
  timer = millis();
  digitalWrite(rele, LOW);
}
void turnOff()
{
  Serial.println("Salida en HIGH");
  timer = NULL;
  digitalWrite(rele, HIGH);
}

void loop()
{
  if (timer)
  {
    if (millis() - timer > tiempoSegundos * 1000)
    {
      timer = NULL;
      turnOff();
      verifyWifi();
      client.publish(topic, "0");
      client.publish(status, "0");
    }
  }
  else
  {
    verifyWifi();
  }
  client.loop();
}

void verifyWifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    initWifi();
  }
}