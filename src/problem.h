#include <memory>
#include <set>

using namespace std;

class Customer: enable_shared_from_this<Customer>
{
protected:
    double x, y;

public:
    Customer(double, double);
    double distance_to(Customer);
};

class Depot: public Customer
{
public:
    Depot(double x, double y) : Customer(x, y) { }
};

class Problem: enable_shared_from_this<Problem>
{
private:
    set<Customer> customers;
    set<Depot> depots;

public:
    Problem();
};
