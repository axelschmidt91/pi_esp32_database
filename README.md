# pi_esp32_database

https://randomnerdtutorials.com/esp32-esp8266-raspberry-pi-lamp-server/


## Pi
Rasbian mit neuer Datei ```wpa_supplicant.conf```:
```
country=DE
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
network={
ssid="WLAN-Name"
psk="Passwort"
}
```
Leere Datei ```ssh``` in boot ablegen um ssh zu aktivieren.

Ausführen:
```sudo apt update && sudo apt install git```

Ausführen des scripts setup_pi

Ausführen des scripts setup_database

Kopieren der ```post-esp-data.php``` und ändern des Passworts:
```
sudo cp post-esp-data.php /var/www/html/post-esp-data.php
```

Kopieren der ```esp-data.php``` und ändern des Passwors:
```
sudo cp esp-data.php /var/www/html/esp-data.php
```
