//
// Created by Sergei Alexeev on 05.05.2020.
//

#ifndef NETLIB_CLIENT_H
#define NETLIB_CLIENT_H

#include <iostream>
#include <functional>
#include <queue>

#include <memory>
#include <optional>
#include <thread>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class AbstractClient {
public:
    virtual void run() = 0;
    virtual void write(const std::string & msg) = 0;
    virtual void close() = 0;
    virtual void setMsgHandler(const std::function<void(const std::string &)> & f) = 0;
    virtual void setErrHandler(const std::function<void(int)> & f) = 0;
};

class Client: public std::enable_shared_from_this<Client>, public AbstractClient {
public:
    Client(const Client &other) = delete;
    Client(Client &&other) = delete;
    Client& operator=(const Client &other) = delete;
    Client& operator=(Client &&other) = delete;
    static std::shared_ptr<Client> shared();
    void run() override;
    void write(const std::string & msg) override;
    void close() override;
    void setMsgHandler(const std::function<void(const std::string &)> & f) override;
    void setErrHandler(const std::function<void(int)> & f) override;
private:
    explicit Client(boost::asio::io_service & service);
    void connect(boost::asio::ip::tcp::resolver::iterator & it);
    void loopRead();
    bool readCondition(const boost::system::error_code & err, size_t length);
    void writeFromQue();
private:
    std::thread t;
    static boost::asio::io_service service;
    boost::asio::ip::tcp::socket sock;
    boost::asio::ip::tcp::resolver::iterator eit;
    enum { maxMsg = 2048 };
    std::array<char, maxMsg> readMsg;
    std::queue<std::string> writeMsgQue;
    std::optional<std::function<void(const std::string &)>> msgHandler;
    std::optional<std::function<void(int)>> errHandler;
};


#endif //NETLIB_CLIENT_H