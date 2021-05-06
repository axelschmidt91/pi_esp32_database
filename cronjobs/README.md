# Cronjobs

Insert something like 

```
MAILTO=me@myself.com
*/1 *  *   *   *     /home/pi/pi_esp32_database/cronjobs/mysql_cleanup.sh
*/15 * *   *   *     /home/pi/pi_esp32_database/cronjobs/mysql_sensor_up_check.sh Garten 1
*/15 * *   *   *     /home/pi/pi_esp32_database/cronjobs/mysql_sensor_up_check.sh Flur 1
```

 with ```crontab -e``` to the cron jobs to setup the "Cleanup" of rigged data and the "Sensor up" check of every ESP32 you define.

set system-wide environment variable `MYSQL_USER=[your_user]` and `MYSQL_PW=[your_pw]` to define your password that is used to login to mysql 
