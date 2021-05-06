#!/bin/bash 

mysql -u $MYSQL_USER -p$MYSQL_PW -v -v -v esp_data -e "delete from SensorData where (sensor = 'Temperature' and (value > 50 or value < -20)) or (sensor = 'Pressure' and (value > 101800 or value < 90000))"
