#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <problem.h>

using namespace std;

double triangle(Point first, Point middle, Point last)
{
    return first.time_to(middle) + middle.time_to(last) - first.time_to(last);
}

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Point::time_to(Point other)
{
    double dx = x - other.x, dy = y - other.y;
    return sqrt(dx*dx + dy*dy);
}

Customer::Customer(int id, int duration, double x, double y) : Point(x, y)
{
    this->id = id;
    this->duration = duration;
}

Depot::Depot(int id, double x, double y) : Point(x, y)
{
    this->id = id;
}

/**
 * Constructs a problem instance from a file according to the file format by Cordeau
 * http://neo.lcc.uma.es/vrp/vrp-instances/description-for-files-of-cordeaus-instances/
 *
 * Note not all data are of interest to us, since we are solving a weaker problem.
 * This may change in the future.
 */
Problem::Problem(string filename)
{
    ifstream in(filename, ifstream::in);

    // For throwing away useless information
    int temp;

    // Problem type: this is a MDVRP solver only!
    in >> temp;
    if (temp != 2)
    {
        cout << "Unrecognized problem type: " << temp << " - only MDVRP (2) is valid" << endl;
        return;
    }

    // Number of vehicles, customers and depots
    // We will ignore the vehicles, since it's a decision variable in our interpretation,
    // however, we need to know how many there are to correctly parse the rest of the file.
    int nvehicles, ncustomers, ndepots;
    in >> nvehicles >> ncustomers >> ndepots;

    // Throw away all the data for the vehicles
    for (int i = 0; i < nvehicles; i++)
        in >> temp >> temp;

    // Customer and depot data
    for (int i = 0; i < ncustomers + ndepots; i++)
    {
        int id, x, y, duration;

        in >> id >> x >> y >> duration;

        if (i < ncustomers)
            customers.insert(CustomerPtr(new Customer(id, duration, x, y)));
        else
            depots.insert(DepotPtr(new Depot(id, x, y)));

        // We don't care about demand and frequency
        in >> temp >> temp;

        // Throw away all the data for visit combinations
        int ncombinations;
        in >> ncombinations;
        for (int j = 0; j < ncombinations; j++)
            in >> temp;
    }

    in.close();
    this->ready = true;
}

/**
 * Constructs an empty tour that doesn't go anywhere.
 */
Tour::Tour(DepotPtr depot)
{
    this->depot = depot;
}

/**
 * Constructs a singleton tour that only visits one customer.
 */
Tour::Tour(DepotPtr depot, CustomerPtr customer)
{
    this->depot = depot;
    customers.push_back(CustomerPtr(customer));
}

/**
 * Constructs a new tour by merging two existing tours from the same depot.
 */
Tour::Tour(Tour left, Tour right)
{
    if (left.depot != right.depot)
        throw;

    auto lc = left.customers, rc = right.customers;

    depot = left.depot;

    customers.insert(customers.end(), lc.begin(), lc.end());
    customers.insert(customers.end(), rc.begin(), rc.end());
}

double Tour::duration()
{
    double duration = 0;

    if (!customers.empty())
    {
        duration += depot->time_to(*customers.front());
        duration += depot->time_to(*customers.back());
    }

    for (vector<CustomerPtr>::size_type i = 1; i < customers.size(); i++)
        duration += customers[i-1]->time_to(*customers[i]);

    for (auto customer : customers)
        duration += customer->get_duration();

    return duration;
}

/**
 * Inserts a new customer at the point where the extra distance is minimized.
 * Potentially sub-optimal but very fast.
 */
void Tour::add_customer(CustomerPtr customer)
{
    if (customers.empty())
    {
        customers.push_back(customer);
        return;
    }

    vector<CustomerPtr>::iterator location;
    double cost = -1;

    for (vector<CustomerPtr>::iterator it = customers.begin();; it++)
    {
        double test_cost;

        if (it == customers.begin())
            test_cost = triangle(*depot, *customer, **it);
        else if (it == customers.end())
            test_cost = triangle(**(it-1), *customer, *depot);
        else
            test_cost = triangle(**(it-1), *customer, **it);

        if (test_cost < cost || cost < 0)
        {
            cost = test_cost;
            location = it;
        }

        if (it == customers.end())
            break;
    }

    customers.insert(location, customer);
}

void Tour::describe()
{
    cout << fixed << setprecision(2) << duration() << " [" << depot->get_id() << "]";
    for (auto customer : customers)
        cout << " > " << setw(3) << customer->get_id();
    cout << endl;
}

/**
 * Constructs a non-solution with no tours.
 */
Solution::Solution(ProblemPtr problem)
{
    this->problem = problem;
}

/**
 * Constructs a solution from a set of tours.
 */
Solution::Solution(ProblemPtr problem, set<TourPtr> tours)
{
    this->problem = problem;
    this->tours = tours;
}

void Solution::add_tour(TourPtr tour)
{
    tours.insert(tour);
}

void Solution::describe()
{
    cout << "Number of tours: " << ntours() << endl;
    for (auto depot : problem->get_depots())
        for (auto tour : tours_from_depot(depot))
            tour->describe();
}

/**
 * Checks whether:
 * * Every customer is visited exactly once.
 * * No tour is longer than the given limit.
 */
bool Solution::is_valid()
{
    set<CustomerPtr> visited_customers;

    for (auto tour : tours)
    {
        if (tour->duration() > problem->get_daily_cap())
            return false;

        for (auto customer : tour->get_customers())
        {
            auto ret = visited_customers.insert(customer);
            if (ret.second == false)
                return false;
        }
    }

    return visited_customers.size() == problem->get_customers().size();
}

/**
 * Returns all tours starting from a given depot.
 */
set<TourPtr> Solution::tours_from_depot(DepotPtr depot)
{
    set<TourPtr> tours;

    for (auto tour : this->tours)
        if (tour->get_depot() == depot)
            tours.insert(tour);

    return tours;
}
