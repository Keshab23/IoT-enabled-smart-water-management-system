#include <ESP8266WiFi.h>
String str="";
const char* ssid = "Kriyative";//type your ssid
const char* password = "Kriyative1!";//type your password

IPAddress ip(192,168,1,8);

WiFiClient client;
void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(9600);
  delay(10);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


  

}



 void loop()
 {
      
    
  
      if (client.connect(ip, 8080)) 
        {
            Serial.println("connected");
             // Make a HTTP request:
             client.println("GET /stat HTTP/1.1");
             client.println();
        } 
       else 
        {
          // if you didn't get a connection to the server:
          Serial.println("connection failed");
        }
          delay(1000); 
          //Reading the response from the server..
          while (client.available()) 
           {
            char c = client.read();
            str+= c;
            delay(50);
           }
        if(str.indexOf("ON")!=-1)
        {
          digitalWrite(LED_BUILTIN,LOW);
          Serial.println("on");
        }
      else
        {
         digitalWrite(LED_BUILTIN,HIGH);
         Serial.println("off");
        }
       }
