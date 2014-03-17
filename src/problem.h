#include <memory>
#include <set>
#include <vector>

using namespace std;

/**
 * A point specifies any vertex in the problem graph, either a customer location
 * or a serviceman home.
 */
class Point: public enable_shared_from_this<Point>
{
protected:
    double x, y;

public:
    Point(double, double);
    double time_to(Point);
};

/**
 * Customers derive from points and have IDs and an expected service duration
 * in minutes.
 */
class Customer: public Point
{
protected:
    int id, duration;

public:
    Customer(int, int, double, double);
};

typedef shared_ptr<Customer> CustomerPtr;

/**
 * Depots derive from points, and have IDs. They are the locations of the
 * servicemen at the beginning and end of each day.
 */
class Depot: public Point
{
protected:
    int id;

public:
    Depot(int, double, double);
};

typedef shared_ptr<Depot> DepotPtr;

/**
 * A tour is a sequence of unique points, all of which are customers except
 * the initial and final, which must be the same depot.
 */
class Tour: public enable_shared_from_this<Tour>
{
private:
    DepotPtr depot;
    vector<CustomerPtr> customers;

public:
    Tour(DepotPtr);
    Tour(DepotPtr, CustomerPtr);

    double duration();
};

typedef shared_ptr<Tour> TourPtr;

/**
 * This class contains the specific problem instance.
 */
class Problem: public enable_shared_from_this<Problem>
{
    friend class Solution;

private:
    bool ready = false;
    set<CustomerPtr> customers;
    set<DepotPtr> depots;

public:
    Problem(string);
    bool is_ready() { return this->ready; }
};

typedef shared_ptr<Problem> ProblemPtr;

/**
 * A proposed solution to a given problem is a collection of tours.
 */
class Solution: public enable_shared_from_this<Solution>
{
private:
    ProblemPtr problem;
    set<TourPtr> tours;

public:
    Solution(ProblemPtr);
};
