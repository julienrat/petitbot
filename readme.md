# Le Petit Bot
1. Le Petit Bot c'est un robot, un programme, des logiciels pour commander Petit Bot et un programme pédagogique.
2. Le Petit Bot c'est un matériel facile à assembler par des jeunes à partir de 8 ans
3. Le Petit Bot ne coûte pas plus de 15 euros.

Si vous repoduisez le projet, merci de communiquer avec le hastag #petitbot
## Installation
Pour flasher le PetitBot plusieures solutions s'offrent à vous :

1. Compiler les sources arduino en ajoutant les bibliothèques nécessaires
2. Flasher le wemos à l'aide d'un utilitaire et des binaires

## Compiler les sources
La première solution est de flasher la dernière mouture du code du petitBot.
### Préparez votre environnement de développement
1. Installez la dernière version d'arduino disponible ici : https://www.arduino.cc/en/Main/Software
2. Ajoutez la carte Wemos à votre bibliothèque de cartes :

 
## Flasher les binaires
Téléchargez et décompresser le fichier zip https://github.com/julienrat/petitbot/raw/master/flasher_petitbot.zip
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
Sous linux, installez esptool :

```pip install esptool```

Placez vous dans le répertoire décompressé, puis lancez la commande suivante :

```./esptool.py --port /dev/ttyUSB0 write_flash 0x100000 petitbot_v3.spiffs.bin 0x00000 petitbot_v3.ino.d1_mini.bin```


## Usage
Connectez vous au réseau wifi "petitbot" puis entrez l'adresse suivante dans un navigateur http://192.168.4.1

## Credits
CCbyNCSA Les petits débrouillards 2016

