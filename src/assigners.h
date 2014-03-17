#include <map>
#include <memory>
#include <problem.h>

#ifndef __ASSIGNERS_H__
#define __ASSIGNERS_H__

using namespace std;

class Assigner: public enable_shared_from_this<Assigner>
{
protected:
    map<CustomerPtr, DepotPtr> assignment;
    ProblemPtr problem;

public:
    Assigner(ProblemPtr);
    DepotPtr get_assignment(CustomerPtr);
    SolutionPtr make_solution();
};

typedef shared_ptr<Assigner> AssignerPtr;

class DistanceAssigner: public Assigner
{
public:
    DistanceAssigner(ProblemPtr);
};

typedef shared_ptr<DistanceAssigner> DistanceAssignerPtr;

#endif
