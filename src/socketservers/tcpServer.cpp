#include "tcpServer.h"

tcpServer::tcpServer(const std::string host, uint16_t port) : m_port(port), m_host(host) {
}

tcpServer::~tcpServer() {
}

