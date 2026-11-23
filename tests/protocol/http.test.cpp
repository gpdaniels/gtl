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

#include <protocol/http>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <atomic>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {
    using result_type = gtl::http::result_type;

    /// @brief  Read a request from a string.
    result_type read_request(const std::string& data, gtl::http::request_type& request, unsigned long long int& length) {
        return gtl::http::read_request(data.data(), data.size(), request, length);
    }

    /// @brief  Read a response from a string.
    result_type read_response(const std::string& data, const bool is_closed, gtl::http::response_type& response, unsigned long long int& length) {
        return gtl::http::read_response(data.data(), data.size(), is_closed, false, response, length);
    }

    /// @brief  Send text on a connection, then receive everything the peer sends until it closes the connection.
    std::string exchange(const unsigned short port, const std::string& text) {
        gtl::socket connection;
        REQUIRE(connection.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port }) == true);
        if (!text.empty()) {
            REQUIRE(connection.tcp_send(reinterpret_cast<const unsigned char*>(text.data()), text.size()) == static_cast<long long int>(text.size()));
        }
        std::string received;
        const std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
        for (long long int count = 0; (count >= 0) && (std::chrono::steady_clock::now() < deadline);) {
            unsigned char buffer[1024];
            count = connection.tcp_receive(buffer, sizeof(buffer));
            if (count > 0) {
                received.append(reinterpret_cast<const char*>(buffer), static_cast<unsigned long long int>(count));
            }
            std::this_thread::yield();
        }
        return received;
    }
}

TEST(http, traits, standard) {
    REQUIRE((std::is_pod<gtl::http>::value == true));

    REQUIRE((std::is_trivial<gtl::http>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::http>::value == true));

    REQUIRE((std::is_standard_layout<gtl::http>::value == true));
}

TEST(http, constructor, empty) {
    gtl::http http;
    testbench::do_not_optimise_away(http);
    gtl::http::request_type request;
    REQUIRE(request.method == "GET");
    REQUIRE(request.target == "/");
    REQUIRE(request.headers.empty() == true);
    REQUIRE(request.body.empty() == true);
    REQUIRE(request.get_header("Host") == nullptr);
    gtl::http::response_type response;
    REQUIRE(response.status == 200);
    REQUIRE(response.reason == "OK");
    REQUIRE(response.get_header("Content-Length") == nullptr);
}

TEST(http, function, write_request) {
    std::string output;
    gtl::http::request_type request;
    REQUIRE(gtl::http::write_request(request, output) == true);
    REQUIRE(output == "GET / HTTP/1.1\r\n\r\n");

    request.method = "POST";
    request.target = "/path?query=1";
    request.headers.push_back(gtl::http::header_type{ "Host", "example.test" });
    request.headers.push_back(gtl::http::header_type{ "X-Custom", "a value" });
    request.body = "the body";
    REQUIRE(gtl::http::write_request(request, output) == true);
    REQUIRE(output == "POST /path?query=1 HTTP/1.1\r\nHost: example.test\r\nX-Custom: a value\r\nContent-Length: 8\r\n\r\nthe body");
    // A request that usually has a body gives its length even when it is empty.
    request.body.clear();
    REQUIRE(gtl::http::write_request(request, output) == true);
    REQUIRE(output == "POST /path?query=1 HTTP/1.1\r\nHost: example.test\r\nX-Custom: a value\r\nContent-Length: 0\r\n\r\n");
    // Headers that frame the body are replaced by the real length.
    request.headers.push_back(gtl::http::header_type{ "content-length", "999" });
    request.headers.push_back(gtl::http::header_type{ "Transfer-Encoding", "chunked" });
    request.body = "abc";
    REQUIRE(gtl::http::write_request(request, output) == true);
    REQUIRE(output == "POST /path?query=1 HTTP/1.1\r\nHost: example.test\r\nX-Custom: a value\r\nContent-Length: 3\r\n\r\nabc");

    // Methods, targets, and headers that would let one request be read as another are refused, leaving the output unchanged.
    output = "unchanged";
    const char* methods[] = { "", "GE T", "GET\r\n", "G:T" };
    for (const char* method : methods) {
        gtl::http::request_type invalid;
        invalid.method = method;
        REQUIRE(gtl::http::write_request(invalid, output) == false);
    }
    const char* targets[] = { "", "/a b", "/a\r\nX: y", "/a\nb" };
    for (const char* target : targets) {
        gtl::http::request_type invalid;
        invalid.target = target;
        REQUIRE(gtl::http::write_request(invalid, output) == false);
    }
    const char* names[] = { "", "Na me", "Name:", "Name\r\n" };
    for (const char* name : names) {
        gtl::http::request_type invalid;
        invalid.headers.push_back(gtl::http::header_type{ name, "value" });
        REQUIRE(gtl::http::write_request(invalid, output) == false);
    }
    gtl::http::request_type invalid;
    invalid.headers.push_back(gtl::http::header_type{ "Name", "value\r\nOther: value" });
    REQUIRE(gtl::http::write_request(invalid, output) == false);
    REQUIRE(output == "unchanged");
}

