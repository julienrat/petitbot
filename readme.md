# Le Petit Bot
1. Le Petit Bot c'est un robot, un programme, des logiciels pour commander Petit Bot et un programme pédagogique.
2. Le Petit Bot c'est un matériel facile à assembler par des jeunes à partir de 8 ans
3. Le Petit Bot ne coûte pas plus de 15 euros.

Si vous repoduisez le projet, merci de communiquer avec le hastag #petitbot
## Installation
Pour flasher le PetitBot plusieures solutions s'offrent à vous :

1. Compiler les sources arduino en ajoutant les bibliothèques nécessaires
2. flasher le wemos à l'aide d'un utilitaire et des binaires

## Compiler les sources

## Flasher les binaires
Téléchargez et décompresser le fichier zip https://github.com/julienrat/petitbot/raw/master/flasher_petitbot.zip
### Sous Windows
### Sous Linux
Sous linux, placez vous dans le répertoire décompressé, puis lancez la commande suivante :
'''./esptool.py --port /dev/ttyUSB0 write_flash 0x100000 petitbot_v3.spiffs.bin 0x00000 petitbot_v3.ino.d1_mini.bin 
'''



## Credits
CCbyNCSA Les petits débrouillards 2016

