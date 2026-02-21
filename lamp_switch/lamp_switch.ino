#include <WiFi.h>

const char* ssid = "Airtel_shivamformal";
const char* password = "Tan@12345";

WiFiServer server(80);

// SAFE ESP32-C3 GPIOs
const int LED1 = 8;
const int LED2 = 4;
const int LED3 = 3;
const int LED4 = 2;
const int LED5 = 1;

void sendCorsHeaders(WiFiClient &client) {
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: GET, OPTIONS");
}

void sendText(WiFiClient &client, const String &body) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  sendCorsHeaders(client);
  client.println();
  client.print(body);
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
}

/* ---------- LOOP ---------- */
void loop() {
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
  } else if (req.indexOf("/led") != -1 || req.indexOf("/all/") != -1) {
    sendText(client, "ok\n");
  } else {
    String help = "ESP32 Floor Lamp API\n\n";
    help += "curl http://" + WiFi.localIP().toString() + "/ping\n";
    help += "curl http://" + WiFi.localIP().toString() + "/led1/on\n";
    help += "curl http://" + WiFi.localIP().toString() + "/led1/off\n";
    help += "curl http://" + WiFi.localIP().toString() + "/all/on\n";
    help += "curl http://" + WiFi.localIP().toString() + "/all/off\n";
    sendText(client, help);
  }

  client.stop();
}