#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>
#include <Ultrasonic.h>
#include <Ticker.h>

// Variables globales
// Pour éviter les erreurs de compilation ne pas les déclarer dans les fichiers annexes

// Portail captif
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

#define SERVO_MOTEUR_FREQUENCE 50 // 50hz de fréquence

const char* hostAP = "PetitBot"; // nom du point d'acces petitBot
static int restartNow = false;
const char* version = "VESP32";

String name ;
String invert_motors ;
String access_point;
String ssid ;
String mdp ;
String canal ;
String ip ;

Servo servo1;
Servo servo2;
Servo servo3;

Servo servoG;
Servo servoD;

// Pin recommandés sont: 2,4,12-19,21-23,25-27,32-33 
const int pin_led1 = 32; // pin led 1
const int pin_led2 = 33; // pin led 2

int pin_servoG = 21; // pin servo gauche
int pin_servoD = 22; // pin servo droit

const int pin_servo1 = 15; // pin servo 1
const int pin_servo2 = 16; // pin servo 2
const int pin_servo3 = 17; // pin servo 3

const int pin_dist1 = 18; // pin capteur de distance 1
const int pin_dist2 = 19; // pin capteur de distance 2


// Permet d'executer du code dans un interval de temps
Ticker stopper;

// Capteur ultrason
Ultrasonic Ultrasonic1(pin_dist1, pin_dist2);

// SKETCH BEGIN
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


// Ce code s'execute une fois au demarrage
void setup() {
  debugger_setup ();
  // from config
  config_setup();
  // from wifi_handler
  wifi_setup();
  servo_setup();
  server_setup();
}

// Ce code s'execute en boucle
void loop() {
  dnsServer.processNextRequest(); //demarrage ecoute du portail captif
  ArduinoOTA.handle(); 
  if (restartNow) {
    Serial.println("Restart");
    ESP.restart();
  }
}

void debugger_setup () {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
}