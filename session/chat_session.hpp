#pragma once

#include "../network/tcp_session.hpp"
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <vector>
//#include <map>

using namespace std;


class ChatSession
        : public enable_shared_from_this<ChatSession>
        , public tcp_session
{
private:
    vector<char> m_recv_buff;
    boost::asio::steady_timer m_timer;
    std::chrono::seconds m_dead_line;
    std::chrono::system_clock::time_point m_last_time;//最近一次活动时间，实现客户端静默超时，服务端主动关闭连接

public:
    typedef std::shared_ptr<ChatSession> Ptr;

    static map<string, Ptr> m_sessions;

    ChatSession(tcp::socket socket)
        : tcp_session(std::move(socket))
        , m_recv_buff(1024, 0)
        , m_timer(socket_.get_executor())
        , m_dead_line(120)   //超时设定，120秒没数据交互，服务器主动踢出客户端
    {
        m_last_time = std::chrono::system_clock::now();
        start_timer();
    }

    void start() override
    {
        printf("ChatSession::start\n");

        auto self(shared_from_this());
        const string peerName = socket_.remote_endpoint().address().to_string() + ":" + to_string(socket_.remote_endpoint().port());
        m_sessions.emplace(peerName, self);

        do_read();
    }

    void stop() override
    {
        if (socket_.is_open())
        {
            socket_.close();
        }
        m_timer.cancel();
    }

    void start_timer()
    {
        m_timer.expires_from_now(std::chrono::seconds(5));
        m_timer.async_wait(boost::bind(&ChatSession::on_timer, this, boost::asio::placeholders::error));
    }

    void on_timer(const boost::system::error_code&)
    {
        printf("on_timer......\n");
        if ((std::chrono::system_clock::now() - m_last_time) > m_dead_line)
        {
            printf("time out...\n");
            stop();
            return;
        }
        start_timer();
    }

    void do_read() override
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(m_recv_buff),
                                [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                m_last_time = std::chrono::system_clock::now();
                printf("async_read_some: %d\n", length);

                for (auto it : m_sessions)
                {
                    it.second.get()->do_write(it.first + ":" + string(m_recv_buff.data(), length));
                }

                do_write(length);
            }
        });
    }

    void do_write(const string &msg)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(msg),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                do_read();
            }
        });
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


map<string, ChatSession::Ptr> ChatSession::m_sessions;
