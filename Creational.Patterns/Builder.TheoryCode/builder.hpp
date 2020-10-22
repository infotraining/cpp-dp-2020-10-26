#ifndef BUILDER_HPP_
#define BUILDER_HPP_

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Product;

// "Builder"
class Builder
{
public:
    virtual void reset() = 0;
    virtual void build_part_A() = 0;
    virtual void build_part_B() = 0;
    virtual ~Builder() = default;
};

// "Director"
class Director
{
public:
    void construct(Builder& builder)
    {
        builder.reset();
        builder.build_part_A();
        builder.build_part_B();
    }
};

// "Product"
class Product1
{
    std::vector<std::string> parts_;

public:
    void add(const std::string& part)
    {
        parts_.push_back(part);
    }

    void show() const
    {
        std::cout << "Product1 consists of: ";

        for (const auto& part : parts_)
            std::cout << part << "; ";

        std::cout << std::endl;
    }
};

class Product2
{
    std::multiset<std::string> parts_;

public:
    void attach(const std::string& part)
    {
        parts_.insert(part);
    }

    void print() const
    {
        std::cout << "Product2 consists of: ";

        for (const auto& part : parts_)
            std::cout << part << "; ";

        std::cout << std::endl;
    }
};

// "ConcreteBuilder1"
class ConcreteBuilder1 : public Builder
{
private:
    std::unique_ptr<Product1> product_;

public:
    ConcreteBuilder1()
        : product_(new Product1())
    {
    }

    void reset()
    {
        product_ = std::make_unique<Product1>();
    }

    void build_part_A()
    {
        product_->add("PART A");
    }

    void build_part_B()
    {
        product_->add("PART B");
    }

    std::unique_ptr<Product1> get_result()
    {
        return move(product_);
    }
};

// "ConcreteBuilder2"
class ConcreteBuilder2 : public Builder
{
private:
    std::unique_ptr<Product2> product_;

public:
    ConcreteBuilder2()
        : product_(new Product2())
    {
    }

    void reset()
    {
        product_ = std::make_unique<Product2>();
    }

    void build_part_A()
    {
        product_->attach("PART X");
    }

    void build_part_B()
    {
        product_->attach("PART Y");
    }

    std::unique_ptr<Product2> get_result()
    {
        return move(product_);
    }
};

#endif /*BUILDER_HPP_*/
