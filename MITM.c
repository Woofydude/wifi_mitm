#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Flipper.h>

char ssid[32];
char password[64];
char ap_ssid[32];
char ap_password[64];
char server_ip[16];
int server_port;

void setup() {
  Flipper.begin(); // start the Flipper Zero framework

  Flipper.display.println("Enter your WiFi SSID:");
  Flipper.keyboard.read(ssid, 32);
  Flipper.display.println("Enter your WiFi password:");
  Flipper.keyboard.read(password, 64);
  Flipper.display.println("Enter your AP SSID:");
  Flipper.keyboard.read(ap_ssid, 32);
  Flipper.display.println("Enter your AP password:");
  Flipper.keyboard.read(ap_password, 64);
  Flipper.display.println("Enter your logging server IP:");
  Flipper.keyboard.read(server_ip, 16);
  Flipper.display.println("Enter your logging server port:");
  server_port = Flipper.keyboard.readInt();

  WiFi.softAP(ap_ssid, ap_password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  WiFiServer server(80);
  server.begin();

  while (1) {
    WiFiClient client = WiFi.softAP.accept();
    if (client) {
      String request = client.readStringUntil('\r');
      String log_message = "Request: " + request;
      Flipper.display.scroll(log_message);

      WiFiClient web_client;
      if (web_client.connect("www.google.com", 80)) {
        web_client.print(request);

        while (web_client.connected()) {
          if (web_client.available()) {
            String response = web_client.readStringUntil('\r');
            String log_message = "Response: " + response;
            Flipper.display.scroll(log_message);
          }
        }

        web_client.stop();
      }

      client.stop();
    }
  }
}

void loop() {
  // The loop function is not used in this version
  delay(1000);