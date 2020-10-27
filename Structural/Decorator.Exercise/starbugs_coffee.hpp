#ifndef COFFEEHELL_HPP_
#define COFFEEHELL_HPP_

#include <iostream>
#include <memory>
#include <string>

class Coffee
{
public:
    virtual ~Coffee() = default;

    virtual float get_total_price() const = 0;
    virtual std::string get_description() const = 0;
    virtual void prepare() = 0;
};

class CoffeeBase : public Coffee
{
    float price_;
    std::string description_;

public:
    CoffeeBase(float price, const std::string& description)
        : price_ {price}
        , description_ {description}
    {
    }

    float get_total_price() const override
    {
        return price_;
    }

    std::string get_description() const override
    {
        return description_;
    }
};

class Espresso : public CoffeeBase
{
public:
    Espresso(float price = 4.0, const std::string& description = "Espresso")
        : CoffeeBase {price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect espresso: 7 g, 15 bar and 24 sec.\n";
    }
};

class Cappuccino : public CoffeeBase
{
public:
    Cappuccino(float price = 6.0, const std::string& description = "Cappuccino")
        : CoffeeBase {price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect cappuccino.\n";
    }
};

class Latte : public CoffeeBase
{
public:
    Latte(float price = 8.0, const std::string& description = "Latte")
        : CoffeeBase {price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect latte.\n";
    }
};

class CoffeeDecorator : public Coffee
{
public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> coffee)
        : coffee_ {std::move(coffee)}
    {
    }

    float get_total_price() const override
    {
        return coffee_->get_total_price();
    }

    std::string get_description() const override
    {
        return coffee_->get_description();
    }

    void prepare() override
    {
        coffee_->prepare();
    }

protected:
    std::unique_ptr<Coffee> coffee_;
};

class Whipped : public CoffeeDecorator
{
public:
    explicit Whipped(std::unique_ptr<Coffee> coffee, float price = 2.5)
        : CoffeeDecorator(std::move(coffee))
        , price_(price)
    {
    }

    float get_total_price() const override
    {
        return coffee_->get_total_price() + price_;
    }

    std::string get_description() const override
    {
        return "whipped " + coffee_->get_description();
    }

    void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Adding whipped cream...\n";
    }

private:
    float price_ {};
};

class Whisky : public CoffeeDecorator
{
public:
    explicit Whisky(std::unique_ptr<Coffee> coffee, float price = 0.4)
        : CoffeeDecorator(std::move(coffee))
        , price_(price)
    {
    }

    float get_total_price() const override
    {
        return coffee_->get_total_price() + price_;
    }

    std::string get_description() const override
    {
        return coffee_->get_description() + " +whisky";
    }

    void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Pouring 5cl of whisky...\n";
    }

private:
    float price_ {};
};

// TO DO: Condiments: Whipped: 2.5$, Whisky: 6.0$, ExtraEspresso: 4.0$

// TO DO: Add CoffeeDecorator and concrete decorators for condiments

#endif /*COFFEEHELL_HPP_*/
