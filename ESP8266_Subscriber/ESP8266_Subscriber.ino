/****************************************************************************
* Author:      Erhan YILMAZ                                                 *
* Application: MQTT Test application(Subscriber)                            *
* Date :       02.08.2016                                                   *
* Version :    1.0                                                          *
* Description: It is a subscriber part of a mqtt test application           *
* which is works with NodeMCU(ESP8266).                                     *
* It subscribes to "/sensors/#" topic to get all messages under /sensors/   *
* As broker mosquitto used on Ubuntu based pc.                              *
* MQTT deneme uygulaması                                                    *
* Gilberto Conti tarafından yazılan MQTT kütüphanesi kullanılmıştır.        *
* https://github.com/256dpi/arduino-mqtt buradan kütüphaneye erişilebilir.  *
* Uygulama MQTT client olarak çalışıp /sensors/ altında gelen topiclerde ki *                    
* mesajları almaktadır(Subscribe). Alınan mesajları uarttan pcye gönderir.	*
****************************************************************************/

// Libraries
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

// Wifi info
const char *ssid = "SSID_NAME";
const char *pass = "SSID_PASS";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

// Setup for used modules/libraries(Uart,MQTT,WIFI)
void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  // Use your Broker ip address
  client.begin("192.168.1.8", net); // Begin client with broker Ip and wifi client

  connect();
}

void connect()
{
  // Connect to wifi
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // Connect to broker
  Serial.print("\nconnecting...");
  while (!client.connect("ESP8266Sub", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/sensors/#");   // Subscription for sensors
  // client.unsubscribe("/example");
}

// Main loop
void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability
}

// Send incoming topic messages over uart
void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
