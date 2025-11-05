#ifndef BLEACHO_TCPSERVER_H
#define BLEACHO_TCPSERVER_H

#include <cstdint>
#include <string>

class tcpServer {
public:
    tcpServer(const std::string host = "0.0.0.0", uint16_t port = 7272);
    ~tcpServer();
private:
    std::string m_host;
    uint16_t m_port;
};
#endif