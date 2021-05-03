mysql -u pi -praspberryaxel -v -v -v esp_data -e "delete from SensorData where (sensor = 'Temperature' and (value > 50 or value < -20)) or (sensor = 'Pressure' and (value > 103000 or value < 90000))"
