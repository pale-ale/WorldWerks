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
    auto ptx = wwnet::EMessageType::PLAINTXT;
    server.send_single(pc.clientFd, ptx, "Hello, Client!");
    auto [clientType, clientText] = client.rcv_data();
    REQUIRE(clientType == ptx);
    REQUIRE_THAT(clientText, Equals("Hello, Client!"));

    // Client -> Server
    client.send_data(ptx, "Hello, Server!");
    client.send_data(ptx, "Second message");
    auto [serverType, serverText] = server.rcv_single(pc.clientFd);
    REQUIRE(serverType == ptx);
    REQUIRE_THAT(serverText, Equals("Hello, Server!"));
    
    auto [serverType2, serverText2] = server.rcv_single(pc.clientFd);
    REQUIRE(serverType2 == ptx);
    REQUIRE_THAT(serverText2, Equals("Second message"));
}
