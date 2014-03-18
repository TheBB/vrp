#ifndef __PROBLEM_H__
#define __PROBLEM_H__

#include <memory>
#include <set>
#include <vector>
#include <omp.h>

#define ICHG_LEFT_TO_RIGHT 0            // (1,0)-interchange operator
#define ICHG_RIGHT_TO_LEFT 1            // (0,1)-interchange operator
#define ICHG_SWAP 2                     // (1,1)-interchange operator

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

    // Time to get to another point, measured in minutes
    double time_to(Point);
};

typedef shared_ptr<Point> PointPtr;

double triangle(Point, Point, Point);

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

    inline int get_id() const { return id; }
    inline int get_duration() const { return duration; }
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

    inline int get_id() const { return id; }
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
    Tour(Tour, Tour);

    double duration();
    void add_customer(CustomerPtr);

    void describe();

    inline DepotPtr get_depot() const { return depot; }
    inline vector<CustomerPtr> get_customers() const { return customers; }
    inline bool is_empty() const { return customers.empty(); }
    inline CustomerPtr first() const { return customers.front(); }
    inline CustomerPtr last() const { return customers.back(); }
};

typedef shared_ptr<Tour> TourPtr;

/**
 * This class contains the specific problem instance.
 */
class Problem: public enable_shared_from_this<Problem>
{
private:
    bool ready = false;

    // The daily maximal duration of a serviceman, 8 hours measured in minutes
    int daily_cap = 90;

    set<CustomerPtr> customers;
    set<DepotPtr> depots;

public:
    Problem(string);
    bool is_ready() { return this->ready; }

    inline int get_daily_cap() const { return daily_cap; }
    inline set<CustomerPtr> get_customers() const { return customers; }
    inline set<DepotPtr> get_depots() const { return depots; }
    inline int ncustomers() const { return customers.size(); }
    inline int ndepots() const { return depots.size(); }
};

typedef shared_ptr<Problem> ProblemPtr;

/**
 * In the lambda-interchange strategy, a modification to a solution is represented by a Move.
 */
class Move: public enable_shared_from_this<Move>
{
};

typedef shared_ptr<Move> MovePtr;

/**
 * A proposed solution to a given problem is a collection of tours.
 */
class Solution: public enable_shared_from_this<Solution>
{
private:
    ProblemPtr problem;
    vector<TourPtr> tours;

    omp_lock_t cl_lock;
    unsigned int cl_lft, cl_rgt, cl_op, cl_lft_cst, cl_rgt_cst;

public:
    Solution(ProblemPtr);
    Solution(ProblemPtr, set<TourPtr>);
    void add_tour(TourPtr);

    void describe();

    bool is_valid();
    vector<TourPtr> tours_from_depot(DepotPtr);

    void initialize_cycle();
    MovePtr get_next_move();

    inline ProblemPtr get_problem() const { return problem; }
    inline int ntours() const { return tours.size(); }
};

typedef shared_ptr<Solution> SolutionPtr;

#endif
