#include "arduino_secrets.h"

#include <ArduinoClient.h>
#include <M5Tough.h>
#include <WiFi.h>

///////please enter your sensitive data in the Secret arduino_secrets.h
/////// WiFi Settings ///////
String ssid = SECRET_SSID;
String pass = SECRET_PASS;
String xm2mri = SECRET_XM2MRI;
String xm2morigin = SECRET_XM2MORIGIN;

char serverAddress[] = "localhost";
int port = 3000;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("client starting");

  M5.begin();
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.print("Starting..\n");
  M5.Lcd.printf("WIFI-SSID: %s\n", ssid);

  WiFi.begin(ssid, pass);
  M5.Lcd.print("\nConnecting Wifi...\n");
}

void loop() {
  M5.Lcd.setCursor(0,0);
  if((WiFi.status() == WL_CONNECTED)) {
    M5.Lcd.print("making GET request\n");
    client.beginRequest();
    client.get("/Mobius?rcn=1");
    client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
    client.sendHeader("X-M2M-Origin", xm2morigin);
    client.sendHeader("X-M2M-RI", xm2mri);
    client.endRequest();
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    if(statusCode == 200) {
      M5.Lcd.printf("[HTTP] GET... code: %d\n", statusCode);
      M5.Lcd.println(response);
      Serial.print("\nResponse: ");
      Serial.println(response);
    }else {
      M5.Lcd.printf("\n[HTTP] GET... failed, error: %d\n", statusCode);
    }
  }else{
    M5.Lcd.print("connect failed");
  }
  Serial.println("Wait five seconds");
  delay(5000);
  M5.Lcd.clear();
}
