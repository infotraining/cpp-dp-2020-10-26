#ifndef PROXY_HPP_
#define PROXY_HPP_

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <mutex>

// "Subject"
class Subject
{
public:
    virtual void request() = 0;
    virtual ~Subject() = default;
};

// "RealSubject"
class RealSubject : public Subject
{
    std::string data_;

public:
    RealSubject(const std::string& data)
        : data_{data}
    {
        using namespace std::chrono_literals;

        std::cout << "Starting creation of RealSubject" << std::endl;
        std::this_thread::sleep_for(2s);

        std::cout << "RealSubject's creation" << std::endl;
    }

    RealSubject(const RealSubject&) = delete;
    RealSubject& operator=(const RealSubject&) = delete;

    ~RealSubject()
    {
        std::cout << "RealSubject's clean-up" << std::endl;
    }

    void request() override
    {
        std::cout << "Called RealSubject.request() with " << data_ << std::endl;
    }
};

// "Proxy"
class Proxy : public Subject
{
    std::string data_;
    std::unique_ptr<RealSubject> real_subject_;
    std::once_flag init_flag_;

public:
    Proxy(const std::string& data)
        : data_{data}
        , real_subject_{nullptr}
    {
        std::cout << "Proxy's creation" << std::endl;
    }

    void request()
    {
        std::call_once(init_flag_, [this] { real_subject_ = std::make_unique<RealSubject>(data_); });

        real_subject_->request();
    }
};

#endif /*PROXY_HPP_*/
