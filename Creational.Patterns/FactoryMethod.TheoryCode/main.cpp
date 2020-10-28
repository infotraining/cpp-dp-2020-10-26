#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "factory.hpp"

using namespace std;

class Service
{
    LoggerCreator creator_;

public:
    Service(LoggerCreator creator)
        : creator_(creator)
    {
    }

    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    void use()
    {
        unique_ptr<Logger> logger = creator_();
        logger->log("Client::use() has been started...");
        run();
        logger->log("Client::use() has finished...");
    }

protected:
    virtual void run()
    {
    }
};

namespace Canonical
{
    using LoggerFactory = std::unordered_map<std::string, shared_ptr<LoggerCreator>>;
}

using LoggerFactory = std::unordered_map<std::string, LoggerCreator>;

int main()
{
    LoggerFactory logger_factory;
    logger_factory.emplace("ConsoleLogger", &std::make_unique<ConsoleLogger>);
    logger_factory.emplace("FileLogger", [] { return std::make_unique<FileLogger>("data.log"); });
    logger_factory.emplace("DbLogger", [] { return std::make_unique<DbLogger>("DbWithLogs"); });

    Service srv(logger_factory.at("DbLogger"));
    srv.use();

    // begin() & end() - factory methods
    std::list<int> vec = {1, 2, 3};
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        const auto& item = *it;
        std::cout << item << "\n";
    }
}
