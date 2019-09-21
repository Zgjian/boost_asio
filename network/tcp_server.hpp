#pragma once

#include "net.hpp"


class tcp_server
{
private:
    asio::io_context &io_context_;
    tcp::acceptor acceptor_;

public:
    tcp_server(asio::io_context &io_context, const std::string &address, const std::string &port)
        : io_context_(io_context)
        , acceptor_(io_context)
    {
        tcp::resolver resolver(io_context_);
        tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
    }

    virtual ~tcp_server()
    {
    }

    template <typename tcp_session>
    void run()
    {
        do_accept<tcp_session>();
    }

private:

    template <typename tcp_session>
    void do_accept()
    {
        acceptor_.async_accept(
                    [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<tcp_session>(std::move(socket))->start();
            }

            do_accept<tcp_session>();
        });
    }

};
