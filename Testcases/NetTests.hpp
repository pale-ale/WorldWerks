#include <catch2/catch.hpp>
#include "../Net/ServerEndpoint.hpp"
#include "../Net/ClientEndpoint.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Connect server and client", "[NetSC]"){
    ServerEndpoint server("127.0.0.1", 12345);
    ClientEndpoint client;
    PlayerConnection pc;
    pc = server.accept_connection();
    REQUIRE(pc.clientFd == -1);
    REQUIRE(!client.is_connected_and_buffer_empty());

    client.start_connecting("127.0.0.1", 12345);
    pc = server.accept_connection();
    REQUIRE(client.is_connected_and_buffer_empty());

    // Server -> Client
    server.send_single(pc.clientFd, "Hello!");
    auto clientText = client.rcv_data();
    REQUIRE_THAT(clientText, Equals("Hello!"));

    // Client -> Server
    client.send_data("Hello ");
    client.send_data("from ");
    client.send_data("the ");
    client.send_data("otter ");
    client.send_data("side! äöü\t-@");
    auto serverText = server.rcv_single(pc.clientFd);
    REQUIRE_THAT(serverText, Equals("Hello from the otter side! äöü\t-@"));
}
