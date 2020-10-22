#include <cassert>
#include <iostream>
#include <memory>
#include <typeinfo>

using namespace std;

class Engine
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~Engine() = default;
};

class Diesel : public Engine
{
public:
    virtual void start() override
    {
        cout << "Diesel starts\n";
    }

    virtual void stop() override
    {
        cout << "Diesel stops\n";
    }
};

class TDI : public Diesel
{
public:
    virtual void start() override
    {
        cout << "TDI starts\n";
    }

    virtual void stop() override
    {
        cout << "TDI stops\n";
    }
};

class Hybrid : public Engine
{
public:
    virtual void start() override
    {
        cout << "Hybrid starts\n";
    }

    virtual void stop() override
    {
        cout << "Hybrid stops\n";
    }
};

class Car
{
    unique_ptr<Engine> engine_;

public:
    Car(unique_ptr<Engine> engine)
        : engine_{move(engine)}
    {
    }

    void drive(int km)
    {
        engine_->start();
        cout << "Driving " << km << " kms\n";
        engine_->stop();
    }
};

int main()
{
    Car c1{make_unique<Hybrid>()};
    c1.drive(100);

    cout << "\n";
}
