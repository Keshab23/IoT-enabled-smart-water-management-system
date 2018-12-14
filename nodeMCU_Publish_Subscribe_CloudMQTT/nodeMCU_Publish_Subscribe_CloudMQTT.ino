 #include <ESP8266WiFi.h>
 #include <PubSubClient.h>
 using namespace std;

 // WiFi credentials.
const char* WIFI_SSID = "Keshab";
const char* WIFI_PASS = "arpanet12";
const char* device_id = "water1";


// MQTT credentials
const char* mqtt_server = "m15.cloudmqtt.com";
const int mqtt_port = 15400;
const char* mqtt_user = "bhjdgsiu";
const char* mqtt_password = "rPoyUPthnjdI";

//Topic in cloudMQTT
#define in_topic "WaterManagement/LevelData"
#define out_topic "WaterManagement/Control"

int digitalPin;
//espMQTT client import
WiFiClient espClient;
PubSubClient client(espClient);
String m;
  
//code to be run when mqtt subscriber receives something
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.print(topic);
  Serial.println();
  Serial.print("Message: ");
  
  for(int i=0; i<length; i++){
    m+=(char)payload[i];
    //Serial.println(m[i]);
    //Serial.print((char)payload[i]);
    
    }
    Serial.println();
    Serial.println("---------------------");
    if(m=="on")
    {
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.println("ON Received");
    }
    if(m=="off")
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.println("OFF Received");
    }
    m="";
    
  }

void connect() {

  // Connect to Wifi.
  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(WIFI_SSID);

   // WiFi fix: https://github.com/esp8266/Arduino/issues/2186
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());



  // Connect to MQTT.
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println();
  Serial.println("Connecting to CloudMQTT...");

  while(!client.connected()) {
    delay(500);

    if(client.connect(device_id, mqtt_user, mqtt_password)){
      Serial.println("Connected to CloudMQTT");
      Serial.println();
      Serial.println("Connected to WaterManagement Topic");
      }
      else{
        Serial.println("failed with state ");
        Serial.println(client.state());
        }

      Serial.print(".");
  }

}



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Water Level");
  Serial.println("--------------------------------------");
  delay(2000);
  connect();

}

void loop() {

  bool toReconnect = false;
  digitalWrite(LED_BUILTIN,LOW);
  //If I lose connection reconnect
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    toReconnect = true;
  }

  if(!client.connected()) {
    Serial.println("Disconnected from CloutMQTT");
    toReconnect = true;
  }

  if(toReconnect) {
    connect();
  }
  delay(1000);
  client.loop();
  int lev = random(1,100);
  String l= String(lev);
  client.publish(in_topic,l.c_str());
  client.subscribe(out_topic);
  

  delay(1000);
}
