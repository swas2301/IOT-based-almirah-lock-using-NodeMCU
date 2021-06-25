#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "LOCK";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LEDpin = D1;
bool LEDstatus = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/on", handle_ledon);
  server.on("/off", handle_ledoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LEDstatus)
  digitalWrite(LEDpin, HIGH);
  else
  digitalWrite(LEDpin, LOW);
}

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title>Lock Control</title>\n";
  ptr += "<meta charset=UTF-8>";
  ptr += "<meta name=viewport ptr=width=device-width>";
  ptr += "<style type=text/css>";
  ptr += "body {";
  ptr += "margin: 0px;";
  ptr += "backgound-color: #FFFFFF;";
  ptr += "font-family: helvetica, arial;";
  ptr += "font-size: 100%;";
  ptr += "color: #555555;";
  ptr += "}";
  ptr += "td {";
  ptr += "text-align: center;";
  ptr += "}";
  ptr += "span {";
  ptr += "font-family: helvetica, arial;";
  ptr += "font-size: 70%;";
  ptr += "color: #777777;";
  ptr += "}";
  ptr += "button {";
  ptr += "width: 25%;";
  ptr += "height:100%;";
  ptr += "font-family: helvetica, arial;";
  ptr += "font-size: 100%;";
  ptr += "color: #555555;";
  ptr += "background: #BFDFFF;";
  ptr += "padding: 5px 5px 5px 5px;";
  ptr += "border: none;";
  ptr += "}";
  ptr += "</style>";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>E-LOCK</h1>\n";
  ptr +="<form method=\"get\">\n";
  if(led)
  ptr +="<button type=button style=background:#BFFFCF onclick=\"window.location.href='/off'\">LOCK OFF</button>\n";
  else
  ptr +="<button type=button style=background:#BFFFCF onclick=\"window.location.href='/on'\">LOCK ON</button>\n";
  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
