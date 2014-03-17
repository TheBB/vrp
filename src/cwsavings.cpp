#include <iostream>
#include <set>
#include <problem.h>
#include <cwsavings.h>

/**
 * Implementation of the Clark and Wright savings algorithm
 * This function modifies an existing solution by merging tours from the same depot if this results in a net
 * cost reduction. The tours are merged in order of net reduction from highest to lowest. This is primarily
 * used to initiate the solver with a sensible first guess.
 */
SolutionPtr cw_savings(SolutionPtr prevsol)
{
    set<TourPtr> newsol;

    for (auto depot : prevsol->get_problem()->get_depots())
    {
        auto tours = prevsol->tours_from_depot(depot);

        while (true)
        {
            double reduction = -1;
            TourPtr left, right;

            for (auto t1 : tours)
            for (auto t2 : tours)
            {
                if (t1 == t2)
                    continue;

                double test_reduction = triangle(*t1->last(), *depot, *t2->first());

                // Only accept this merge if it doesn't break the daily cap condition
                if (   test_reduction > reduction
                    && t1->duration() + t2->duration() - test_reduction <= 
                               prevsol->get_problem()->get_daily_cap())
                {
                    reduction = test_reduction;
                    left = t1;
                    right = t2;
                }
            }

            if (reduction >= 0)
            {
                tours.erase(left);
                tours.erase(right);
                tours.insert(TourPtr(new Tour(*left, *right)));
            }
            else
                break;
        }

        newsol.insert(tours.begin(), tours.end());
    }

    return SolutionPtr(new Solution(prevsol->get_problem(), newsol));
}
