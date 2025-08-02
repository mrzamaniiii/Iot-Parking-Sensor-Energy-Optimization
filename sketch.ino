#include <WiFi.h>
#include <esp_now.h>

#define TRIGGER_PIN 12  // Trigger pin
#define ECHO_PIN 13  // Echo pin
#define uS_TO_S_FACTOR 1000000
#define SLEEP_TIME 15e6  // Deep sleep duration (15 seconds)
#define THRESHOLD_DISTANCE 50

uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x84, 0xFB, 0x90};  // MAC address of the ESP32 sink node
esp_now_peer_info_t peerInfo;
bool transmissionDone = false;  // Flag to track when transmission is complete

// Measure distance using the HC-SR04
float getDistance() {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);  // Delay for stability
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return (duration * 0.0343) / 2;  // Convert time to distance in cm
}


// ESP-NOW callback function for transmission completion
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
    transmissionDone = true;  // Mark transmission as complete
}

// Send parking status using ESP-NOW
void sendStatus(const char* status) {
    transmissionDone = false;
    unsigned long transmissionStartTime = millis();  // Start transmission timer
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)status, strlen(status) + 1);

    if (result == ESP_OK) {
        Serial.println("Message sent successfully");
    } else {
        Serial.println("Error sending message");
    }

    // Wait for the callback to confirm transmission
    unsigned long waitStart = millis();
    while (!transmissionDone && millis() - waitStart < 100) {  // Timeout after 100ms
        delay(1);
    }

    unsigned long transmissionTime = millis() - transmissionStartTime;  // Calculate actual transmission time
    Serial.println("Transmission Time: " + String(transmissionTime) + " ms");
}

void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    Serial.print("Received Parking Status: ");
    Serial.println((char*)data);
}

void setup() {
    unsigned long startMillis = millis();  // Time of booting

    Serial.begin(115200);
    Serial.println("ESP32 Wake-Up Time: " + String(millis()) + " ms");  // Log wake-up time

    WiFi.mode(WIFI_STA);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        return;
    }

    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSent);  // Register callback for send completion

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("Enabling Wi-Fi 2sec...");
    unsigned long wifiEnableTime = millis();  // Time Wi-Fi turned on
    delay(2000);

    // Measure distance
    unsigned long sensorStartTime = millis();  // Start measuring time
    float distance = getDistance();
    unsigned long sensorReadingTime = millis() - sensorStartTime;  // Time spent on sensor reading

    Serial.print("Measured Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    const char* parkingStatus = (distance <= THRESHOLD_DISTANCE) ? "OCCUPIED" : "FREE";  // Determine parking status
    Serial.print("Parking Status: ");
    Serial.println(parkingStatus);

    // Send data
    sendStatus(parkingStatus);
    Serial.println("Disabling Wi-Fi 2sec...");
    WiFi.mode(WIFI_OFF);
    unsigned long wifiDisableTime = millis();  // Time Wi-Fi turned off
    delay(2000);

    // Display execution times
    Serial.println("Execution Time Logs");
    Serial.println("Booting Time: " + String(startMillis) + " ms");
    Serial.println("Wi-Fi Turn-on Time: " + String(wifiEnableTime) + " ms");
    Serial.println("Sensor Reading Time: " + String(sensorReadingTime) + " ms");
    Serial.println("Wi-Fi Turn-off Time: " + String(wifiDisableTime) + " ms");

    Serial.println("Entering Deep Sleep...");
    unsigned long deepSleepTime = millis();  // Time before deep sleep
    Serial.println("Time deep sleep: " + String(deepSleepTime) + " ms");

    Serial.flush();

    esp_sleep_enable_timer_wakeup(SLEEP_TIME);
    esp_deep_sleep_start();
}

void loop() {}