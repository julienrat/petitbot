#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>

#include <Ticker.h>

Ticker stopper;

// SKETCH BEGIN
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
const char* hostAP = "PetitBLOCK"; // nom du point d'acces petitBot
static int restartNow = false;
const char* version = "V3";
//Bibliotheque Servo
#include <Servo.h>
// Bibliotheque capteur ultrason
#include <Ultrasonic.h>
Ultrasonic Ultrasonic1(D5, D0);

Servo servo1; // creation
Servo servo2;
Servo servo3;

Servo servoG;
Servo servoD;
// Constantes de Broches
const int pin_led1 = D3; // pin led 1
const int pin_led2 = D4; // pin led 2

int pin_servoG = D1; // pin servo gauche
int pin_servoD = D2; // pin servo droit

const int pin_servo1 = D8; // pin servo 1
const int pin_servo2 = D7; // pin servo 2
const int pin_servo3 = D6; // pin servo 3

const int pin_dist1 = D5; // pin capteur de distance 1
const int pin_dist2 = D0; // pin capteur de distance 2

String name ;
String invert_motors ;
String access_point;
String ssid ;
String mdp ;
String canal ;
String ip ;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  SPIFFS.begin();
  server.begin();

  //initialisation des broches led :
  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);

  //initialisation des servo
  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
  servo3.attach(pin_servo3);




  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on("/led1_on", HTTP_GET, [](AsyncWebServerRequest * request) {

    digitalWrite(pin_led1, HIGH);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/led1_off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(pin_led1, HIGH);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/led2_on", HTTP_GET, [](AsyncWebServerRequest * request) {

    digitalWrite(pin_led2, HIGH);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/led2_off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(pin_led2, HIGH);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/servo1", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("angle")) {
        String angle = request->arg("angle");
        Serial.print(angle.toInt());
        request->send(200, "text/json", "Servo1 : " + angle );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    }
  });

  server.on("/servo2", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("angle")) {
        String angle = request->arg("angle");
        servo2.write(angle.toInt());
        request->send(200, "text/json", "Servo2 : " + angle );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    }
  });

  server.on("/servo3", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("angle")) {
        String angle = request->arg("angle");
        servo3.write(angle.toInt());
        request->send(200, "text/json", "Servo3 : " + angle );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    }
  });

  server.on("/servoG", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("angle")) {
        String angle = request->arg("angle");
        servoG.write(angle.toInt());
        request->send(200, "text/json", "ServoG : " + angle );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    }
  });

  server.on("/servoD", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoD.attach(pin_servoD);
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("angle")) {
        String angle = request->arg("angle");
        servoD.write(angle.toInt());
        request->send(200, "text/json", "ServoD : " + angle );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    }
  });


  server.on("/avance", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("vitesse")) {
        String vitesse = request->arg("vitesse");
        if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
        servoG.write(90 + vitesse.toInt());
        servoD.write(90 - vitesse.toInt());
        request->send(200, "text/json", "Avance : " + vitesse );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    } else {
      servoG.write(180);
      servoD.write(0);
      request->send(200, "text/json", "Avance");
    }
  });
  server.on("/step_avance", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);

    servoG.write(180);
    servoD.write(0);
    stopper.attach(0.4, arret);

    request->send(200, "text/json", "step avance");

  });

  server.on("/step_recule", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);

    servoG.write(0);
    servoD.write(180);
    stopper.attach(0.4, arret);

    request->send(200, "text/json", "step recule");

  });
  server.on("/recule", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("vitesse")) {
        String vitesse = request->arg("vitesse");
        if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
        servoD.write(90 + vitesse.toInt());
        servoG.write(90 - vitesse.toInt());
        request->send(200, "text/json", "Recule : " + vitesse );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    } else {
      servoD.write(180);
      servoG.write(0);
      request->send(200, "text/json", "Recule");
    }
  });

  server.on("/gauche", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("vitesse")) {
        String vitesse = request->arg("vitesse");
        if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
        servoD.write(90 - vitesse.toInt());
        servoG.write(90 - vitesse.toInt());
        request->send(200, "text/json", "Gauche : " + vitesse );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    } else {
      servoD.write(180);
      servoG.write(180);
      request->send(200, "text/json", "Gauche");
    }
  });

  server.on("/step_gauche", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);

    servoD.write(180);
    servoG.write(180);
    stopper.attach(0.2, arret);
    request->send(200, "text/json", "Gauche");

  });


  server.on("/step_droite", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);

    servoD.write(0);
    servoG.write(0);

    stopper.attach(0.2, arret);
    request->send(200, "text/json", "Droite");

  });

  server.on("/droite", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
    if (request->params() > 0 and request->args() > 0) {
      if (request->hasParam("vitesse")) {
        String vitesse = request->arg("vitesse");
        if (vitesse == "" || vitesse.toInt() > 90) vitesse = "90";
        servoD.write(90 - vitesse.toInt());
        servoG.write(90 - vitesse.toInt());
        request->send(200, "text/json", "Droite : " + vitesse );
      } else {
        request->send(200, "text/json", "Mauvaise commande" );
      }
    } else {
      servoD.write(0);
      servoG.write(0);
      request->send(200, "text/json", "Droite");
    }
  });

  server.on("/write", HTTP_GET, [](AsyncWebServerRequest * request) { // http://192.168.4.1/write?name=tata%20yoyo&invert_motors=true&mode=acces_point&ssid=reseau_maison&mdp=1245&canal=8&ip=192.168.1.33
    if (request->params() > 0 and request->args() > 0) {
      name = request->arg("name");
      invert_motors = request->arg("invert_motors");
      access_point = request->arg("mode");
      ssid = request->arg("ssid");
      mdp = request->arg("mdp");
      canal = request->arg("canal");
      ip = request->arg("ip");
      String json;
      json += "{";
      json += "\"name\":\"" + name + "\"";
      json += ",\"invert_motors\":\"" + invert_motors + "\"";
      json += ",\"mode\":\"" + access_point + "\"";
      json += ",\"ssid\":\"" + ssid + "\"";
      json += ",\"mdp\":\"" + mdp + "\"";
      json += ",\"canal\":\"" + canal + "\"";
      json += ",\"ip\":\"" + ip + "\"";
      json += "}";
      File configuration = SPIFFS.open("/config.txt", "w");
      configuration.println(json);
      configuration.close();
      request->send(200, "text/json", json);
      ESP.reset();
    }
  });


  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
    servoD.write(90);
    servoG.write(90);
    Serial.println("STOP");
    request->send(200, "text/json", "STOP");
  });

  server.on("/analog", HTTP_GET, [](AsyncWebServerRequest * request) {
    int value = analogRead(A0);
    request->send(200, "text/json", String(value));
  });

  server.on("/version", HTTP_GET, [](AsyncWebServerRequest * request) {
    int value = analogRead(A0);
    request->send(200, "text/json", "PetitBot : " + String(version));
  });



  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest * request) {
    int distance = Ultrasonic1.distanceRead();
    request->send(200, "text/json", String(distance));
  });


  server.on("/update", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200);
  }, handle_update_progress_cb);


  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest * request) {
    String json = "[";
    int n = WiFi.scanComplete();
    if (n == -2) {
      WiFi.scanNetworks(true);
    } else if (n) {
      for (int i = 0; i < n; ++i) {
        if (i) json += ",";
        json += "{";
        json += "\"rssi\":" + String(WiFi.RSSI(i));
        json += ",\"ssid\":\"" + WiFi.SSID(i) + "\"";
        json += ",\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"";
        json += ",\"channel\":" + String(WiFi.channel(i));
        json += ",\"secure\":" + String(WiFi.encryptionType(i));
        json += ",\"hidden\":" + String(WiFi.isHidden(i) ? "true" : "false");
        json += "}";
      }
      WiFi.scanDelete();
      if (WiFi.scanComplete() == -2) {
        WiFi.scanNetworks(true);
      }
    }
    json += "]";
    request->send(200, "text/json", json);
    json = String();
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  read_config();
  set_wifi();
  if (invert_motors.equals("true")) {
    Serial.println("on inverse les moteurs");
    pin_servoG = D2; // pin servo gauche
    pin_servoD = D1; // pin servo droit
  }

}


