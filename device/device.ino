#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

/* Set these to your desired credentials. */
const char *ssid = "Research_LAB";  //ENTER YOUR WIFI SETTINGS
const char *password = "rlab@uem_jaipur!303807&chomu";


//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D8,INPUT);
  delay(1000);
  digitalWrite(D4,1);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  
  HTTPClient http;    //Declare object of class HTTPClient

   String getData, Link;
  
  //POST Data
  String postData;
  if (digitalRead(D8)==0) postData = "id=doorlock&key=1234&input_data=close";
  else  postData = "id=doorlock&key=1234&input_data=open";
 
  http.begin("http://18.221.122.79/postdata.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  int pos1 = payload.indexOf("[");
  int pos2 = payload.lastIndexOf("]");
  
  // Parse the string looking for ,
  String data = payload.substring(pos1 + 1, pos2);
  if (data.length()>4)
  {
      Serial.println("[" + data + "]");
      int idx1 = data.indexOf(',');
      int idx2 = data.indexOf(',', idx1 + 1);
      int idx3 = data.indexOf(',', idx2 + 1);
      int idx4 = data.indexOf(',', idx3 + 1);
      int relay1 = (data.substring(0, idx1)).toInt();
      int relay2 = (data.substring(idx1 + 1, idx2)).toInt();
      int relay3 = (data.substring(idx2 + 1, idx3)).toInt();
      int relay4 = (data.substring(idx3 + 1, idx4)).toInt();

      digitalWrite(D1,relay1);
      digitalWrite(D2,relay1);
      digitalWrite(D5,relay3);
      digitalWrite(D6,relay4);

      digitalWrite(D4,0);
  }

  http.end();  //Close connection
  
  delay(300);  //Post Data at every 5 seconds
  digitalWrite(D4,1);
  delay(200);
}
//=======================================================================
