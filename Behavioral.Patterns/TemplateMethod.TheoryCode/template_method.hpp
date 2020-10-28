#ifndef TEMPLATE_METHOD_HPP_
#define TEMPLATE_METHOD_HPP_

#include <iostream>
#include <memory>
#include <string>

class Service
{
public:
    virtual void run(const std::string& data) = 0;
    virtual ~Service() = default;
};

class BasicService : public Service
{
public:
    virtual void run(const std::string& data)
    {
        std::cout << "BasicService runs with " << data << std::endl;
    }
};

class AdvancedService : public Service
{
public:
    virtual void run(const std::string& data)
    {
        std::cout << "AdvancedService runs with " << data << std::endl;
    }
};

// "AbstractClass"
class AbstractClass
{
protected:
    virtual void step_1() = 0;

    virtual void step_2() = 0;

    virtual bool is_valid() const = 0;

    virtual std::unique_ptr<Service> create_service() const
    {
        return std::make_unique<BasicService>();
    }

public:
    void template_method()
    {
        step_1();

        if (is_valid())
            step_2();

        auto srv = create_service();
        srv->run("Hello from Template Method");
    }

    virtual ~AbstractClass() {}
};

// "ConcreteClass"
class ConcreteClassA : public AbstractClass
{
protected:
    void step_1() override
    {
        std::cout << "ConcreteClassA::step_1()" << std::endl;
    }

    void step_2() override
    {
        std::cout << "ConcreteClassA::step_2()" << std::endl;
    }

    bool is_valid() const override
    {
        return true;
    }
};

// "ConcreteClass"
class ConcreteClassB : public AbstractClass
{
protected:
    void step_1() override
    {
        std::cout << "ConcreteClassB::step_1()" << std::endl;
    }

    void step_2() override
    {
        std::cout << "ConcreteClassB::step_2()" << std::endl;
    }

    bool is_valid() const override
    {
        return false;
    }

    std::unique_ptr<Service> create_service() const override
    {
        return std::make_unique<AdvancedService>();
    }
};

#endif /*TEMPLATE_METHOD_HPP_*/
