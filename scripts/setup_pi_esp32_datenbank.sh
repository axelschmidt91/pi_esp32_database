
#!/bin/sh
echo Update des Pi
sudo apt update && sudo apt upgrade -y

echo Install Apache
sudo apt install apache2 -y

cd /var/www/html
ls -al

echo Die IP-Adresse des PI:
hostname -I

echo Install PHP
sudo apt install php -y
sudo rm index.html

echo index.php erstellen und 
echo "<?php echo "hello world"; ?>" > index.php

sudo service apache2 restart

echo Install MariaDB und MySQL
sudo apt install mariadb-server php-mysql -y
sudo mysql_secure_installation

sudo phpenmod mysqli
sudo service apache restart
