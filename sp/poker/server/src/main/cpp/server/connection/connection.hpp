#pragma once

#include <memory>
#include <thread>
#include "message/message.hpp"
#include "../../async/blocking_queue.hpp"
#include "../../async/safe_queue.hpp"

class connection {

private:
    int socket;

    bool is_alive_ = false;

    std::shared_ptr<blocking_queue<std::shared_ptr<message>>> received_messages;

    std::shared_ptr<blocking_queue<std::shared_ptr<message>>> send_queue;

    std::shared_ptr<std::jthread> receiving_thread;

    std::shared_ptr<std::jthread> sending_thread;

    void handle_receive();

    void handle_send();

    std::shared_ptr<message> receive_message();

public:
    explicit connection(int socket);

    void dispatch();

    int get_socket() const;

    bool is_alive() const;

    const std::shared_ptr<blocking_queue<std::shared_ptr<message>>> &get_received_messages() const;

    const std::shared_ptr<blocking_queue<std::shared_ptr<message>>> &get_send_queue() const;

    const std::shared_ptr<std::jthread> &get_receiving_thread() const;

    const std::shared_ptr<std::jthread> &get_sending_thread() const;

    void terminate_receiving_thread();

    void terminate_sending_thread();

    void terminate_threads();

    void send_message(std::shared_ptr<message> msg);
};
