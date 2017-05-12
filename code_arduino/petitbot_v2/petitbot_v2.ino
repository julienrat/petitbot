/*
  Petitbot V2
  Requiert la bibliotheque ULTRASONIC disponible nativement dans arduino ou à cette adresse https://github.com/ErickSimoes/Ultrasonic

*/

#include <ESP8266WiFi.h>

//Bibliothèques Wifi
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80); //objet serveur
const char* hostAP = "PetitBot"; // nom du point d'acces petitBot

//Bibliotheque Servo
#include <Servo.h>

Servo servo1; // creation
Servo servo2;
Servo servo3;

Servo servoG;
Servo servoD;

// Bibliotheque capteur ultrason
#include <Ultrasonic.h>
Ultrasonic Ultrasonic1(D5, D0);


// Constantes de Broches
const int pin_led1 = D3; // pin led 1
const int pin_led2 = D4; // pin led 2

const int pin_servoG = D1; // pin servo gauche
const int pin_servoD = D2; // pin servo droit

const int pin_servo1 = D8; // pin servo 1
const int pin_servo2 = D7; // pin servo 2
const int pin_servo3 = D6; // pin servo 3

const int pin_dist1 = D5; // pin capteur de distance 1
const int pin_dist2 = D0; // pin capteur de distance 2

void setup() {
  Serial.begin (9600); // initialisation de la connexion série

  //initialisation des broches led :
  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);

  //initialisation des servo
  servoG.attach(pin_servoG);
  servoD.attach(pin_servoD);

  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
  servo3.attach(pin_servo3);

  //Initialisation du WIFI
  WiFi.mode(WIFI_AP);
  WiFi.softAP(hostAP, "", 2);
  MDNS.begin(hostAP);
  MDNS.addService("http", "tcp", 80);
  serveur();
  server.begin(); //

}

void loop() {
  server.handleClient();
}

void serveur() {

  server.on("/led1_on", HTTP_GET, []() {
    digitalWrite(pin_led1, HIGH);
    server.send(200, "text/json", "Led1 allumée");
  });

  server.on("/led1_off", HTTP_GET, []() {
    digitalWrite(pin_led1, LOW);
    server.send(200, "text/json", "Led1 éteinte");
  });

  server.on("/led2_on", HTTP_GET, []() {
    digitalWrite(pin_led2, HIGH);
    server.send(200, "text/json", "Led2 allumée");
  });

  server.on("/led2_off", HTTP_GET, []() {
    digitalWrite(pin_led2, LOW);
    server.send(200, "text/json", "Led2 éteinte");
  });

  server.on("/servo1", HTTP_GET, []() {
    String angle = server.arg("angle");
    servo1.write(angle.toInt());
    server.send(200, "text/json", "Servo1 : " + angle);
  });

  server.on("/servo2", HTTP_GET, []() {
    String angle = server.arg("angle");
    servo2.write(angle.toInt());
    server.send(200, "text/json", "Servo2 : " + angle);
  });

  server.on("/servo3", HTTP_GET, []() {
    String angle = server.arg("angle");
    servo3.write(angle.toInt());
    server.send(200, "text/json", "Servo3 : " + angle);
  });

  server.on("/servoG", HTTP_GET, []() {
    String angle = server.arg("angle");
    servoG.write(angle.toInt());
    server.send(200, "text/json", "ServoG : " + angle);
  });

  server.on("/servoD", HTTP_GET, []() {
    String angle = server.arg("angle");
    servoD.write(angle.toInt());
    server.send(200, "text/json", "ServoD : " + angle);
  });

  server.on("/avance", HTTP_GET, []() {
    String vitesse = server.arg("vitesse");
    if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
    servoG.write(90 + vitesse.toInt());
    servoD.write(90 - vitesse.toInt());
    server.send(200, "text/json", "avance : " + String(100 * (90 + vitesse.toInt()) / 180) + " %");
  });

  server.on("/recule", HTTP_GET, []() {
    String vitesse = server.arg("vitesse");
    if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
    servoG.write(90 - vitesse.toInt());
    servoD.write(90 + vitesse.toInt());
    server.send(200, "text/json", "recule : " + String(100 * (90 + vitesse.toInt()) / 180) + " %");
  });

  server.on("/droite", HTTP_GET, []() {
    String vitesse = server.arg("vitesse");
    if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
    servoG.write(90 + vitesse.toInt());
    servoD.write(90 + vitesse.toInt());
    server.send(200, "text/json", "droite : " + String(100 * (90 + vitesse.toInt()) / 180) + " %");
  });
  server.on("/gauche", HTTP_GET, []() {
    String vitesse = server.arg("vitesse");
    if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
    servoG.write(90 - vitesse.toInt());
    servoD.write(90 - vitesse.toInt());
    server.send(200, "text/json", "droite : " + String(100 * (90 + vitesse.toInt()) / 180) + " %");
  });

  server.on("/stop", HTTP_GET, []() {
    servoG.write(90);
    servoD.write(90);
    server.send(200, "text/json", "STOP !");
  });

  server.on("/analog", HTTP_GET, []() {
    int value = analogRead(A0);
    server.send(200, "text/json", String(value));
  });

  server.on("/distance", HTTP_GET, []() {
    int distance = Ultrasonic1.distanceRead();
    server.send(200, "text/json", String(distance));
  });
  
  server.on("/", HTTP_GET, []() {
    pageweb();
  });

}

void pageweb() {
  // Prepare la page web de réponse. on commence par le "header" commun; (le code HTML sera ecrit dans la chaine de carcatere s).
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n";
  s += "<html>\r\n";
  s += "<center>";
  s += "</h1>";
  s += "<br>"; //aller a la ligne
  s += "<br>"; //aller a la ligne
  s += "<a href=\"/stop\"\"><button style=\"font-size:200%; width: 18%\"\>Stop </button></a>\r\n";// creer un boutton "Stop"
  s += "<a href=\"/avance\"\"><button style=\"font-size:200%; width: 18%\"\>Avance </button></a>\r\n";
  s += "<a href=\"/recule\"\"><button style=\"font-size:200%; width: 18%\"\>Recule </button></a>\r\n";
  s += "<a href=\"/droite\"\"><button style=\"font-size:200%; width: 18%\"\>Droite </button></a>\r\n";
  s += "<a href=\"/gauche\"\"><button style=\"font-size:200%; width: 18%\"\>Gauche </button></a><br />\r\n";
  s += "</center>";

  s += "</html>\n"; //Fin de la page Web

  // Envoie de la reponse au navigateur
  server.send(200, "text/html", s);
  delay(1);


}
