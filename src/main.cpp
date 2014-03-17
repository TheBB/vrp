#include <iostream>
#include <assigners.h>
#include <problem.h>

using namespace std;

int main(int argc, char **argv)
{
    ProblemPtr problem(new Problem("../instances/p01"));

    if (!problem->is_ready())
    {
        cout << "Loading of problem failed" << endl;
        return 1;
    }

    cout << "Successfully loaded problem" << endl;

    DistanceAssigner assigner(problem);
    SolutionPtr solution = assigner.make_solution();

    return 0;
}
