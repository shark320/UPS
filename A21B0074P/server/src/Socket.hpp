#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <stdexcept>

class Socket {
private:
    int skt;  // Integer representing the socket descriptor.

public:
    /**
     * @brief Constructor for the Socket class.
     * @param domain The communication domain for the socket (e.g., AF_INET for IPv4).
     * @param type The socket type (e.g., SOCK_STREAM for a stream socket).
     * @param protocol The protocol to be used for the socket (e.g., IPPROTO_TCP for TCP).
     * @throws std::runtime_error if the socket creation fails.
     */
    Socket(int domain, int type, int protocol);

    /**
     * @brief Destructor for the Socket class.
     *        Closes the socket when the object is destroyed.
     */
    ~Socket();

    /**
     * @brief Deleted copy constructor to prevent copying of Socket objects.
     */
    Socket(const Socket&) = delete;

    /**
     * @brief Conversion operator to int.
     *        Allows the Socket object to be used as an integer socket descriptor.
     * @return The socket descriptor as an integer.
     */
    operator int() const;
};

#endif //SERVER_SOCKET_HPP
