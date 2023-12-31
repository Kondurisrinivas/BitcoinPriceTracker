 #include <ESP8266WiFi.h>                                          
#include <ArduinoJson.h>
#include <Wire.h>
 
                                                               
                                                                  // WiFi settings
const char* ssid     = "xxxxxxxxxxxx";                            //replace with your SSID
const char* password = "xxxxxxxxxxxx";                            //replace with your PW
 
                                                                  // API server
const char* host = "api.coindesk.com";
 
void setup() {
 
                                                                  // Serial
  Serial.begin(115200);
  delay(10);
 
                                                                  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
 
                                                                 // Connect to API
  Serial.print("connecting to ");
  Serial.println(host);
  
                                                                 // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
                                                                // We now create a URI for the request
  String url = "/v1/bpi/currentprice.json";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
                                                                // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");


// After concatenating all the lines, the complete HTTP GET request looks like this:
// GET /v1/bpi/currentprice.json HTTP/1.1
// Host: api.coindesk.com
// Connection: close


  delay(100);
  
                                                                // Read all the lines of the reply from server and print them to Serial
  String answer;
  while(client.available()){
    String line = client.readStringUntil('\r');
    answer += line;
  }
 
  client.stop();
  Serial.println();
  Serial.println("closing connection");
 
                                                                // Process answer
  Serial.println();
  Serial.println("Answer: ");
  Serial.println(answer);
 
                                                                // Convert to JSON
  String jsonAnswer;
  int jsonIndex;
 
  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }
 
                                                                // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(jsonAnswer);
  jsonAnswer.trim();
 
                                                                // Get rate as float
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  float price = priceString.toFloat();
 
                                                                // Print price
  Serial.println();
  Serial.println("Bitcoin price: ");
  Serial.println(price);
                                                                // Wait 5 seconds
  delay(5000);
}
