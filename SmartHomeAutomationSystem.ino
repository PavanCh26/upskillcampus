#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Pin Configuration
#define DHTPIN D4
#define DHTTYPE DHT11

#define RELAY1 D1
#define RELAY2 D2

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

// Relay Status
bool relay1State = false;
bool relay2State = false;

void handleRoot() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String html = "<!DOCTYPE html><html>";
  html += "<head><title>Smart Home Automation</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;margin-top:30px;}";
  html += "button{padding:12px 25px;margin:10px;font-size:18px;}";
  html += "</style></head><body>";

  html += "<h1>Smart Home Automation System Using IoT</h1>";

  html += "<h3>Temperature: ";
  html += String(temperature);
  html += " °C</h3>";

  html += "<h3>Humidity: ";
  html += String(humidity);
  html += " %</h3>";

  html += "<hr>";

  html += "<h2>Light 1</h2>";

  if(relay1State)
    html += "<a href='/light1off'><button>TURN OFF</button></a>";
  else
    html += "<a href='/light1on'><button>TURN ON</button></a>";

  html += "<h2>Light 2</h2>";

  if(relay2State)
    html += "<a href='/light2off'><button>TURN OFF</button></a>";
  else
    html += "<a href='/light2on'><button>TURN ON</button></a>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void light1On() {
  digitalWrite(RELAY1, HIGH);
  relay1State = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void light1Off() {
  digitalWrite(RELAY1, LOW);
  relay1State = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void light2On() {
  digitalWrite(RELAY2, HIGH);
  relay2State = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void light2Off() {
  digitalWrite(RELAY2, LOW);
  relay2State = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  dht.begin();

  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/light1on", light1On);
  server.on("/light1off", light1Off);

  server.on("/light2on", light2On);
  server.on("/light2off", light2Off);

  server.begin();

  Serial.println("Web Server Started");
}

void loop() {

  server.handleClient();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C");

  Serial.print(" | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}
