#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <problem.h>

using namespace std;

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Point::distance_to(Point other)
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

Problem::Problem(string filename)
{
    ifstream in(filename, ifstream::in);

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
