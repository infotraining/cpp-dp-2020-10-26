#include <iostream>
#include <memory>
#include "builder.hpp"

using namespace std;

int main()
{
    Director director;

    cout << "Building with ConcreteBuilder1:\n";

    ConcreteBuilder1 builder1;
    director.construct(builder1);
    std::unique_ptr<Product1> p1 = builder1.get_result();
    p1->show();

    cout << "\n\nBuilding with ConcreteBuilder2:\n";

    ConcreteBuilder2 builder2;
    director.construct(builder2);
    unique_ptr<Product2> p2 = builder2.get_result();
    p2->print();
}
