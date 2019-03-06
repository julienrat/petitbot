/*
 * Petitbot ESP32
 *
 * Un programme pedagogique des petits debrouillards ?=+ pour gerer le robot "Petit Bot" 
 * Voir sur http://wikidebrouillard.org/index.php?title=Petit_Bot_un_robot_controlable_en_Wifi
 * Ce programme est inspire de : http://www.esp8266.com/viewtopic.php?f=29&t=6419#sthash.gd1tJhwU.dpuf
 * Sous licence CC-By-Sa
 * Par des gens bien
 */

#include <ArduinoJson.h>
#include <FS.h>
#include <Ticker.h>
#include <ESP32Servo.h>
#include <Ultrasonic.h>

#define SERVO_MOTEUR_FREQUENCE 50 // 50hz de fréquence

Ticker stopper; // Horloge

// Configuration
const char* version = "VESP32";

// Declaration de variables
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

void setup() {
  /*
   * Mise en place des variables et routes HTTP pour le ptitbot.
   */
  Serial.begin(115200);
  Serial.setDebugOutput(false);
    
  // Initialisation des broches led
  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);

  // On règle la fréquence des moteurs, ne pas dépasser 50hz
  servo1.setPeriodHertz(SERVO_MOTEUR_FREQUENCE);
  servo2.setPeriodHertz(SERVO_MOTEUR_FREQUENCE);
  servo3.setPeriodHertz(SERVO_MOTEUR_FREQUENCE);
  servoG.setPeriodHertz(SERVO_MOTEUR_FREQUENCE);
  servoD.setPeriodHertz(SERVO_MOTEUR_FREQUENCE);

  // Initialisation des servo
  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
  servo3.attach(pin_servo3);

  // Initialisation servo Gauche et Droite
  servoG.attach(pin_servoG);
  servoD.attach(pin_servoD);
}

void loop() {
  /*
   * Fonction loop qui va être executé continuellement par l'arduino 
   */
    Serial.println("Hello, world!");
}

void arret() {
  /*
   * Fonction pour arrêter les servomoteurs Gauche et Droite de l'arduino
   */
  servoG.attach(pin_servoG);
  servoD.attach(pin_servoD);

  servoD.write(90);
  servoG.write(90);

  stopper.detach();
  Serial.println("STOP");
}
