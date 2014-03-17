#include <iostream>
#include <assigners.h>
#include <problem.h>
#include <cwsavings.h>

using namespace std;

int main(int argc, char **argv)
{
    ProblemPtr problem(new Problem("../instances/p01"));

    if (!problem->is_ready())
    {
        cout << "Loading of problem failed" << endl;
        return 1;
    }

    cout << "Successfully loaded problem, "
         << problem->ncustomers() << " customers and "
         << problem->ndepots() << " depots." << endl;

    DistanceAssigner assigner(problem);
    SolutionPtr solution = assigner.make_solution();

    //solution->describe();
    //cout << (solution->is_valid() ? "Valid" : "Invalid") << endl;

    SolutionPtr merged = cw_savings(solution);

    merged->describe();
    cout << (merged->is_valid() ? "Valid" : "Invalid") << endl;

    return 0;
}
