
// Configure et démarre le portail 
void server_setup () {

  ArduinoOTA.setHostname(name.c_str());
  ArduinoOTA.begin();
  MDNS.addService("http", "tcp", 80);
  MDNS.begin(name.c_str());
  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  server.begin();

  /////redirections du portail captif
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
    delay(100);
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
    delay(100);
  });

  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
    delay(100);
  });

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
    delay(100);
  });
  ////////////////



  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on("/led1_on", HTTP_GET, [](AsyncWebServerRequest * request) {

    digitalWrite(pin_led1, LOW); //erreur initial sur le code, il était écrit HIGH
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/led1_off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(pin_led1, HIGH);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/led2_on", HTTP_GET, [](AsyncWebServerRequest * request) {

    digitalWrite(pin_led2, LOW);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/led2_off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(pin_led2, HIGH);
    request->send(200, "text/json", "Led1 allumée");
  });

  server.on("/servo1", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleSetAngleRequest (servo1, "servo1");
  });

  server.on("/servo2", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleSetAngleRequest (servo2, "servo2");
  });

  server.on("/servo3", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleSetAngleRequest (servo3, "servo3");
  });

  server.on("/servoG", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleSetAngleRequest (servoG, "servoG");
  });

  server.on("/servoD", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleSetAngleRequest (servoD, "servoD");
  });

  server.on("/avance", HTTP_GET, [](AsyncWebServerRequest * request) {
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
        if (vitesse == "" || vitesse.toInt() > 90) {
          vitesse = "90";
        }
        servoD.write(90 - vitesse.toInt());
        servoG.write(90 - vitesse.toInt());
        request->send(200, "text/json", "Gauche : " + vitesse );
      } else {
        request->send(400, "text/json", "Mauvaise commande" );
      }
    } else {
      servoD.write(0);
      servoG.write(0);
      request->send(200, "text/json", "Gauche");
    }
  });

  server.on("/step_gauche", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);

    servoD.write(0);
    servoG.write(0);
    stopper.attach(0.2, arret);
    request->send(200, "text/json", "Gauche");

  });


  server.on("/step_droite", HTTP_GET, [](AsyncWebServerRequest * request) {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);

    servoD.write(180);
    servoG.write(180);

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
        request->send(400, "text/json", "Mauvaise commande" );
      }
    } else {
      servoD.write(180);
      servoG.write(180);
      request->send(200, "text/json", "Droite");
    }
  });

  server.on("/write", HTTP_GET, [](AsyncWebServerRequest * request) { // http://192.168.4.1/write?name=tata%20yoyo&invert_motors=true&mode=acces_point&ssid=reseau_maison&mdp=1245&canal=8&ip=192.168.1.33    
      if (request->params() > 0 and request->args() > 0) {
        String json = config_update (request);
        request->send(200, "text/json", json);
        ESP.restart();
      }
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest * request) {
    servo_stop ();
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
    int distance = Ultrasonic1.read();
    request->send(200, "text/json", String(distance));
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200);
  }, ota_update);


  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest * request) {
    String json = wifi_getJSONScan();
    request->send(200, "text/json", json);
    json = String();
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

}

void handleSetAngleRequest (Servo servo, String servoName) {
  if (request->params() > 0 and request->args() > 0 and request->hasParam("angle")) {
    String angle = request->arg("angle");
    servo.write(angle.toInt());
    request->send(200, "text/json", servoName + " : " + angle );
  } else {
    request->send(400, "text/json", "Mauvaise commande" );
  }
}
