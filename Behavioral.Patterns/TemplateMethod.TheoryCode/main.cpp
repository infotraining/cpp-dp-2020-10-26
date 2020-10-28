#include "template_method.hpp"

using namespace std;

void client(AbstractClass& ac)
{
    ac.template_method();
}

int main()
{
    ConcreteClassA obj;
    client(obj);

    std::cout << "\n\n";

    auto ac = make_unique<ConcreteClassB>();
    client(*ac);
};
