#!/bin/bash


empty=$(mysql -u $MYSQL_USER -p$MYSQL_PW -v -v esp_data -e "SELECT * FROM SensorData WHERE location = '$1' AND unix_timestamp(reading_time) > (unix_timestamp(current_time()) - 60*$2) " | grep -w "Empty set" | wc -l)

if [ $empty -gt 0 ]
then
	echo "No Data from sensor = '$1' in the last $2 minutes" >&2
fi
