#ifndef ADAPTER_HPP_
#define ADAPTER_HPP_

#include <iostream>
#include <vector>
#include <list>

// "Target"
class Target
{
public:
    virtual void request(const std::vector<int>& vec) = 0;
    virtual ~Target() = default;
};

// "Adaptee"
class Adaptee
{
public:
    void specific_request(const std::list<int>& lst)
    {
        std::cout << "Called specific_request(";
        for(const auto& item : lst)
            std::cout << item << " ";
        std::cout << ")\n" << std::endl;
    }
};

// "Adapter"
class ClassAdapter : public Target, private Adaptee
{
public:
    void request(const std::vector<int>& vec) override
    {
        std::list<int> lst(vec.begin(), vec.end());
        specific_request(lst);
    }
};

// "Adapter"
class ObjectAdapter : public Target
{
private:
    Adaptee& adaptee_;

public:
    ObjectAdapter(Adaptee& adaptee)
        : adaptee_(adaptee)
    {
    }

    void request(const std::vector<int>& vec) override
    {
        std::list<int> lst(vec.begin(), vec.end());
        adaptee_.specific_request(lst);
    }
};

#endif /*ADAPTER_HPP_*/
