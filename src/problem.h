#include <memory>
#include <set>

using namespace std;

class Point: public enable_shared_from_this<Point>
{
protected:
    double x, y;

public:
    Point(double, double);
    double distance_to(Point);
};

class Customer: public Point
{
protected:
    int id, duration;

public:
    Customer(int, int, double, double);
};

typedef shared_ptr<Customer> CustomerPtr;

class Depot: public Point
{
protected:
    int id;

public:
    Depot(int, double, double);
};

typedef shared_ptr<Depot> DepotPtr;

class Problem: public enable_shared_from_this<Problem>
{
private:
    bool ready = false;
    set<CustomerPtr> customers;
    set<DepotPtr> depots;

public:
    Problem(string);
    bool is_ready() { return this->ready; }
};
