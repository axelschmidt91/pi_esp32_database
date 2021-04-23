# pi_esp32_database

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
