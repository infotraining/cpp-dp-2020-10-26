#include "proxy.hpp"

using namespace std;

class Client
{
    unique_ptr<Subject> subject_;
public:
    Client(unique_ptr<Subject> subject) : subject_{move(subject)}
	{
	}

	void use()
	{
		subject_->request();
	}
};

int main()
{
    unique_ptr<Proxy> proxy = make_unique<Proxy>("Data");

    Client c {move(proxy)};

	std::cout << std::endl;

	c.use();

	std::cout << std::endl;

	c.use();
}
