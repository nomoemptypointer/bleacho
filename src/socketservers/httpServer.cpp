#include "httpServer.h"

httpServer::httpServer(const std::string host, uint16_t port) : m_port(port), m_host(host) {
}

httpServer::~httpServer() {
}
