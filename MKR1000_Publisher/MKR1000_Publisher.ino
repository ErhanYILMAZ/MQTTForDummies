/****************************************************************************
* Author:      Erhan YILMAZ                                                 *
* Application: MQTT Test application(Publisher)                            *
* Date :       02.08.2016                                                   *
* Version :    1.0                                                          *
* Description: It is a publisher part of a mqtt test application            *
* which is works with arduino mkr1000.                                      *
* It gets 3 channels temp values with LM35 sensors then send values         *
* as topics to subscribers.                                                 *
* As broker mosquitto used on Ubuntu based pc.                              *
* MQTT deneme uygulaması                                                    *
* Gilberto Conti tarafından yazılan MQTT kütüphanesi kullanılmıştır.        *
* https://github.com/256dpi/arduino-mqtt buradan kütüphaneye erişilebilir.  *
* Uygulama MQTT client olarak çalışıp LM35 sıcaklık sensörlerinden ölçtüğü  *
* değerleri topicler halinde yayınlamaktadır(Publish).                      *
****************************************************************************/

// Libraries
#include <SPI.h>
#include <WiFi101.h>
#include <MQTTClient.h>

// Wifi info
char *ssid = "SSID_NAME";
char *pass = "SSID_PASS";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

// Setup for used modules/libraries(ADC,Uart,MQTT,WIFI)
void setup()
{
  Serial.begin(115200);
  analogReference(AR_INTERNAL1V0);
  analogReadResolution(12);
  WiFi.begin(ssid, pass);
  client.begin("192.168.1.8", net); // Begin client with broker Ip and wifi client

  connect();
}

void connect()
{
  // Connect to wifi
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  
  // Connect to broker
  Serial.print("\nconnecting...");
  while (!client.connect("arduinoPub", "try", "try"))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  // client.subscribe("/example");    // No subscription for any topic
  // client.unsubscribe("/example");  // No unsubscription aswell
}

void loop() 
{
  char temp[50];

  client.loop();

  if(!client.connected()) 
  {
    connect();
  }

  // publish a messages roughly every 5 seconds.
  if(millis() - lastMillis > 5000) 
  {
    lastMillis = millis();
    int sensorValue = analogRead(A0);
    float temperature = sensorValue / 40.96;
    sprintf(temp,"%.2f°C", temperature);
    
    // /sensors/temp1 topic for temp sensor 1
    client.publish("/sensors/temp1", temp);
    
    sensorValue = analogRead(A1);
    temperature = sensorValue / 40.96;
    sprintf(temp,"%.2f°C", temperature);
    // /sensors/temp2 topic for temp sensor 2
    client.publish("/sensors/temp2", temp);

    sensorValue = analogRead(A2);
    temperature = sensorValue / 40.96;
    sprintf(temp,"%.2f°C", temperature);
    // /sensors/temp3 topic for temp sensor 3
    client.publish("/sensors/temp3", temp);
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) 
{
  // Callback function for subscriptions
}
