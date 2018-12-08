#define trigPin D1
#define echoPin D2
#include <ESP8266WiFi.h>
const char* ssid = "My ASUS";//type your ssid
const char* password = "sripad1996";//type your password
IPAddress ip(192,168,43,97);
int d=50;


WiFiClient Client;
int realdistance;
void setup() 
{
  
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.println("OVERHEAD TANK");
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}

 void loop()
 {
    ////////////////////////////////////////////////////////
    /////  Calculating distance 
    long duration, distance;
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); //10 micro second pulse emitted
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); //pulse duration counted
    //distance = (340*duration)/20000;  //in cm
    distance = (340*duration)/10000;
    realdistance=d-distance;
    Serial.println(realdistance);
    ////////////////////////////////////////////////////////////
    //
    //Connecting to the server
  
    if(Client.connect(ip,8080))
    {
    
      Client.println("GET /deptho/"+(String)realdistance+" HTTP/1.1");
      Client.stop(); 
    }
    else
    {
      Serial.println("Not connected");
      delay(100);
    
    }
     delay(2000);
  }
