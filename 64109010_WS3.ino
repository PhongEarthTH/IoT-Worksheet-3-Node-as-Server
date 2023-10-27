#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const int ledPin = D6; // กำหนดตามขาที่เชื่อมต่อ LED
bool ledStatus = false; // false = OFF, true = ON

DHT dht14(D4, DHT11);
ESP8266WebServer server(80);

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial. print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String html = "<html><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }";
  html += ".button { padding: 10px 20px; background-color: #0099FF; color: white; border: none; border-radius: 5px; cursor: pointer; }";
  html += ".button:hover { background-color: #0077CC; }";
  html += "</style>";
  html += "</head><body>";

  html += "<h2>สถานะไฟ LED D6: " + String(ledStatus ? "ON" : "OFF") + "</h2>";
  html += "<a href=\"/toggle\"><button class=\"button\">Switch LED</button></a><br><br>";

  html += "ความชื้น (HUM): " + String(dht14.readHumidity()) + " %<br>";
  html += "อุณหภูมิ (TM): " + String(dht14.readTemperature()) + " C<br>";

  html += "</body></html>";
  server.send(200, "text/html; charset=UTF-8", html);
}


void handleToggle() {
  ledStatus = !ledStatus;
  digitalWrite(ledPin, ledStatus ? HIGH : LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(9600);
  init_wifi("Galaxy A71 96DE", "25452002");

  pinMode(ledPin, OUTPUT);

  dht14.begin();

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();

  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(1000);
}
