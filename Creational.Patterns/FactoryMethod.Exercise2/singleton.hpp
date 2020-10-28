#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <iostream>

template <typename T>
class SingletonHolder
{
private:
    SingletonHolder() = default;
    ~SingletonHolder() = default;

public:
    SingletonHolder(const SingletonHolder&) = delete;
    SingletonHolder& operator=(const SingletonHolder&) = delete;

    static T& instance()
    {
        static T unique_instance;

        return unique_instance;
    }
};

// struct Logger
// {
//     void log(const std::string& logger) {}
// };

// TEST_CASE("")
// {
//     using SingletonLogger = SingletonHolder<Logger>;

//     thread thd1{[] { SingletonLogger::instance().log("a"); }};
//     thread thd2{[] { SingletonLogger::instance().log("a"); }};
//     thd1.join();
//     thd2.join();
// }

#endif // SINGLETON_HPP
