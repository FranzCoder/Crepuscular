# Crepuscular

Semplice progetto, sviluppato su Arduino Micro, per realizzare un crepuscolare dotato di relay e sensori di temperatura, umidità e luminosità, la comunicazione con il dispositivo avviene tramite protocollo MQTT.

Componenti hardware:
 - Arduino Micro
 - Sensore di temeperatura DHT22
 - Sensore di luminosità LM393
 - Modulo relay 1 canale 5V
 - Modulo WiFi ESP8266 ESP-01
 - Diodo 1N4148 o 1N4007
 - Regolatore di tensione AMS1117 5v/3.3v LM1117
 - Modulo di alimentazione HLK-PM01 220v/5v (necssario solo per alimentazione diretta da rete elettrica)
 
IDE e progettazione:
 - Visual Studio 2019 Community with Visual Micro (Arduino IDE for Visual Studio - https://www.visualmicro.com - Il progetto è comunque compilabile utilizzando l'IDE Arduino standard)
 - Fritzing 0.9.3b.64.pc (www.fritzing.org)

Librerie Arduino:
 - PubSubClient 2.7.0 (https://pubsubclient.knolleary.net/)
 - SimpleDHT 1.0.12 (https://github.com/winlinvip/SimpleDHT)
 - WiFiEsp 2.2.2 (https://github.com/bportaluri/WiFiEsp)
 
