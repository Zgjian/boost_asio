#pragma once

#include "net.hpp"


class tcp_session
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
        if (socket_.is_open())
        {
            socket_.close();
        }
    }

    virtual void start() = 0;

    virtual void stop()
    {
        if (socket_.is_open())
        {
            socket_.close();
        }
    }

    virtual void do_read() = 0;

//    virtual void do_write() = 0;
};
