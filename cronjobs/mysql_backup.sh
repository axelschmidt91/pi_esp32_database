#!/bin/bash
#
# backup-mysql.sh 1.0.5
#
# Dumps all databases to seperate files.
# All files are created in a folder named by the current date.
# Folders exceeding the defined hold time are purged automatically.
# 
# (c)2015-2019 Harald Schneider
#

# Setup.start
#

HOLD_DAYS=14 
TIMESTAMP=$(date +"%F")
BACKUP_DIR="/home/pi/.backups/mysql"

MYSQL_USR=$MYSQL_USER
MYSQL_PWD=$MYSQL_PW

# Use this on Linux
#
MYSQL_CMD=/usr/bin/mysql  
MYSQL_DMP=/usr/bin/mysqldump  
MYSQL_CHECK=/usr/bin/mysqlcheck

# Use this for MAMP on macOS:
#
#MYSQL_CMD=/Applications/MAMP/Library/bin/mysql
#MYSQL_DMP=/Applications/MAMP/Library/bin/mysqldump
#MYSQL_CHECK=/Applications/MAMP/Library/bin/mysqlcheck

# Use this for AMPPS on macOS:
#
#MYSQL_CMD=/Applications/AMPPS/mysql/bin/mysql
#MYSQL_DMP=/Applications/AMPPS/mysql/bin/mysqldump
#MYSQL_CHECK=/Applications/AMPPS/mysql/bin/mysqlcheck

#
# Setup.end


# Check and auto-repair all databases first
#
echo
echo "Checking all databases - this can take a while ..."
$MYSQL_CHECK -u $MYSQL_USR --password=$MYSQL_PWD --auto-repair --all-databases

# Backup
#
echo
echo "Starting backup ..."
mkdir -p "$BACKUP_DIR/$TIMESTAMP"
databases=`$MYSQL_CMD --user=$MYSQL_USR -p$MYSQL_PWD -e "SHOW DATABASES;" | grep -Ev "(Database|information_schema|performance_schema)"`
 
for db in $databases; do
	  echo "Dumping $db ..."
	    $MYSQL_DMP --force --opt --user=$MYSQL_USR -p$MYSQL_PWD --databases "$db" | gzip > "$BACKUP_DIR/$TIMESTAMP/$db.gz"
    done

    echo
    echo "Cleaning up ..."
    find $BACKUP_DIR -maxdepth 1 -mindepth 1 -type d -mtime +$HOLD_DAYS -exec rm -rf {} \;

    echo "-- DONE!"
