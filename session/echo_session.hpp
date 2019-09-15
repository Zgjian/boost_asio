#pragma once

#include "../network/tcp_session.hpp"
#include <vector>

using namespace std;


class EchoSession : public tcp_session
{
private:
    vector<char> m_recv_buff;

public:
    EchoSession(tcp::socket socket)
        : tcp_session(std::move(socket))
        , m_recv_buff(1024, 0)
    {
    }

    void start() override
    {
        printf("start\n");
        do_read();
    }

    void do_read() override
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(m_recv_buff),
                                [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                printf("async_read_some: %d\n", length);
                do_write(length);
            }
        });
    }

    void do_write() override
    {
    }

private:
    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(m_recv_buff.data(), length),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                do_read();
            }
        });
    }

};
