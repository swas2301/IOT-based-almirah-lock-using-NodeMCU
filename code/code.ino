#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "Research_LAB";
const char* password = "rlab@uem_jaipur!303807&chomu";
 
void setup () {
 
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	pinMode(D1,OUTPUT);
	pinMode(D2,OUTPUT);
	//pinMode(D3,OUTPUT);

	while (WiFi.status() != WL_CONNECTED) {
	 
		 delay(1000);
		 Serial.println("Connecting..");
	 
	}
 
}
 
void loop() {
 
	if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
		HTTPClient http;  //Declare an object of class HTTPClient
		 
		http.begin("https://smpower.000webhostapp.com/node.php?id=doorlock&key=1234&input=off");  //Specify request destination
		int httpCode = http.GET();                                                                  //Send the request
		 
		if (httpCode > 0) { //Check the returning code
			digitalWrite(D2,HIGH);
			String payload = http.getString();   //Get the request response payload
			//Serial.println(payload);                     //Print the response payload
			if (payload.equals("on"))
			{
				Serial.println("led on");
				digitalWrite(D1,HIGH);
				//digitalWrite(D3,HIGH);
        delay(3000);
        digitalWrite(D1,LOW);
        //digitalWrite(D3,LOW);
			}
			else
			{
				Serial.println("led off");
				digitalWrite(D1,LOW);
				//digitalWrite(D3,LOW);
			}
			delay(30);
			digitalWrite(D2,LOW);
		}
		 
		http.end();   //Close connection
 
	}
 
	//delay(3000);    //Send a request every 3 seconds
 
}
