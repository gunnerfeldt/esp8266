#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ArduinoJson.h>

const char* ssid = "Ingrid Studio";
const char* password = "stockholm";

String WebPage = "<!DOCTYPE html><html><style>input[type=\"text\"]{width: 90%; height: 3vh;}input[type=\"button\"]{width: 9%; height: 3.6vh;}.rxd{height: 90vh;}textarea{width: 99%; height: 100%; resize: none;}</style><script>var Socket;function start(){Socket=new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage=function(evt){document.getElementById(\"rxConsole\").value +=evt.data;}}function enterpressed(){Socket.send(document.getElementById(\"txbuff\").value); document.getElementById(\"txbuff\").value=\"\";}</script><body onload=\"javascript:start();\"> <div><input class=\"txd\" type=\"text\" id=\"txbuff\" onkeydown=\"if(event.keyCode==13) enterpressed();\"><input class=\"txd\" type=\"button\" onclick=\"enterpressed();\" value=\"Send\" > </div><br><div class=\"rxd\"> <textarea id=\"rxConsole\" readonly></textarea> </div></body></html>";

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

void setup() {
  
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", [](){
    server.send(200, "text/html", WebPage);
    });
    
    server.begin();
    
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
    server.handleClient();
    if (Serial.available() > 0){
      char c[] = {(char)Serial.read()};
//      webSocket.broadcastTXT(c, sizeof(c));
    }
}


/*
 * Incoming websocket transfer
 * payload is char array of length
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   if (type == WStype_TEXT){
    for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
    Serial.println();
    webSocketReport();
   }
}

void webSocketReport(){


// char msg[] = "{\"ID\":1,\"type\":2}";
 char msg[] = {100,101,102};
Serial.print("msg: ");
Serial.println(msg);
Serial.print("Length: ");
Serial.println(sizeof(msg));
webSocket.broadcastTXT(msg, sizeof(msg));




/*
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
JSONencoder["device"] = "ESP32";
JSONencoder["sensorType"] = "Temperature";
JSONencoder["value"] = 23;
char JSONmessageBuffer[100];
JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

Serial.print("JSONmessageBuffer: ");
Serial.println(JSONmessageBuffer);
Serial.print("Length: ");
Serial.println(sizeof(JSONmessageBuffer));

webSocket.broadcastTXT(JSONmessageBuffer, sizeof(JSONmessageBuffer));
*/
}

