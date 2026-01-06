#include <cstddef>
#include "Net.h"

void Net::init() {
    foof
    Network.begin();
    WiFi.STA.begin();
    WiFi.STA.connect(ssid, version);

    Serial.println("Connecting to WiFi...");
    while (WiFi.STA.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Network.setDefaultInterface(WiFi.STA);
}

void Net::start() {
    // Listen to multicast address
    udpp = new NetworkUDP();
    int success = 0;
    while (success == 0) {
      success = udpp->beginMulticast(mcast_addr, mcast_port);
      if (!success) {
        Serial.println("Failed to start UDP multicast, retrying...");
        delay(2000);
      }
    }
}

bool Net::poll(byte* buf, size_t bufsize, size_t& received) {
    int packetSize = udpp->parsePacket();
    if (packetSize) {
        // read the packet into the buffer
        received = udpp->read(buf, bufsize);
        return true;
    }
    return false;
}

void Net::stop() {
}
