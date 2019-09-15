#pragma once

#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;


class tcp_session
        : public std::enable_shared_from_this<tcp_session>
{
public:
    tcp::socket socket_;

public:
    tcp_session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    virtual ~tcp_session()
    {
        socket_.close();
    }

    virtual void start() = 0;

    virtual void do_read() = 0;

    virtual void do_write() = 0;
};
