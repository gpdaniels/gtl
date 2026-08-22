/*
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <testbench/main.tests.hpp>

#include <testbench/comparison.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

#include <execution/barrier>
#include <io/socket>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

class closed_port {
private:
    gtl::socket::tcp_server config_bound = {};
    gtl::socket socket_bound;

public:
    closed_port() {
        REQUIRE(this->socket_bound.open(gtl::socket::tcp_server{ gtl::socket::ip_any, gtl::socket::port_any }, false) == true);
        REQUIRE(this->socket_bound.get_config(this->config_bound.address, this->config_bound.port) == true);
    }

    unsigned short get_port() const {
        return this->config_bound.port;
    }
};

class local_server {
private:
    gtl::socket::tcp_server config_server = {};
    gtl::socket socket_server;
    bool socket_server_opened = false;
    std::atomic<bool> server_running = true;
    std::thread thread_server;
    std::mutex mutex_client;
    gtl::socket socket_client;
    gtl::socket socket_client_initial;
    unsigned int accepted_connections = 0;

public:
    ~local_server() {
        this->server_running = false;
        gtl::socket socket_wakeup;
        static_cast<void>(socket_wakeup.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, this->config_server.port }));
        this->thread_server.join();
    }

    local_server() {
        gtl::barrier barrier(2);
        this->thread_server = std::thread([this, &barrier]() {
            this->socket_server_opened = this->socket_server.open(gtl::socket::tcp_server{ gtl::socket::ip_any, gtl::socket::port_any }) && this->socket_server.get_config(this->config_server.address, this->config_server.port);
            if (!this->socket_server_opened) {
                this->socket_server.close();
            }
            barrier.sync();
            while (this->server_running) {
                gtl::socket client;
                if (this->socket_server.accept(client)) {
                    std::lock_guard<std::mutex> lock(this->mutex_client);
                    ++this->accepted_connections;
                    static_cast<void>(lock);
                    this->socket_client.close();
                    this->socket_client = std::move(client);
                }
                std::this_thread::yield();
            }
        });
        barrier.sync();
        REQUIRE(this->socket_server_opened == true);
        if (!this->socket_server_opened) {
            return;
        }
        REQUIRE(this->socket_client_initial.is_open() == false);
        for (int i = 0; i < 100; ++i) {
            if (this->socket_client_initial.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, this->config_server.port })) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        REQUIRE(this->socket_client_initial.is_open() == true);
        const std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
        while (std::chrono::steady_clock::now() < deadline) {
            {
                std::lock_guard<std::mutex> lock(this->mutex_client);
                static_cast<void>(lock);
                if (this->accepted_connections != 0) {
                    break;
                }
            }
            std::this_thread::yield();
        }
        REQUIRE(this->get_accepted_connections_count() == 0);
    }

    unsigned short get_port() const {
        return this->config_server.port;
    }

    unsigned int get_accepted_connections_count() {
        std::lock_guard<std::mutex> lock(this->mutex_client);
        static_cast<void>(lock);
        if (this->accepted_connections == 0) {
            return 0;
        }
        return (this->accepted_connections - 1);
    }

    bool send_to_last_client(const unsigned char* message, unsigned long long int length) {
        std::lock_guard<std::mutex> lock(this->mutex_client);
        static_cast<void>(lock);
        if (!this->socket_client.is_open()) {
            return false;
        }
        unsigned long long int written_length = length;
        REQUIRE(this->socket_client.write(message, written_length));
        REQUIRE(written_length == length);
        return true;
    }
};

TEST(socket, traits, standard) {
    REQUIRE((std::is_pod<gtl::socket>::value == false));

    REQUIRE((std::is_trivial<gtl::socket>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::socket>::value == false));

    REQUIRE((std::is_standard_layout<gtl::socket>::value == true));
}

TEST(socket, constructor, empty) {
    gtl::socket socket;
    testbench::do_not_optimise_away(socket);
}

TEST(socket, function, is_open) {
    gtl::socket socket;
    REQUIRE(socket.is_open() == false);
}

TEST(socket, function, open) {
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::udp_client{ gtl::socket::ip_any, gtl::socket::port_any }) == true);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::udp_client{ gtl::socket::ip_any, gtl::socket::port_any }) == true);
    }

    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_server{ gtl::socket::ip_any, gtl::socket::port_any }, false) == true);
        REQUIRE(socket.is_open() == true);
    }

    closed_port port_closed;

    {
        // Opening a client without connecting succeeds even when the remote port is closed.
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }, false) == true);
        REQUIRE(socket.is_open() == true);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }

    local_server server;

    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, server.get_port() }) == true);
    }
}

TEST(socket, function, close) {
    gtl::socket socket;
    REQUIRE(socket.is_open() == false);
    socket.close();
    REQUIRE(socket.is_open() == false);
    REQUIRE(socket.open(gtl::socket::udp_client{ gtl::socket::ip_any, gtl::socket::port_any }) == true);
    REQUIRE(socket.is_open() == true);
    socket.close();
    REQUIRE(socket.is_open() == false);
    socket.close();
    REQUIRE(socket.is_open() == false);
}

TEST(socket, function, accept) {
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::udp_client{ gtl::socket::ip_any, gtl::socket::port_any }) == true);
        gtl::socket client;
        REQUIRE(socket.accept(client) == false);
    }

    closed_port port_closed;

    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }

    local_server server;

    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, server.get_port() }) == true);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port_closed.get_port() }) == false);
    }
    {
        gtl::socket socket;
        REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, server.get_port() }) == true);
    }
}

TEST(socket, function, read_write_tcp) {
    local_server server;
    gtl::socket socket;
    REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, server.get_port() }) == true);
    // Wait for the server to accept the connection.
    const std::chrono::steady_clock::time_point deadline_accept = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while ((server.get_accepted_connections_count() != 1) && (std::chrono::steady_clock::now() < deadline_accept)) {
        std::this_thread::yield();
    }
    REQUIRE(server.get_accepted_connections_count() == 1);
    REQUIRE(socket.is_data_available() == false);
    const char* sent_message = "Test message";
    const unsigned long long int sent_length = testbench::string_length(sent_message);
    REQUIRE(server.send_to_last_client(reinterpret_cast<const unsigned char*>(sent_message), sent_length));
    // Wait for the sent data to arrive.
    const std::chrono::steady_clock::time_point deadline_data = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while ((!socket.is_data_available()) && (std::chrono::steady_clock::now() < deadline_data)) {
        std::this_thread::yield();
    }
    REQUIRE(socket.is_data_available() == true);
    // Read until the whole message has arrived, as tcp data can be delivered in multiple parts.
    unsigned char received_message[128] = {};
    unsigned long long int received_length = 0;
    const std::chrono::steady_clock::time_point deadline_read = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while ((received_length < sent_length) && (std::chrono::steady_clock::now() < deadline_read)) {
        unsigned long long int read_length = (sizeof(received_message) - 1) - received_length;
        const bool read_success = socket.read(&received_message[received_length], read_length);
        REQUIRE(read_success == true);
        if (!read_success) {
            break;
        }
        received_length += read_length;
        std::this_thread::yield();
    }
    received_message[127] = 0;

    REQUIRE(received_length == sent_length, "Mismatching lengths of sent (%llu) and receieved (%llu) data.", sent_length, received_length);
    REQUIRE(testbench::is_memory_same(sent_message, received_message, sent_length), "Mismatching sent and receieved data. '%s' != '%s'", sent_message, received_message);
}

TEST(socket, function, write_closed_tcp) {
    local_server server;
    gtl::socket socket;
    REQUIRE(socket.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, server.get_port() }) == true);
    // Wait for the server to accept the connection.
    const std::chrono::steady_clock::time_point deadline_accept = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while ((server.get_accepted_connections_count() != 1) && (std::chrono::steady_clock::now() < deadline_accept)) {
        std::this_thread::yield();
    }
    REQUIRE(server.get_accepted_connections_count() == 1);
    // Cause the server to close the connection by replacing it with another.
    gtl::socket socket_replacement;
    REQUIRE(socket_replacement.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, server.get_port() }) == true);
    const std::chrono::steady_clock::time_point deadline_replace = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while ((server.get_accepted_connections_count() != 2) && (std::chrono::steady_clock::now() < deadline_replace)) {
        std::this_thread::yield();
    }
    REQUIRE(server.get_accepted_connections_count() == 2);
    // Writing to the closed connection must eventually fail rather than succeed or raise a signal.
    const char* sent_message = "Test message";
    bool write_success = true;
    const std::chrono::steady_clock::time_point deadline_write = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (write_success && (std::chrono::steady_clock::now() < deadline_write)) {
        unsigned long long int sent_length = testbench::string_length(sent_message);
        write_success = socket.write(reinterpret_cast<const unsigned char*>(sent_message), sent_length);
        std::this_thread::yield();
    }
    REQUIRE(write_success == false);
    // Further writes to the closed connection must fail without raising a signal.
    for (int i = 0; i < 10; ++i) {
        unsigned long long int sent_length = testbench::string_length(sent_message);
        REQUIRE(socket.write(reinterpret_cast<const unsigned char*>(sent_message), sent_length) == false);
    }
}

TEST(socket, function, read_write_udp) {
    gtl::socket socket1;
    REQUIRE(socket1.open(gtl::socket::udp_client{ gtl::socket::ip_loopback, gtl::socket::port_any }) == true);
    gtl::socket::ip socket1_address = {};
    unsigned short socket1_port = 0;
    REQUIRE(socket1.get_config(socket1_address, socket1_port) == true);

    gtl::socket socket2;
    REQUIRE(socket2.open(gtl::socket::udp_client{ gtl::socket::ip_loopback, gtl::socket::port_any }) == true);
    gtl::socket::ip socket2_address = {};
    unsigned short socket2_port = 0;
    REQUIRE(socket2.get_config(socket2_address, socket2_port) == true);

    const char* sent_message1 = "Test message 1";
    unsigned long long int sent_length1 = testbench::string_length(sent_message1);
    REQUIRE(socket1.write(reinterpret_cast<const unsigned char*>(sent_message1), sent_length1, gtl::socket::ip_loopback, socket2_port));

    unsigned char received_message1[128] = {};
    unsigned long long int received_length1 = 0;
    gtl::socket::ip address1 = {};
    unsigned short port1 = 0;
    const std::chrono::steady_clock::time_point deadline1 = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (std::chrono::steady_clock::now() < deadline1) {
        received_length1 = 128;
        const bool read_success = socket2.read(received_message1, received_length1, address1, port1);
        REQUIRE(read_success == true);
        if ((!read_success) || (received_length1 != 0)) {
            break;
        }
        std::this_thread::yield();
    }
    received_message1[127] = 0;

    REQUIRE(address1 == gtl::socket::ip_loopback, "Socket 1 ip does not match loopback, got %d.%d.%d.%d.", address1.segment[0], address1.segment[1], address1.segment[2], address1.segment[3]);
    REQUIRE(port1 == socket1_port, "Socket1 port does not matched expected value expected %d, got %d.", socket1_port, port1);
    REQUIRE(received_length1 == sent_length1, "Mismatching lengths of sent (%llu) and receieved (%llu) data.", sent_length1, received_length1);
    REQUIRE(testbench::is_memory_same(sent_message1, received_message1, sent_length1), "Mismatching sent and receieved data. '%s' != '%s'", sent_message1, received_message1);

    const char* sent_message2 = "Test message the second";
    unsigned long long int sent_length2 = testbench::string_length(sent_message2);
    REQUIRE(socket2.write(reinterpret_cast<const unsigned char*>(sent_message2), sent_length2, gtl::socket::ip_loopback, socket1_port));

    unsigned char received_message2[128] = {};
    unsigned long long int received_length2 = 0;
    gtl::socket::ip address2 = {};
    unsigned short port2 = 0;
    const std::chrono::steady_clock::time_point deadline2 = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (std::chrono::steady_clock::now() < deadline2) {
        received_length2 = 128;
        const bool read_success = socket1.read(received_message2, received_length2, address2, port2);
        REQUIRE(read_success == true);
        if ((!read_success) || (received_length2 != 0)) {
            break;
        }
        std::this_thread::yield();
    }
    received_message2[127] = 0;

    REQUIRE(address2 == gtl::socket::ip_loopback, "Socket 2 ip does not match loopback, got %d.%d.%d.%d.", address2.segment[0], address2.segment[1], address2.segment[2], address2.segment[3]);
    REQUIRE(port2 == socket2_port, "Socket2 port does not matched expected value expected %d, got %d.", socket2_port, port2);
    REQUIRE(received_length2 == sent_length2, "Mismatching lengths of sent (%llu) and receieved (%llu) data.", sent_length2, received_length2);
    REQUIRE(testbench::is_memory_same(sent_message2, received_message2, sent_length2), "Mismatching sent and receieved data. '%s' != '%s'", sent_message2, received_message2);
}
