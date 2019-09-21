#include <iostream>
#include "network/tcp_server.hpp"
#include "session/echo_session.hpp"
#include "session/chat_session.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    printf("main\n");

    const int threads = 3;
    asio::io_context io_context{threads};


    tcp_server server(io_context, "0.0.0.0", "8081");
    server.run<ChatSession>();


    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i)
    {
        v.emplace_back([&]{io_context.run();});
    }

    io_context.run();

    return 0;
}
