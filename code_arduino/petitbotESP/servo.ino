// Configuration du servo et des commandes

void servo_setup () {
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

void servo_stop () {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
    servoD.write(90);
    servoG.write(90);
    servoG.detach(); // ces deux commandes ont été rajouté car parfois les roues du bot continuaient à trouver même en ayant envoyé le signal STOP
    servoD.detach(); // ces deux commandes ont été rajouté car parfois les roues du bot continuaient à trouver même en ayant envoyé le signal STOP
    Serial.println("STOP");
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
