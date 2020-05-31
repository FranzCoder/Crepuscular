#include <PubSubClient.h>
#include <WiFiEsp.h>

#pragma region Settings
char* ssid = "SSID";
char* key = "WPA";
IPAddress myIPAddress(192, 168, 1, 100);
IPAddress mqttServerIP (192, 168, 1, 10);
int mqttServerPort = 1883;
char* mqttUsername = "mqttuser";
char* mqttPasswd = "mqttpassword";
char* topicState = "sensor/crep/relay/0";
char* topicAvailability = "sensor/crep/online";
char* topicRelayCommand = "sensor/crep/relay/0/command";
char* topicCrepuscularCommand = "sensor/crep/command";
char* topicBrightness = "sensor/crep/info/bright";
char* topicTemperature = "sensor/crep/info/temp";
char* topicHumidity = "sensor/crep/info/hum";

const long intervalTemp = 10 * 60000;
const long intervalLight = 5 * 60000;
const long intervalState = 0.5 * 60000;
const long intervalMQTT = 100;

const float tempThreshold = 1;
const float humThreshold = 0.5;
#pragma endregion

#pragma region PIN Settings
const int pinRele = 2;
const int pinTemp = 3;
const int pinLight = A0;
#pragma endregion

#pragma region Global variables
float temperature = 0;
float humidity = 0;

long lastTemp = -intervalTemp;
long lastLight = -intervalLight;
long lastMQTT = -intervalMQTT;
long lastState = -intervalState;

String relePrevState = "off";

float lastTempValue = 0;
float lastHumValue = 0;
#pragma endregion

void setup() {
  Serial.begin (115200);
  pinMode(LED_BUILTIN, OUTPUT);

  releSetOutputPin (pinRele);
  tempSetInputPin (pinTemp);
  lightSetInputPin (pinLight);
  digitalWrite (LED_BUILTIN, HIGH);
  releStateChange(HIGH);
  
  MQTTInit(ssid, key, myIPAddress, mqttServerIP, mqttServerPort, mqttUsername, mqttPasswd);
  MQTTSubscribe (topicRelayCommand);
  MQTTSubscribe (topicCrepuscularCommand);
}

void loop() {
  long currentTime = millis();
  int brightnessValue = 0;
  int err = 0;

  if (currentTime - lastMQTT > intervalMQTT)
  {
    MQTTConnect();
    MQTTLoop();
    lastMQTT = currentTime;
  }

  // Invio messaggio di stato corrente
  if (currentTime - lastState > intervalState)
  {
    char buff[4];
    relePrevState.toCharArray(buff, 4);
    MQTTPublish (topicState,buff); 
    lastState = currentTime;
  }
  
  // Lettura temperatura e umidità
  if (currentTime - lastTemp > intervalTemp)
  {
    if (err = tempReadData (temperature, humidity) == 0)
    {
      //Serial.print (temperature);
      //Serial.print ( " °C - ");
      //Serial.print (humidity);
      //Serial.println (" RH%");
      char buff[8]; // Buffer big enough for 7-character float
      dtostrf(temperature, 0, 2, buff); // Leave room for too large numbers!
      //Serial.println (buff);
      if (temperature < lastTempValue -  tempThreshold || temperature > lastTempValue + tempThreshold)
      { 
        MQTTPublish (topicTemperature, buff);
        lastTempValue = temperature;
      }
      dtostrf(humidity, 0, 2, buff); 
      if (humidity < lastHumValue - humThreshold || humidity > lastHumValue + humThreshold)
      { 
        MQTTPublish (topicHumidity, buff);
        lastHumValue = humidity;
      }
    }
    else
    {
      Serial.print ("Temp. read error: ");
      Serial.println (err);
    }
    lastTemp = currentTime;
  }

  // Lettura luminosità
  if (currentTime - lastLight > intervalLight)
  {
    brightnessValue = lightReadData ();
    if (brightnessValue != 0)
    {
      //Serial.print (brightnessValue);
      //Serial.println (" Lumens");
 
      char buff[4]; // Buffer big enough for 4-character float
      dtostrf(brightnessValue, 0, 0, buff);
      MQTTPublish (topicBrightness, buff);
    }
    else
    {
      Serial.print ("Brightness read error: ");
      Serial.println (err);
    }
    lastLight = currentTime;
  }
}

void MQTTCallback (String &topic, String &payload)
{
  Serial.println ("Incoming: " + topic + " - " + payload);
  if (topic == topicRelayCommand)
  {
    if (payload == "on")
    {
      if (relePrevState != payload)
      {
         releStateChange(LOW);
         MQTTPublish (topicState, "on");
      }
      relePrevState = "on";
    }
    if (payload == "off")
    {
      if (relePrevState != payload)
      {
        releStateChange(HIGH);
        MQTTPublish (topicState, "off");
      }
      relePrevState = "off";
    }
  }
  if (topic == topicCrepuscularCommand)
  {
    if (payload == "announce") MQTTPublish (topicAvailability, "true");
  }
}
