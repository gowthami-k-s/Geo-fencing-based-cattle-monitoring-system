
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define SS 5
#define RST 14
#define DIO0 2

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

unsigned long lastSend = 0;

void setup()
{
    Serial.begin(115200);

    // GPS RX = GPIO16, TX = GPIO17
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(433E6))
    {
        Serial.println("LoRa initialization failed!");
        while (1);
    }

    Serial.println("GPS LoRa Transmitter Ready");
}

void loop()
{
    while (gpsSerial.available())
    {
        gps.encode(gpsSerial.read());
    }

    if (millis() - lastSend >= 2000)
    {
        lastSend = millis();

        if (gps.location.isValid() &&
            gps.location.age() < 3000)
        {
            float lat = gps.location.lat();
            float lon = gps.location.lng();

            String data = String(lat, 6) + "," +
                          String(lon, 6);

            Serial.print("Latitude: ");
            Serial.println(lat, 6);

            Serial.print("Longitude: ");
            Serial.println(lon, 6);

            Serial.print("Sending GPS data: ");
            Serial.println(data);

            LoRa.beginPacket();
            LoRa.print(data);
            LoRa.endPacket();

            Serial.println("Packet sent successfully\n");
        }
        else
        {
            Serial.println("Waiting for valid GPS fix...");
        }
    }
}
