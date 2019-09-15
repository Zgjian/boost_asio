#pragma once

#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;


class tcp_server
        : public std::enable_shared_from_this<tcp_server>
{
private:
    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;

public:
    explicit tcp_server(const std::string &address, const std::string &port)
        : acceptor_(io_context_)
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
        io_context_.run();
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

//    void onTimer(const boost::system::error_code &err)
//    {
//        printf("onTimer...\n");
//    }

};
