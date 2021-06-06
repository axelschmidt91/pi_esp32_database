#!/bin/bash

DBTABLE=SensorData
DBTABLE_OLD=SensorData_old
DAYS_IN_PAST=7

mysql -u $MYSQL_USER -p$MYSQL_PW -v -v esp_data -e "CREATE TABLE IF NOT EXISTS $DBTABLE_OLD LIKE $DBTABLE;"
mysql -u $MYSQL_USER -p$MYSQL_PW -v -v esp_data -e "INSERT INTO $DBTABLE_OLD SELECT * FROM $DBTABLE WHERE NOT EXISTS( SELECT * FROM $DBTABLE_OLD WHERE $DBTABLE.id=$DBTABLE_OLD.id);"
mysql -u $MYSQL_USER -p$MYSQL_PW -v -v esp_data -e "DELETE FROM $DBTABLE WHERE unix_timestamp(reading_time) < (unix_timestamp(current_time()) - 60*60*24*$DAYS_IN_PAST );"
