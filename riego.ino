#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "user_interface.h"
#include "config.h"

const short int BUILTIN_LED1 = 2;
 
WiFiClient espClient;
PubSubClient client(espClient);

const byte ledPin = 4; // Pin with RELE

void setup() {
  Serial.begin(115200);
  delay(10);
   wifi_set_phy_mode(PHY_MODE_11B);
  pinMode(ledPin, OUTPUT);
  pinMode(BUILTIN_LED1, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);  
  digitalWrite(ledPin, HIGH);
  
  WiFi.setOutputPower(20.5);
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  wifi_set_sleep_type(NONE_SLEEP_T);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);  
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  digitalWrite(LED_BUILTIN, LOW);  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("Riego", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe(topic);
 
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  Serial.println((char)payload[0]);
  ((char)payload[0] =='1') ? turnOn() : turnOff();
 
  Serial.println();
  Serial.println("-----------------------");
 
}
void turnOn() { 
  Serial.println("Salida en LOW");
  timer = millis();
  digitalWrite(ledPin, LOW);
}
void turnOff() {
  Serial.println("Salida en HIGH");
  timer = NULL;
  digitalWrite(ledPin, HIGH);  
}

void loop() {
  if (timer){
    if (millis() - timer > tiempoSegundos * 1000 ) {
       timer = NULL;
       turnOff();
       client.publish(topic,"0");
       char * cadena;
       strcat(cadena,topic);
       strcat(cadena,"/status");
       client.publish(cadena ,"0");
    }
  }
  client.loop();
}
