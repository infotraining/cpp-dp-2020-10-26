#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "factory.hpp"

using namespace std;

class Service
{
    shared_ptr<LoggerCreator> creator_;

public:
    Service(shared_ptr<LoggerCreator> creator)
        : creator_(creator)
    {
    }

    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    void use()
    {
        unique_ptr<Logger> logger = creator_->create_logger();
        logger->log("Client::use() has been started...");
        run();
        logger->log("Client::use() has finished...");
    }

protected:
    virtual void run() { }
};

using LoggerFactory = std::unordered_map<std::string, shared_ptr<LoggerCreator>>;

int main()
{
    LoggerFactory logger_factory;
    logger_factory.insert(make_pair("ConsoleLogger", make_shared<ConsoleLoggerCreator>()));
    logger_factory.insert(make_pair("FileLogger", make_shared<FileLoggerCreator>("data.log")));

    Service srv(logger_factory.at("ConsoleLogger"));
    srv.use();
}