void arret() {
  servoG.attach(pin_servoG);
  servoD.attach(pin_servoD);
  servoD.write(90);
  servoG.write(90);
  stopper.detach();
  Serial.println("STOP");
}

void read_config() {
  File configuration = SPIFFS.open("/config.txt", "r");
  StaticJsonBuffer<200> jsonBuffer;
  char char_json[200];

  String json = configuration.readStringUntil('\n');
  json.toCharArray(char_json, 200);
  JsonObject& root = jsonBuffer.parseObject(char_json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  const char* ip_char = root["ip"];
  const char* name_char = root["name"];
  const char* access_point_char = root["mode"];
  const char* ssid_char = root["ssid"];
  const char* mdp_char = root["mdp"];
  const char* canal_char = root["canal"];
  const char* invert_motors_char = root["invert_motors"];

  ip = String(ip_char);
  name = String(name_char);
  access_point = String(access_point_char);
  ssid = String(ssid_char);
  mdp = String(mdp_char);
  canal = String(canal_char);
  invert_motors = String(invert_motors_char);

}
static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!index) {
    Serial.println("Update");
    if (filename.endsWith(".d1_mini.bin")) {
      Serial.println("update to flash");
      Update.runAsync(true);
      if (!Update.begin(free_space)) {
        Update.printError(Serial);
      }
    }

    if (filename.endsWith(".spiffs.bin")) {
      Serial.println("update to spiffs");
      Update.runAsync(true);
      if (!Update.begin(free_space, U_SPIFFS)) {
        Update.printError(Serial);
      }
    }

  }

  if (Update.write(data, len) != len) {
    Update.printError(Serial);
  }

  if (final) {
    if (!Update.end(true)) {
      Update.printError(Serial);
    } else {
      restartNow = true;//Set flag so main loop can issue restart call
      Serial.println("Update complete");
    }
  }
}
void set_wifi() {
WiFi.hostname(name.c_str());
  if (access_point.equals("access_point")) {
    Serial.println(" ");
    Serial.println("PetitBot : " + String(version));
    Serial.println("Configuration Point d'acces");
    Serial.print("nom du point d'acces : ");
    Serial.println(name);    
    WiFi.mode(WIFI_AP);
    
    WiFi.softAP(name.c_str(), "", canal.toInt());
  } else {
    Serial.print("Connexion au reseau : ");
    Serial.println(ssid);   
    WiFi.mode(WIFI_STA);

    int timeout = 0;
    WiFi.begin(ssid.c_str(), mdp.c_str());

    Serial.println("connection en cours...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      timeout++;
      Serial.print(".");

      if (timeout > 50) {
        Serial.println("TimeOUT");
        access_point = "access_point";
        set_wifi();
        return;

      }
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }






  ArduinoOTA.setHostname(name.c_str());
  ArduinoOTA.begin();
  MDNS.addService("http", "tcp", 80);
  MDNS.begin(name.c_str());








}
void loop() {

  ArduinoOTA.handle();
  if (restartNow) {
    Serial.println("Restart");
    ESP.reset();
  }
}



