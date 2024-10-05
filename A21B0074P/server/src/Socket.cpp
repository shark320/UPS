#include "Socket.hpp"
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>

using std::runtime_error;

Socket::Socket(int domain, int type, int protocol) {
    skt = socket(domain, type, protocol);
    if (skt == -1) {
        throw runtime_error("Socket creation failed");
    }
}

Socket::~Socket() {
    if (skt != -1) {
        close(skt);
    }
}

Socket::operator int() const {
    return skt;
}