#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include<SPI.h>
#include<MFRC522.h>

#define RST_PIN D3
#define SS_PIN D8

MFRC522 rfid(SS_PIN, RST_PIN);

const char* ssid = "";
const char* password = "";

WiFiServer server(4444);


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  MDNS.begin("writer4");
  server.begin();
  SPI.begin();
  rfid.PCD_Init();
  digitalWrite(LED_BUILTIN, LOW);
}




void loop()
{
  String uid = "";
  MDNS.update();
  rfid.PCD_Init();
  WiFiClient client = server.available();
  if (client)
  {
    
    if (client.connected())
    {
      
      if (!rfid.PICC_IsNewCardPresent()) 
      {
        client.print("FAILED");
      }
      else if (! rfid.PICC_ReadCardSerial()) 
      {
        client.print("FAILED");
        
      }
      else{
        for(int i=0;i<4;i++)uid.concat(String(rfid.uid.uidByte[i], HEX));
        client.print(uid);
      }
      client.stop();
    }
  }
 rfid.PICC_HaltA();
}
