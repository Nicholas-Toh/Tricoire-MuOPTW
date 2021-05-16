#ifndef SOLOMON_H
#define SOLOMON_H

#include "Problem.h"
#include "Solution.h"
#include <vector>

enum SolomonStrategy {HighestDemand = 1, FarthestDistance = 2, EarliestReadyTime = 3, SortedHighestDemand = 4};

class SolomonConstructive
{
    public:
        SolomonConstructive(double mu = 1, double lambda = 1, double alpha1 = 0, double alpha2 = 1);
        int selectSeedCustomer(vector<Customer>customers, Customer depot, SolomonStrategy strategy);
        int searchHighestDemandCustomer(vector<Customer>customers, Customer depot);
        int searchSortedHighestDemand(vector<Customer>customers, Customer depot);
        int searchFarthestCustomer(vector<Customer>customers, Customer depot);
        int searchEarliestCustomer(vector<Customer>customers, Customer depot);
        Solution solve(Problem& problem, SolomonStrategy strategy);
        Solution solve(Problem& problem, SolomonStrategy strategy, Solution existingSolution);
        Solution solveOptional(Problem& problem);
        Solution solveOptional(Problem& problem, Solution existingSolution);
        Solution sequentialBestInsert(vector<Customer>& unroutedCustomers, Solution& existingSolution, int route);
        double criterionC1(int i, Customer& u, int j, Route& route);
        double criterionC11(int i, Customer& u, int j, Route& route);
        double criterionC12(int i, Customer& u, int j, Route& route);
        double criterionC2(Customer& u, double c1Value, Route& route);
        
    private:
        double coefMu = 1;
        double coefLambda = 1;
        double coefAlpha1 = 0;
        double coefAlpha2 = 1;

    
};

#endif