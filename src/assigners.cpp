#include <assigners.h>

Assigner::Assigner(ProblemPtr problem)
{
    this->problem = problem;
}

DepotPtr Assigner::get_assignment(CustomerPtr customer)
{
    return assignment[customer];
}

SolutionPtr Assigner::make_solution()
{
    SolutionPtr solution = SolutionPtr(new Solution(this->problem));

    for (CustomerPtr customer : problem->get_customers())
    {
        TourPtr tour(new Tour(get_assignment(customer)));
        tour->add_customer(customer);
        solution->add_tour(tour);
    }

    return solution;
}

DistanceAssigner::DistanceAssigner(ProblemPtr problem) : Assigner(problem)
{
    for (CustomerPtr customer : problem->get_customers())
    {
        DepotPtr nearest_depot;
        double duration = -1;

        for (DepotPtr depot : problem->get_depots())
        {
            double test_duration = depot->time_to(*customer);
            if (test_duration < duration || duration < 0)
            {
                duration = test_duration;
                nearest_depot = depot;
            }
        }

        assignment[customer] = nearest_depot;
    }
}
