#ifndef CONFIG_H
#define CONFIG_H
const char* ssid = "Nombre de la Red WIFI";
const char* password = "Password WiFi";
const char* mqtt_server = "direccion del servidor mqtt";
const char* mqttUser = "mqtt user";
const char* mqttPassword = "Mqtt password";
const char* topic = "/casa/planta_baja/riego";
const char* status = "/casa/planta_baja/riego/status";
unsigned long tiempoSegundos = 240;
unsigned long timer;
#endif
