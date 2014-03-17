#include <iostream>
#include <problem.h>

using namespace std;

int main(int argc, char **argv)
{
    Problem problem("../instances/p01");

    if (!problem.is_ready())
    {
        cout << "Loading of problem failed" << endl;
        return 1;
    }

    cout << "Successfully loaded problem" << endl;

    return 0;
}
