
#include <SPI.h>
#include <LoRa.h>
#include <HardwareSerial.h>
#include <math.h>

#define SS 5
#define RST 14
#define DIO0 2
#define BUZZER 4

HardwareSerial gsm(1);

// Replace with your phone number
const char PHONE_NUMBER[] = "+91XXXXXXXXXX";

// RSSI calibration values
// These are starting values. Calibrate using your own modules.
const float RSSI_AT_1M = -45.0;
const float PATH_LOSS_N = 2.5;

// Zone limits in metres
const float SAFE_ZONE = 5.0;
const float WARNING_ZONE = 7.0;
const float ALERT_ZONE = 10.0;

const unsigned long SMS_INTERVAL = 30000;

unsigned long lastOutsideSMS = 0;

int previousZone = -1;

float filteredRSSI = 0;
bool firstReading = true;

String locationLink;

// Function to estimate distance from RSSI
float calculateDistance(float rssi)
{
    float distance = pow(
        10.0,
        (RSSI_AT_1M - rssi) / (10.0 * PATH_LOSS_N)
    );

    return distance;
}

// Function to send SMS
void sendSMS(String message)
{
    Serial.println("Sending SMS...");

    gsm.println("AT+CMGF=1");
    delay(1000);

    gsm.print("AT+CMGS=\"");
    gsm.print(PHONE_NUMBER);
    gsm.println("\"");

    delay(1000);

    gsm.print(message);
    gsm.write(26);

    delay(5000);

    Serial.println("SMS command completed");
    Serial.println(message);
}

// Function to check zone
int getZone(float distance)
{
    if (distance <= SAFE_ZONE)
        return 0;

    else if (distance <= WARNING_ZONE)
        return 1;

    else if (distance <= ALERT_ZONE)
        return 2;

    else
        return 3;
}

void setup()
{
    Serial.begin(115200);

    // GSM RX = GPIO16, TX = GPIO17
    gsm.begin(9600, SERIAL_8N1, 16, 17);

    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(433E6))
    {
        Serial.println("LoRa initialization failed!");
        while (1);
    }

    Serial.println("RSSI GPS Receiver Ready");
}

void loop()
{
    int packetSize = LoRa.parsePacket();

    if (packetSize)
    {
        String data = "";

        while (LoRa.available())
        {
            data += (char)LoRa.read();
        }

        int commaIndex = data.indexOf(',');

        if (commaIndex == -1)
        {
            Serial.println("Invalid GPS packet");
            return;
        }

        float lat = data.substring(0, commaIndex).toFloat();
        float lon = data.substring(commaIndex + 1).toFloat();

        // Read RSSI of the received LoRa packet
        float currentRSSI = LoRa.packetRssi();

        // Apply simple RSSI smoothing
        if (firstReading)
        {
            filteredRSSI = currentRSSI;
            firstReading = false;
        }
        else
        {
            filteredRSSI =
                (0.7 * filteredRSSI) + (0.3 * currentRSSI);
        }

        float distance = calculateDistance(filteredRSSI);

        locationLink = "https://maps.google.com/?q=" +
                       String(lat, 6) + "," +
                       String(lon, 6);

        Serial.println("\n---------- CATTLE STATUS ----------");

        Serial.print("Latitude: ");
        Serial.println(lat, 6);

        Serial.print("Longitude: ");
        Serial.println(lon, 6);

        Serial.print("RSSI: ");
        Serial.print(currentRSSI);
        Serial.println(" dBm");

        Serial.print("Filtered RSSI: ");
        Serial.println(filteredRSSI);

        Serial.print("Estimated Distance: ");
        Serial.print(distance);
        Serial.println(" metres");

        int currentZone = getZone(distance);

        if (currentZone == 0)
        {
            Serial.println("ZONE: SAFE");

            digitalWrite(BUZZER, LOW);

            if (previousZone != 0)
            {
                // No SMS in the 0-5 metre safe zone
                Serial.println("Cattle is in safe zone");
            }
        }

        else if (currentZone == 1)
        {
            Serial.println("ZONE: SAFE BOUNDARY");

            digitalWrite(BUZZER, LOW);

            if (previousZone != 1)
            {
                sendSMS(
                    "Cattle crossed 5 metres but is within "
                    "the safe boundary.\nLocation:\n" +
                    locationLink
                );
            }
        }

        else if (currentZone == 2)
        {
            Serial.println("ZONE: WARNING");

            digitalWrite(BUZZER, LOW);

            if (previousZone != 2)
            {
                sendSMS(
                    "WARNING! Cattle is approaching the "
                    "boundary (7-10 metres).\nLocation:\n" +
                    locationLink
                );
            }
        }

        else
        {
            Serial.println("ZONE: OUT OF BOUNDARY");

            digitalWrite(BUZZER, HIGH);

            if (previousZone != 3 ||
                millis() - lastOutsideSMS >= SMS_INTERVAL)
            {
                sendSMS(
                    "ALERT! Cattle crossed the 10 metre "
                    "boundary.\nLocation:\n" +
                    locationLink
                );

                lastOutsideSMS = millis();
            }
        }

        previousZone = currentZone;

        Serial.println("-----------------------------------\n");
    }
}
