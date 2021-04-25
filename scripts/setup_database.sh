#!/bin/sh

if [ $1 -eq 0 ]
  then
    echo "No password for user pi provided"
    exit 0
fi

echo Create database user pi
mysql -u root -e "CREATE USER IF NOT EXISTS pi@localhost identified by '$1';"
echo Grant Privileges
mysql -u root -e "GRANT ALL PRIVILEGES ON *.* TO pi@localhost;"
echo Flush Privileges
mysql -u root -e "FLUSH PRIVILEGES;"

echo Create Database if not exists
mysql -u root -e "CREATE DATABASE IF NOT EXISTS esp_data CHARACTER SET utf8mb4;"
mysql -u root -e "USE esp_data; CREATE TABLE IF NOT EXISTS SensorData (id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, sensor VARCHAR(30) NOT NULL, location VARCHAR(30) NOT NULL, value FLOAT(10), unit VARCHAR(30), reading_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP)"
