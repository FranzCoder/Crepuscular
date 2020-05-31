#include <WiFiEsp.h>
#include <PubSubClient.h>

WiFiEspClient espClient;
PubSubClient mqttClient (espClient);

char* mqttSsid;
char* mqttKey;
char* mqttServer;
int mqttPort;
char* mqttUser;
char* mqttPassword;

String mqttTopicsList;

void MQTTInit (char* ssid, char* key, IPAddress staticIP, IPAddress server, int port, char* user, char* password)
{
  Serial1.begin (115200);

  // Initialize ESP Module
  WiFi.init (&Serial1);
  WiFi.config (staticIP);
  WiFi.begin (ssid, key);

  Serial.println ("Checking WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(1000);
  }
  Serial.println ("WiFi connected");

  mqttUser = user;
  mqttPassword = password;
  mqttClient.setServer(server, port);
  mqttClient.setCallback (messageReceived);
  
  // Allow the hardware to sort itself out
  delay(1500);
}

void MQTTPublish (char* topic, char* message)
{
  mqttClient.publish (topic, message);
}

void MQTTSubscribe (char* topic)
{
  mqttTopicsList = mqttTopicsList + topic + "|";
}

void MQTTConnect()
{
  // Loop until connection
  while (!mqttClient.connected ())
  {
    Serial.println ("Attempting to MQTT connection...");

    if (mqttClient.connect("Crepuscular", mqttUser, mqttPassword, topicAvailability, 0, false, "false"))
    {
      Serial.println ("MQTT Connected");
      
      int iPosDelim = 0;
      int iPosStart = 0;
  
      Serial.println("Topic subscribed: ");    
      do
      {
        iPosDelim = mqttTopicsList.indexOf("|", iPosStart);
        if (iPosDelim > (iPosStart + 1))
        {
          char topic[30];
          int topicLength;
          topicLength = mqttTopicsList.substring(iPosStart, iPosDelim).length() + 1;
          mqttTopicsList.substring(iPosStart, iPosDelim).toCharArray (topic, topicLength);
          
          Serial.println(topic);
          
          mqttClient.subscribe (topic);
          iPosStart = iPosDelim + 1;
        }      
      }
      while (iPosDelim >= 0);

      delay (1000);
      MQTTPublish (topicAvailability, "true");
    }
    else
    {
      Serial.print ("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" - Retry in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void MQTTLoop()
{
  mqttClient.loop();
}

void messageReceived(char* topic, byte* payload, unsigned int length)
{
  String strTopic(topic);
  String strPayload;

  for (int i = 0; i < length; i++)
  {
    strPayload += " ";
    strPayload[i] = ((char)payload[i]);
  }
  
  MQTTCallback (strTopic, strPayload);
}
