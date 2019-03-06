#Splitting the code
On a découpé le code par responsabilité et chaque methode est prefixé par son nom de fichier



## Migration
###1/ Installer la board :
On a commencé par installer la board via le board manager : https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md : on a donc ajouté https://dl.espressif.com/dl/package_esp32_index.json via le champ Additional Board Manager URLs

###2/ Copie des fichiers statiques
On a remplacé  Arduino ESP8266 filesystem uploader par [Arduino ESP32 filesystem uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin) pour pouvoir copier les fichiers statiques sur le robot ( ça marche comme avant, ça copie le dossier data). C'est le même mode opératoire que pour le précédent, le .jar est recupérable ici : https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/tag/1.0 .


### 3/ Gestion des librairies tierces 
Ona du migrer des librairies :

|| [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)| ESPAsyncWebServer| version embarqué par le board arduino-esp32||
||[AsyncTCP](https://github.com/me-no-dev/AsyncTCP) | ESPAsyncTCP | à installer via le dossier [libraries](./libraries) ||
|| ESPmDNS | ESP8266mDNS | à installer via le dossier [libraries](./libraries) ||
|| WiFi| ESP8266WiFi &  WiFiClient | version embarqué par le board arduino-esp32||
|| [ESP32Servo](https://github.com/jkb-git/ESP32Servo)| Servo | à installer via le dossier [libraries](./libraries) ||
|| Ultrasonic | Ultrasonic| Montée de version|| 
|| ArduinoOTA | ArduinoOTA | Inchangé ( à) installer via l'IDE)||
|| ArduinoJson | ArduinoJson | à installer via le dossier [libraries](./libraries) ||