TEST(http, function, write_response) {
    std::string output;
    gtl::http::response_type response;
    REQUIRE(gtl::http::write_response(response, output) == true);
    REQUIRE(output == "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    response.status = 404;
    response.reason = "Not Found";
    response.headers.push_back(gtl::http::header_type{ "Content-Type", "text/plain" });
    response.body = "nothing here";
    REQUIRE(gtl::http::write_response(response, output) == true);
    REQUIRE(output == "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nnothing here");
    // A response that never has a body does not give a length, and cannot be given a body.
    gtl::http::response_type empty;
    empty.status = 204;
    empty.reason = "No Content";
    REQUIRE(gtl::http::write_response(empty, output) == true);
    REQUIRE(output == "HTTP/1.1 204 No Content\r\n\r\n");
    output = "unchanged";
    empty.body = "a body";
    REQUIRE(gtl::http::write_response(empty, output) == false);
    gtl::http::response_type invalid;
    invalid.status = 99;
    REQUIRE(gtl::http::write_response(invalid, output) == false);
    invalid.status = 1000;
    REQUIRE(gtl::http::write_response(invalid, output) == false);
    invalid.status = 200;
    invalid.reason = "OK\r\nInjected: header";
    REQUIRE(gtl::http::write_response(invalid, output) == false);
    REQUIRE(output == "unchanged");
}

TEST(http, function, read_request) {
    gtl::http::request_type request;
    unsigned long long int length = 0;
    {
        const std::string data = "GET /index.html?a=b HTTP/1.1\r\nHost: example.test\r\nX-Empty:\r\nX-Spaces: \t padded value \t \r\n\r\n";
        REQUIRE(read_request(data, request, length) == result_type::complete);
        REQUIRE(length == data.size());
        REQUIRE(request.method == "GET");
        REQUIRE(request.target == "/index.html?a=b");
        REQUIRE(request.headers.size() == 3);
        REQUIRE(request.body.empty() == true);
        // Headers are found whatever the case of their name, and their values have no white space around them.
        REQUIRE(request.get_header("Host") != nullptr);
        REQUIRE(*request.get_header("host") == "example.test");
        REQUIRE(*request.get_header("HOST") == "example.test");
        REQUIRE(*request.get_header("X-Empty") == "");
        REQUIRE(*request.get_header("x-spaces") == "padded value");
        REQUIRE(request.get_header("Hos") == nullptr);
        REQUIRE(request.get_header("Hosts") == nullptr);

        // Every shorter length is incomplete, and does not change the request.
        gtl::http::request_type unchanged;
        unchanged.method = "UNCHANGED";
        for (unsigned long long int partial = 0; partial < data.size(); ++partial) {
            REQUIRE_SILENT(gtl::http::read_request(data.data(), partial, unchanged, length) == result_type::incomplete);
        }
        REQUIRE(unchanged.method == "UNCHANGED");
    }
    {
        // A body with a length, followed by another request which is not used.
        const std::string data = "POST /submit HTTP/1.1\r\nContent-Length: 11\r\n\r\nhello worldGET / HTTP/1.1\r\n\r\n";
        REQUIRE(read_request(data, request, length) == result_type::complete);
        REQUIRE(request.method == "POST");
        REQUIRE(request.body == "hello world");
        REQUIRE(length == (data.size() - 18));
        REQUIRE(read_request(data.substr(length), request, length) == result_type::complete);
        REQUIRE(request.method == "GET");
        REQUIRE(length == 18);
        REQUIRE(read_request(data.substr(0, data.size() - 19), request, length) == result_type::incomplete);
    }
    {
        // A body in chunks, with a chunk extension and a trailing header, both of which are ignored.
        const std::string data = "PUT /chunks HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n6;extension=1\r\n world\r\nA\r\n0123456789\r\n0\r\nTrailer: value\r\n\r\n";
        REQUIRE(read_request(data, request, length) == result_type::complete);
        REQUIRE(request.body == "hello world0123456789");
        REQUIRE(length == data.size());
        for (unsigned long long int partial = 0; partial < data.size(); ++partial) {
            REQUIRE_SILENT(gtl::http::read_request(data.data(), partial, request, length) == result_type::incomplete);
        }
    }
    {
        // The older version of the protocol, and bodies that are larger than is allowed.
        const std::string data = "GET / HTTP/1.0\r\n\r\n";
        REQUIRE(read_request(data, request, length) == result_type::complete);
        const std::string large = "POST / HTTP/1.1\r\nContent-Length: 11\r\n\r\nhello world";
        REQUIRE(gtl::http::read_request(large.data(), large.size(), request, length, 11) == result_type::complete);
        REQUIRE(gtl::http::read_request(large.data(), large.size(), request, length, 10) == result_type::invalid);
        const std::string chunked = "POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n6\r\nhello \r\n5\r\nworld\r\n0\r\n\r\n";
        REQUIRE(gtl::http::read_request(chunked.data(), chunked.size(), request, length, 11) == result_type::complete);
        REQUIRE(gtl::http::read_request(chunked.data(), chunked.size(), request, length, 10) == result_type::invalid);
    }
    REQUIRE(gtl::http::read_request(nullptr, 0, request, length) == result_type::incomplete);
    REQUIRE(gtl::http::read_request(nullptr, 10, request, length) == result_type::invalid);
}

TEST(http, function, read_request_invalid) {
    const char* invalid[] = {
        // Start lines that are not a method, a target, and a version.
        "GET\r\n\r\n",
        "GET /\r\n\r\n",
        "GET / HTTP/2.0\r\n\r\n",
        "GET / http/1.1\r\n\r\n",
        "GET  / HTTP/1.1\r\n\r\n",
        "GET / extra HTTP/1.1\r\n\r\n",
        "G@T / HTTP/1.1\r\n\r\n",
        " GET / HTTP/1.1\r\n\r\n",
        "\r\nGET / HTTP/1.1\r\n\r\n",
        // Headers that are not a name, a colon, and a value.
        "GET / HTTP/1.1\r\nNoColon\r\n\r\n",
        "GET / HTTP/1.1\r\n: value\r\n\r\n",
        "GET / HTTP/1.1\r\nBad Name: value\r\n\r\n",
        "GET / HTTP/1.1\r\nName : value\r\n\r\n",
        "GET / HTTP/1.1\r\n continued: value\r\n\r\n",
        // Lines that do not end with both characters.
        "GET / HTTP/1.1\nHost: a\r\n\r\n",
        "GET / HTTP/1.1\r\nHost: a\nOther: b\r\n\r\n",
        "GET / HTTP/1.1\r\nHost: a\rb\r\n\r\n",
        // Lengths that are not numbers, that disagree, or that are combined with chunks.
        "POST / HTTP/1.1\r\nContent-Length: abc\r\n\r\n",
        "POST / HTTP/1.1\r\nContent-Length: -1\r\n\r\n",
        "POST / HTTP/1.1\r\nContent-Length: 1 2\r\n\r\n",
        "POST / HTTP/1.1\r\nContent-Length:\r\n\r\n",
        "POST / HTTP/1.1\r\nContent-Length: 99999999999999999999\r\n\r\n",
        "POST / HTTP/1.1\r\nContent-Length: 3\r\nContent-Length: 4\r\n\r\nabcd",
        "POST / HTTP/1.1\r\nContent-Length: 3\r\nTransfer-Encoding: chunked\r\n\r\n3\r\nabc\r\n0\r\n\r\n",
        "POST / HTTP/1.1\r\nTransfer-Encoding: gzip\r\n\r\n",
        // Chunks that are not well formed.
        "POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\nxyz\r\nabc\r\n0\r\n\r\n",
        "POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n3\r\nabcd\r\n0\r\n\r\n",
        "POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n\r\nabc\r\n0\r\n\r\n",
        "POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\nFFFFFFFFFFFFFFFFF\r\n",
    };
    gtl::http::request_type request;
    request.method = "UNCHANGED";
    unsigned long long int length = 1234;
    for (const char* data : invalid) {
        REQUIRE(read_request(data, request, length) == result_type::invalid, "Read the invalid request '%s'.", data);
    }
    // A null character in the head.
    const std::string null_character("GET / HTTP/1.1\r\nHost: a\0b\r\n\r\n", 29);
    REQUIRE(read_request(null_character, request, length) == result_type::invalid);
    REQUIRE(request.method == "UNCHANGED");
    REQUIRE(length == 1234);

    // A head that is too large, with or without its end, and too many headers.
    std::string large = "GET / HTTP/1.1\r\nX-Large: ";
    large.append(gtl::http::head_size_max, 'a');
    REQUIRE(read_request(large, request, length) == result_type::invalid);
    large += "\r\n\r\n";
    REQUIRE(read_request(large, request, length) == result_type::invalid);
    std::string many = "GET / HTTP/1.1\r\n";
    for (unsigned long long int i = 0; i < gtl::http::headers_length_max; ++i) {
        many += "X: y\r\n";
    }
    REQUIRE(read_request(many + "\r\n", request, length) == result_type::complete);
    REQUIRE(request.headers.size() == gtl::http::headers_length_max);
    REQUIRE(read_request(many + "X: y\r\n\r\n", request, length) == result_type::invalid);
}

TEST(http, function, read_response) {
    gtl::http::response_type response;
    unsigned long long int length = 0;
    {
        const std::string data = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nhelloextra";
        REQUIRE(read_response(data, false, response, length) == result_type::complete);
        REQUIRE(length == (data.size() - 5));
        REQUIRE(response.status == 200);
        REQUIRE(response.reason == "OK");
        REQUIRE(response.body == "hello");
        REQUIRE(*response.get_header("content-type") == "text/plain");
        const unsigned long long int whole = length;
        for (unsigned long long int partial = 0; partial < whole; ++partial) {
            REQUIRE_SILENT(gtl::http::read_response(data.data(), partial, false, false, response, length) == result_type::incomplete);
        }
    }
    {
        // A reason phrase with spaces, and reason phrases that are empty.
        const std::string data = "HTTP/1.0 404 Not Found At All\r\nContent-Length: 0\r\n\r\n";
        REQUIRE(read_response(data, false, response, length) == result_type::complete);
        REQUIRE(response.status == 404);
        REQUIRE(response.reason == "Not Found At All");
        REQUIRE(read_response("HTTP/1.1 500 \r\nContent-Length: 0\r\n\r\n", false, response, length) == result_type::complete);
        REQUIRE(response.status == 500);
        REQUIRE(response.reason == "");
        REQUIRE(read_response("HTTP/1.1 500\r\nContent-Length: 0\r\n\r\n", false, response, length) == result_type::complete);
        REQUIRE(response.reason == "");
    }
    {
        // A body in chunks.
        const std::string data = "HTTP/1.1 200 OK\r\nTransfer-Encoding: Chunked\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n";
        REQUIRE(read_response(data, false, response, length) == result_type::complete);
        REQUIRE(response.body == "Wikipedia");
        REQUIRE(length == data.size());
    }
    {
        // A body with no length ends when the connection does.
        const std::string data = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nall of the rest";
        REQUIRE(read_response(data, false, response, length) == result_type::incomplete);
        REQUIRE(read_response(data, true, response, length) == result_type::complete);
        REQUIRE(response.body == "all of the rest");
        REQUIRE(length == data.size());
        // A body with a length that is cut short by the end of the connection is never complete.
        const std::string cut = "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nshort";
        REQUIRE(read_response(cut, true, response, length) == result_type::incomplete);
    }
    {
        // Responses that never have a body, whatever their headers say.
        const std::string no_content = "HTTP/1.1 204 No Content\r\n\r\n";
        REQUIRE(read_response(no_content, false, response, length) == result_type::complete);
        REQUIRE(response.status == 204);
        REQUIRE(response.body.empty() == true);
        const std::string not_modified = "HTTP/1.1 304 Not Modified\r\nContent-Length: 100\r\n\r\n";
        REQUIRE(read_response(not_modified, false, response, length) == result_type::complete);
        REQUIRE(length == not_modified.size());
        const std::string head = "HTTP/1.1 200 OK\r\nContent-Length: 100\r\n\r\n";
        REQUIRE(read_response(head, false, response, length) == result_type::incomplete);
        REQUIRE(gtl::http::read_response(head.data(), head.size(), false, true, response, length) == result_type::complete);
        REQUIRE(response.body.empty() == true);
        REQUIRE(*response.get_header("Content-Length") == "100");
    }
    const char* invalid[] = {
        "HTTP/1.1\r\n\r\n",
        "HTTP/1.1 20 OK\r\n\r\n",
        "HTTP/1.1 2000 OK\r\n\r\n",
        "HTTP/1.1 abc OK\r\n\r\n",
        "HTTP/1.1 099 OK\r\n\r\n",
        "HTTP/2.0 200 OK\r\n\r\n",
        "HTTP/1.1  200 OK\r\n\r\n",
        "GET / HTTP/1.1\r\n\r\n",
        "HTTP/1.1 200 OK\r\nContent-Length: x\r\n\r\n",
        "HTTP/1.1 200 OK\r\nBad Header\r\n\r\n",
    };
    response.status = 777;
    for (const char* data : invalid) {
        REQUIRE(read_response(data, true, response, length) == result_type::invalid, "Read the invalid response '%s'.", data);
    }
    REQUIRE(response.status == 777);
    REQUIRE(gtl::http::read_response(nullptr, 0, false, false, response, length) == result_type::incomplete);
    REQUIRE(gtl::http::read_response(nullptr, 10, false, false, response, length) == result_type::invalid);
}

TEST(http, evaluate, round_trip) {
    // What is written can be read back, for bodies of many sizes that hold every byte value.
    const unsigned long long int sizes[6] = { 0, 1, 255, 256, 5000, 100000 };
    for (const unsigned long long int size : sizes) {
        std::string body;
        for (unsigned long long int i = 0; i < size; ++i) {
            body.push_back(static_cast<char>(i * 7));
        }
        gtl::http::request_type request;
        request.method = "PUT";
        request.target = "/round/trip";
        request.headers.push_back(gtl::http::header_type{ "Host", "example.test" });
        request.body = body;
        std::string encoded;
        REQUIRE(gtl::http::write_request(request, encoded) == true);
        gtl::http::request_type request_read;
        unsigned long long int length = 0;
        REQUIRE(read_request(encoded, request_read, length) == result_type::complete);
        REQUIRE(length == encoded.size());
        REQUIRE(request_read.method == "PUT");
        REQUIRE(request_read.target == "/round/trip");
        REQUIRE(request_read.body == body);
        REQUIRE(*request_read.get_header("Host") == "example.test");

        gtl::http::response_type response;
        response.status = 201;
        response.reason = "Created";
        response.body = body;
        REQUIRE(gtl::http::write_response(response, encoded) == true);
        gtl::http::response_type response_read;
        REQUIRE(read_response(encoded, false, response_read, length) == result_type::complete);
        REQUIRE(length == encoded.size());
        REQUIRE(response_read.status == 201);
        REQUIRE(response_read.reason == "Created");
        REQUIRE(response_read.body == body);
    }
}

TEST(http, evaluate, client_and_server) {
    // A server on a local port that answers every request on each connection it accepts.
    gtl::socket listener;
    REQUIRE(listener.open(gtl::socket::tcp_server{ gtl::socket::ip_loopback, gtl::socket::port_any }) == true);
    gtl::socket::ip address = {};
    unsigned short port = 0;
    REQUIRE(listener.get_config(address, port) == true);
    constexpr const unsigned int connections = 7;
    std::atomic<unsigned int> served(0);
    std::thread server([&listener, &served]() {
        for (unsigned int i = 0; i < connections; ++i) {
            gtl::socket connection;
            if (!listener.accept(connection)) {
                break;
            }
            served += gtl::http::serve(connection, [](const gtl::http::request_type& request) -> gtl::http::response_type {
                gtl::http::response_type response;
                if (request.target == "/missing") {
                    response.status = 404;
                    response.reason = "Not Found";
                }
                response.headers.push_back(gtl::http::header_type{ "Content-Type", "text/plain" });
                response.body = request.method + " " + request.target + " " + request.body;
                return response;
            },
                                       5000);
        }
    });

    gtl::http::request_type request;
    gtl::http::response_type response;
    request.headers.push_back(gtl::http::header_type{ "Host", "localhost" });
    REQUIRE(gtl::http::send_request(gtl::socket::ip_loopback, port, request, response) == true);
    REQUIRE(response.status == 200);
    REQUIRE(response.body == "GET / ");
    REQUIRE(*response.get_header("Content-Type") == "text/plain");
    REQUIRE(*response.get_header("Connection") == "close");

    request.target = "/missing";
    REQUIRE(gtl::http::send_request(gtl::socket::ip_loopback, port, request, response) == true);
    REQUIRE(response.status == 404);
    REQUIRE(response.reason == "Not Found");

    // A large body in each direction.
    request.method = "POST";
    request.target = "/large";
    request.body.assign(300000, 'x');
    REQUIRE(gtl::http::send_request(gtl::socket::ip_loopback, port, request, response) == true);
    REQUIRE(response.status == 200);
    REQUIRE(response.body.size() == (12 + 300000));

    // A response to a request for only the headers has no body.
    request.method = "HEAD";
    request.target = "/head";
    request.body.clear();
    REQUIRE(gtl::http::send_request(gtl::socket::ip_loopback, port, request, response) == true);
    REQUIRE(response.status == 200);
    REQUIRE(response.body.empty() == true);

    // Two requests on one connection, the second of which asks for the connection to be closed.
    {
        std::string received = exchange(port, "GET /first HTTP/1.1\r\nHost: localhost\r\n\r\nGET /second HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n");
        unsigned long long int length = 0;
        REQUIRE(read_response(received, false, response, length) == result_type::complete);
        REQUIRE(response.body == "GET /first ");
        REQUIRE(response.get_header("Connection") == nullptr);
        received.erase(0, length);
        REQUIRE(read_response(received, true, response, length) == result_type::complete);
        REQUIRE(response.body == "GET /second ");
        REQUIRE(*response.get_header("Connection") == "close");
        REQUIRE(length == received.size());
    }

    // A request that is not valid is answered with an error, and the connection is closed.
    {
        const std::string received = exchange(port, "NOT A REQUEST\r\n\r\n");
        unsigned long long int length = 0;
        REQUIRE(read_response(received, true, response, length) == result_type::complete);
        REQUIRE(response.status == 400);
    }

    // A client that connects and closes without sending anything.
    {
        gtl::socket connection;
        REQUIRE(connection.open(gtl::socket::tcp_client{ gtl::socket::ip_any, gtl::socket::port_any, gtl::socket::ip_loopback, port }) == true);
    }
    server.join();
    REQUIRE(served == 6);

    // Nothing is listening once the server has stopped.
    listener.close();
    REQUIRE(gtl::http::send_request(gtl::socket::ip_loopback, port, request, response, 1000) == false);
}
