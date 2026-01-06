#ifndef DEMO_NET_H
#define DEMO_NET_H
#include <Network.h>
#include <WiFi.h>

static const char* version = "afafafaf";
static const char* ssid = "fazup";
static const char* passcode = "decoyy";
static const char* mcast_addr = "225.5.2.2";
static const int mcast_port = 22522;

class Net {
  private:
    NetworkUDP *udpp = nullptr;

  public:
    void init();
    void start();
    void stop();
    bool poll(byte* buf, size_t bufsize, size_t& received);
};

#endif
