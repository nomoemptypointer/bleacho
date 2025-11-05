#ifndef BLEACHO_HTTPSERVER_H
#define BLEACHO_HTTPSERVER_H

#include <cstdint>
#include <string>

class httpServer {
public:
    httpServer(const std::string host = "0.0.0.0", uint16_t port = 80);
    ~httpServer();
private:
    std::string m_host;
    uint16_t m_port;
};
#endif