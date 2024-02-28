
// Gère les mise à jour à distance
static void ota_update(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!index) {
    Serial.println("Update");
    if (filename.endsWith(".d1_mini.bin")) {
      Serial.println("update to flash");
      if (!Update.begin(free_space)) { // starts with max available size
        Update.printError(Serial);
      }
    }

    if (filename.endsWith(".spiffs.bin")) {
      Serial.println("update to spiffs");
      if (!Update.begin(free_space, U_SPIFFS)) {
        Update.printError(Serial);
      }
    }

  }
  /* flashing firmware to ESP*/
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
