# Мониторниг климата в серверной

Для проекта были использованы Arduino UNO с датчиками темепературы/влажности DHT11 и Ethernet Shield W5500. 
При обращении через веб-интерфейс Arduino возаращает данные датчиков в формате JSON.
Данные обрабатываются сервером Icinga/Zabbix.