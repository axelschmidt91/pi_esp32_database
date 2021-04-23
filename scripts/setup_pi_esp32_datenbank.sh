#!/bin/sh
echo
echo Update des Pi
echo
sudo apt update && sudo apt upgrade -y

echo
echo Install Apache
echo
sudo apt install apache2 -y

cd /var/www/html
ls -al

echo
echo Die IP-Adresse des PI:
echo
hostname -I

echo
echo Install PHP
echo
sudo apt install php -y
sudo rm index.html
sudo service apache2 restart

echo
echo Install MariaDB und MySQL
echo
sudo apt install mariadb-server php-mysql -y
sudo service apache2 restart

echo
echo Install Security Applications
echo
sudo mysql_secure_installation

echo
echo Install phpMyAdmin
echo
sudo apt install phpmyadmin -y
sudo phpenmod mysqli
sudo service apache2 restart

if [!-d /var/www/html/phpmyadmin]; then
    sudo ln -s /usr/share/phpmyadmin /var/www/html/phpmyadmin
fi;


echo 
echo Permission settings
echo
ls -lh /var/www/
sudo chown -R pi:www-data /var/www/html/
sudo chmod -R 770 /var/www/html/
ls -lh /var/www/
