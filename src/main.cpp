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

    // Initial solution using a naive customer->depot assignment and C-W savings to merge tours
    SolutionPtr solution = cw_savings(DistanceAssigner(problem).make_solution());

    solution->describe();
    cout << (solution->is_valid() ? "Valid" : "Invalid") << endl;

    MovePtr move;
    solution->initialize_cycle();
    int i = 0;
    while (move = solution->get_next_move())
    {
        i++;
    }

    cout << "Total moves: " << i << endl;

    return 0;
}
