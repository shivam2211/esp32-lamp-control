#include <WiFi.h>
#include <ArduinoOTA.h>
#include <pgmspace.h>
#include "index_html.h"

const char* ssid = "Airtel_shivamformal";
const char* password = "Tan@12345";

const char* otaPassword = "lampota123";  // OTA upload password (change to your own)

WiFiServer server(80);

// SAFE ESP32-C3 GPIOs
const int LED1 = 1; //
const int LED2 = 3; // Correct
const int LED3 = 0; //
const int LED4 = 4; //
const int LED5 = 2; //
const int LED_PINS[5] = { LED1, LED2, LED3, LED4, LED5 };

void sendCorsHeaders(WiFiClient &client) {
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: GET, OPTIONS");
}

void sendText(WiFiClient &client, const String &body) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
  client.println("Pragma: no-cache");
  client.println("Expires: 0");
  sendCorsHeaders(client);
  client.println();
  client.print(body);
}

void sendHtml(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=utf-8");
  client.println("Connection: close");
  client.println("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
  client.println("Pragma: no-cache");
  client.println("Expires: 0");
  client.println();
  const size_t len = strlen_P(INDEX_HTML);
  constexpr size_t chunkSize = 384;
  char buffer[chunkSize];
  for (size_t offset = 0; offset < len; offset += chunkSize) {
    const size_t n = (len - offset) > chunkSize ? chunkSize : (len - offset);
    memcpy_P(buffer, INDEX_HTML + offset, n);
    client.write(reinterpret_cast<const uint8_t*>(buffer), n);
  }
}

void sendJson(WiFiClient &client, const String &body) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
  client.println("Pragma: no-cache");
  client.println("Expires: 0");
  sendCorsHeaders(client);
  client.println();
  client.print(body);
}

bool isBulbOnByPin(int pin) {
  // Active-low wiring: LOW means bulb is physically ON.
  return digitalRead(pin) == LOW;
}

String buildStateJson() {
  String json;
  json.reserve(64);
  json += "{\"bulbs\":[";
  for (int i = 0; i < 5; i++) {
    if (i > 0) json += ',';
    json += isBulbOnByPin(LED_PINS[i]) ? "true" : "false";
  }
  json += "]}";
  return json;
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();

  ArduinoOTA.setHostname("lamp-switch");
  ArduinoOTA.setPassword(otaPassword);
  ArduinoOTA.begin();
  Serial.println("OTA ready");

}

/* ---------- LOOP ---------- */
void loop() {
  ArduinoOTA.handle();

  WiFiClient client = server.available();
  if (!client) return;

  client.setTimeout(5);
  String req = client.readStringUntil('\n');
  client.flush();
  req.trim();

  // Handle CORS preflight (OPTIONS)
  if (req.indexOf("OPTIONS") != -1) {
    client.println("HTTP/1.1 204 No Content");
    client.println("Connection: close");
    sendCorsHeaders(client);
    client.println();
    client.stop();
    return;
  }

  // Serve embedded web app at GET / or GET /index.html
  if (req.startsWith("GET /")) {
    int pathEnd = req.indexOf(' ', 4);
    String path = pathEnd > 4 ? req.substring(4, pathEnd) : "";
    if (path == "/" || path == "/index.html") {
      sendHtml(client);
      client.stop();
      return;
    }
  }

  if (req.indexOf("/led1/on")  != -1) digitalWrite(LED1, HIGH);
  if (req.indexOf("/led1/off") != -1) digitalWrite(LED1, LOW);
  if (req.indexOf("/led2/on")  != -1) digitalWrite(LED2, HIGH);
  if (req.indexOf("/led2/off") != -1) digitalWrite(LED2, LOW);
  if (req.indexOf("/led3/on")  != -1) digitalWrite(LED3, HIGH);
  if (req.indexOf("/led3/off") != -1) digitalWrite(LED3, LOW);
  if (req.indexOf("/led4/on")  != -1) digitalWrite(LED4, HIGH);
  if (req.indexOf("/led4/off") != -1) digitalWrite(LED4, LOW);
  if (req.indexOf("/led5/on")  != -1) digitalWrite(LED5, HIGH);
  if (req.indexOf("/led5/off") != -1) digitalWrite(LED5, LOW);

  if (req.indexOf("/all/on") != -1) {
    digitalWrite(LED1,HIGH); digitalWrite(LED2,HIGH);
    digitalWrite(LED3,HIGH); digitalWrite(LED4,HIGH); digitalWrite(LED5,HIGH);
  }

  if (req.indexOf("/all/off") != -1) {
    digitalWrite(LED1,LOW); digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW); digitalWrite(LED4,LOW); digitalWrite(LED5,LOW);
  }

  if (req.indexOf("/ping") != -1) {
    sendText(client, "ok\n");
  } else if (req.indexOf("/state") != -1) {
    sendJson(client, buildStateJson());
  } else if (req.indexOf("/led") != -1 || req.indexOf("/all/") != -1) {
    sendText(client, "ok\n");
  } else {
    String help = "ESP32 Floor Lamp API\n\n";
    help += "curl http://" + WiFi.localIP().toString() + "/ping\n";
    help += "curl http://" + WiFi.localIP().toString() + "/led1/on\n";
    help += "curl http://" + WiFi.localIP().toString() + "/led1/off\n";
    help += "curl http://" + WiFi.localIP().toString() + "/all/on\n";
    help += "curl http://" + WiFi.localIP().toString() + "/all/off\n";
    help += "curl http://" + WiFi.localIP().toString() + "/state\n";
    sendText(client, help);
  }

  client.stop();
}