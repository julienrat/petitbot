# Le Petit Bot ---
1. Le Petit Bot c'est un robot, un programme, des logiciels pour commander Petit Bot et un programme pédagogique.
2. Le Petit Bot c'est un matériel facile à assembler par des jeunes à partir de 8 ans
3. Le Petit Bot ne coûte pas plus de 15 euros.

Si vous repoduisez le projet, merci de communiquer avec le hastag #petitbot
## Installation
Pour flasher le PetitBot plusieures solutions s'offrent à vous :

1. Compiler les sources arduino en ajoutant les bibliothèques nécessaires
2. Flasher le wemos à l'aide d'un utilitaire et des binaires

## Compilez les sources (recommandé)
La première solution est de flasher la dernière mouture du code du petitBot.
### Préparez votre environnement de développement
1. Installez la dernière version d'arduino disponible ici : https://www.arduino.cc/en/Main/Software
2. Ajoutez la carte Wemos à votre bibliothèque de cartes :
3. Rendez-vous dans Fichier>Préférences et ajoutez le lien http://arduino.esp8266.com/stable/package_esp8266com_index.json  dans la partie URL de gestionnaire de cartes supplémentaires
4. Puis dans Outils>Type de carte>Gestionnaire de carte ajoutez ESP8266
![alt text](https://github.com/julienrat/petitbot/blob/master/images/install_wemos.gif "Logo Title Text 1")
5.	Sélectionnez la carte Wemos D1 R1 mini dans Outils>Type de carte>Gestionnaire de cartes
![alt text](https://github.com/julienrat/petitbot/blob/master/images/choix_carte.gif "Logo Title Text 1")
6. Branchez le Wemos et sélectionnez le bon port USB
![alt text](https://github.com/julienrat/petitbot/blob/master/images/choix_port.gif "Logo Title Text 1")
7. Installez l'utilitaire de flashage de la mémoire SPIFFS, Téléchargez le fichier https://github.com/esp8266/arduino-esp8266fs-plugin/releases/tag/0.4.0
8. Copiez-collez je fichier esp8266fs.jar dans le répertoire <home_dir>/Arduino/tools/ESP8266FS/tool/esp8266fs.jar
![alt text](https://github.com/julienrat/petitbot/blob/master/images/install_plugin.gif "Logo Title Text 1")
9. Redémarrez Arduino et une nouvelle option "ESP8266 Sketch Data Upload" apparaitra dans le menu Outils
![alt text](https://github.com/julienrat/petitbot/blob/master/images/upload_spiffs.gif "Logo Title Text 1")
10. Installez les bibliothèques nécessaires pour compiler Petitbot
- ArduinoJson : https://github.com/julienrat/petitbot/raw/master/code_arduino/petitbot_v3/libraries/ArduinoJson.zip
- ESPAsyncTCP : https://github.com/julienrat/petitbot/raw/master/code_arduino/petitbot_v3/libraries/ESPAsyncTCP-master.zip
- ESPAsyncWebServer : https://github.com/julienrat/petitbot/raw/master/code_arduino/petitbot_v3/libraries/ESPAsyncWebServer-master.zip
- Ultrasonic : https://github.com/julienrat/petitbot/raw/master/code_arduino/petitbot_v3/libraries/Ultrasonic.zip
![alt text](https://github.com/julienrat/petitbot/blob/master/images/libs.gif "Logo Title Text 1")
11. Voila votre environnement est prêt, il suffit alors d'importer le code petitbot (avec son repertoire data)

### Flashez le Wemos
1. Importez le code petitbot (dézippez le avec son repertoire data) dans votre dossier de sketchs <home_dir>/Arduino: https://github.com/julienrat/petitbot/raw/master/code_arduino/petitbot_v3.zip
![alt text](https://github.com/julienrat/petitbot/blob/master/images/import.gif "Logo Title Text 1")
2. Redémarrez Arduino, branchez la carte Wemos, Assurez-vous d'avoir sélectionné le bon port et uploadez le programme.
![alt text](https://github.com/julienrat/petitbot/blob/master/images/upload.gif "Logo Title Text 1")
3. Enfin uploadez les pages Web dans la mémoire SPIFFS du wemos Outils>ESP8266 Sketch Data Upload
![alt text](https://github.com/julienrat/petitbot/blob/master/images/spiffs_upload.gif "Logo Title Text 1")
4. Et Voilà !
## Flashez les binaires 
Téléchargez et décompresser le fichier zip dans votre répertoire ome_dir>/Arduino https://github.com/julienrat/petitbot/raw/master/flasher_petitbot.zip
### Sous Windows
Sous windows, vous aurez besoin du logiciel ESP8266Flasher.EXE (présent dans le zip), branchez le wemos, installez le driver Wemos https://wiki.wemos.cc/_media/file:ch341ser_win.zip, lancez ESP8266Flasher.EXE puis sélectionnez le bon port COM


![alt text](https://github.com/julienrat/petitbot/raw/master/images/Capture.PNG "Logo Title Text 1")


Dans l'onglet config, sélectionnez le fichier petitbot_v3.ino.d1_mini.bin et fixez l'adresse de flashage à 0x00000, en dessous sélectionnez le fichier petitbot_v3.spiffs.bin et fixez l'adresse de flashage à 0x10000


![alt text](https://github.com/julienrat/petitbot/raw/master/images/Capture2.PNG "Logo Title Text 1")


Dans l'onglet AVANCED, Sélectionnez la vitesse à 921600


![alt text](https://github.com/julienrat/petitbot/raw/master/images/Capture3.PNG "Logo Title Text 1")


Enfin, revenez sur la première page, puis cliquez sur Flasher


![alt text](https://github.com/julienrat/petitbot/raw/master/images/Capture.PNG "Logo Title Text 1")
### Sous Linux
Sous linux, installez python serial :

```sudo apt-get install python-serial```

Placez vous dans le répertoire décompressé, puis lancez la commande suivante :

```sudo python esptool.py --port /dev/ttyUSB0 write_flash 0x100000 petitbot_v3.spiffs.bin 0x00000 petitbot_v3.ino.d1_mini.bin```


## Usage
Connectez vous au réseau wifi "petitbot" puis entrez l'adresse suivante dans un navigateur http://192.168.4.1

## Credits
CCbyNCSA Les petits débrouillards 2016

