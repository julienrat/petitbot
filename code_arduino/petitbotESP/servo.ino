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

// Modifie l'angle d'un servo
void servo_setAngle (Servo servo, String angle) {
  servo.write(angle.toInt());
};

// Recule 
void servo_recule (String vitesse){
  servoD.write(90 + vitesse.toInt());
  servoG.write(90 - vitesse.toInt());
}

// Avance 
void servo_avance (String vitesse) {
  servoG.write(90 + vitesse.toInt());
  servoD.write(90 - vitesse.toInt());
}

// Tourne à gauche 
void servo_gauche (String vitesse) {
  servoD.write(90 - vitesse.toInt());
  servoG.write(90 - vitesse.toInt());
}

// Tourne à droite 
void servo_droite (String vitesse) {
  servoD.write(90 + vitesse.toInt());
  servoG.write(90 + vitesse.toInt());
}


// Arrete les servomoteurs Gauche et Droit
void servo_stop () {
  servoD.write(90);
  servoG.write(90);

  stopper.detach();
  Serial.println("STOP");
}



// Avance puis s'arrete
void servo_step_avance () {
  servo_avance("90");
  stopper.attach(0.4, servo_stop);
}

// Recule puis s'arrete
void servo_step_recule () {
  servo_recule("90");
  stopper.attach(0.4, servo_stop);
}

// Tourne à droite puis s'arrete
void servo_step_droite () {
  servo_droite("90");
  stopper.attach(0.2, servo_stop);
}

// Tourne à gauche puis s'arrete
void servo_step_gauche () {
  servo_gauche("90");
  stopper.attach(0.2, servo_stop);
}
