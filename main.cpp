#include <iostream>
#include "network/tcp_server.hpp"
#include "session/echo_session.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    printf("main\n");

    tcp_server server("0.0.0.0", "8081");
    server.run<EchoSession>();



    return 0;
}
