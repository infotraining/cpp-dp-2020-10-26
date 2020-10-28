#include "adapter.hpp"
#include <iostream>

using namespace std;

class Client
{
public:
    void do_operation(Target& t)
    {
        std::vector vec{1,2, 3};
        t.request(vec);
    }
};

int main()
{
    Client client;

    cout << "-- do_operation on ClassAdapter" << endl;
    ClassAdapter cadapter;
    client.do_operation(cadapter);

    cout << endl;

    cout << "-- do_operation on ObjectAdapter" << endl;
    Adaptee adaptee;
    ObjectAdapter oadapter(adaptee);
    client.do_operation(oadapter);

    // range-based for
    int vec[] = {1, 2, 3};
    for(const auto& item : vec)
    {
        std::cout << item << " ";
    }

    for(auto it = begin(vec); it != end(vec); ++it)
    {
        const auto& item = *it;
        std::cout << item << " ";
    }
    std::cout << "\n";
}

