#!/bin/sh
echo     Update des Pi
echo *********************
sudo apt update && sudo apt upgrade -y

echo     Install Apache
echo **********************
sudo apt install apache2 -y

cd /var/www/html
ls -al

echo     Die IP-Adresse des PI:
echo ******************************
hostname -I

echo     Install PHP
echo *******************
sudo apt install php -y
sudo rm index.html

echo     index.php erstellen und initieren
echo *****************************************
echo "<?php echo "hello world"; ?>" > index.php

sudo service apache2 restart

echo     Install MariaDB und MySQL
echo *********************************
sudo apt install mariadb-server php-mysql -y

echo     Install Security Applications
echo *************************************
sudo mysql_secure_installation
sudo phpenmod mysqli
sudo service apache restart
