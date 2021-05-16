#include "../include/Solomon.h"
#include <algorithm>
#include <limits>
#include <iostream>

SolomonConstructive::SolomonConstructive(double mu, double lambda, double alpha1, double alpha2)
{
    coefMu = mu;
    coefLambda = lambda;
    coefAlpha1 = alpha1;
    coefAlpha2 = alpha2;
}

int SolomonConstructive::selectSeedCustomer(vector<Customer>customers, Customer depot, SolomonStrategy strategy) 
{
    if (strategy == SolomonStrategy::HighestDemand)
    {
        return searchHighestDemandCustomer(customers, depot);
    }
    else if (strategy == SolomonStrategy::SortedHighestDemand)
    {
        return searchSortedHighestDemand(customers, depot);
    }  
    else if (strategy == SolomonStrategy::FarthestDistance)
    {
        return searchFarthestCustomer(customers, depot);
    }   
    else if (strategy == SolomonStrategy::EarliestReadyTime)
    {
        return searchEarliestCustomer(customers, depot);
    }
    else
        return -1;
}

int SolomonConstructive::searchHighestDemandCustomer(vector<Customer>customers, Customer depot) 
{
    int highest = 0;
    for (int i = 1; i < customers.size(); ++i)
    {
        if (highest == 0 || customers[i].info.id != depot.info.id)
        {    
            if (customers[i].info.value > customers[highest].info.value)
                highest = i;
        }
    }
        return highest;
}

int SolomonConstructive::searchSortedHighestDemand(vector<Customer>customers, Customer depot) 
{
    return -1;
}

int SolomonConstructive::searchFarthestCustomer(vector<Customer>customers, Customer depot) 
{
    int farthestCustomer = 0;
    double farthestDistance = 0;

    for (int i = 0; i < customers.size(); ++i)
    {
        if (customers[i].info.id != depot.info.id)
        {
            double distance = depot.getTravelTime(customers[i]);
            if (distance > farthestDistance)
            {
                farthestCustomer = i;
                farthestDistance = distance;
            }
        }
    }

    return farthestCustomer;
}

int SolomonConstructive::searchEarliestCustomer(vector<Customer>customers, Customer depot) 
{
    int earliest = 0;

    for (int i = 0; i < customers.size(); ++i)
    {
        if (customers[i].info.id != depot.info.id)
        {
            double distance = depot.getTravelTime(customers[i]);
            if (customers[i].info.endTime < customers[earliest].info.endTime)
            {
                earliest = i;
            }
        }
    }
    return earliest;
}

Solution SolomonConstructive::solve(Problem& problem, SolomonStrategy strategy) 
{
    Solution solution = Solution(problem);
    vector<Customer> unroutedCustomers = problem.getCustomers();
        
    for (int i = 0; i < problem.planningHorizon; ++i)
    {
        int seedId = selectSeedCustomer(unroutedCustomers, problem.depot, strategy);
        Customer seed = unroutedCustomers[seedId];
        unroutedCustomers.erase(unroutedCustomers.begin()+seedId);
        Route partialRoute = Route(problem, seed);
        if (partialRoute.isFeasible())
            solution.addRoute(partialRoute);
    }
    while (unroutedCustomers.size() > 0)
    {
        vector<int> bestU = vector<int>();
        vector<int> bestRoute = vector<int>();
        vector<int> c1Vals = vector<int>();
        bool feasible = false;

        for (auto cust : unroutedCustomers)
        {
            double minC1 = std::numeric_limits<double>::infinity();
            int optimalU = -1;
            int optimalRoute = -1;

            for (int j = 0; j < solution.routes.size(); ++j)
            {
                for (int i = 1; i < solution.routes[j].nodeList.size(); ++i)
                {
                    double c1 = criterionC1(i-1, cust, i, solution.routes[j]);
                    solution.routes[j].insertCustomer(cust, i);
                    bool isFeasible = solution.routes[j].isFeasible();
                    
                    if (c1 < minC1 && isFeasible)
                    {
                        minC1 = c1;
                        optimalU = i;
                        optimalRoute = j;
                        feasible = true;
                   }
                    solution.routes[j].deleteCustomer(i);
               }
            }
                    
            bestRoute.push_back(optimalRoute);     
            bestU.push_back(optimalU);
            c1Vals.push_back(minC1);
        }

        if (!feasible)
        {
            //for route in solution.routes:
                //route.optimize();

            problem.unplannedCustomers = unroutedCustomers;
            break;
        }
            
        else
        {
            int bestCust = 0;
            double minC2 = std::numeric_limits<double>::infinity();
            int route = 0;
            for (int i = 0; i < unroutedCustomers.size(); ++i)
            {
                if (bestU[i] != -1 && bestRoute[i] != -1)
                {
                    double c2 = criterionC2(unroutedCustomers[i], c1Vals[i], solution.routes[bestRoute[i]]);
                    if (c2 < minC2)
                    {
                        minC2 = c2;
                        bestCust = i;
                        route = bestRoute[i];
                   }
               }
           }
            solution.routes[route].insertCustomer(unroutedCustomers[bestCust], bestU[bestCust]);
            unroutedCustomers.erase(unroutedCustomers.begin()+bestCust);
        }
        if (unroutedCustomers.size() == 0)
        {
            //for route in solution.routes:
                //route.optimize()
            break;
       }
    }
    return solution;
}

Solution SolomonConstructive::solve(Problem& problem, SolomonStrategy strategy, Solution solution) 
{
    vector<Customer> unroutedCustomers = problem.getCustomers();
    while (unroutedCustomers.size() > 0)
    {
        vector<int> bestU = vector<int>();
        vector<int> bestRoute = vector<int>();
        vector<double> c1Vals = vector<double>();
        bool feasible = false;
        
        for (auto cust : unroutedCustomers)
        {
            double minC1 = std::numeric_limits<double>::infinity();
            int optimalU = -1;
            int optimalRoute = -1;

            for (int j = 0; j < solution.routes.size(); ++j)
            {
                for (int i = 1; i < solution.routes[j].nodeList.size(); ++i)
                {
                    double c1 = criterionC1(i-1, cust, i, solution.routes[j]);
                    solution.routes[j].insertCustomer(cust, i);
                    bool isFeasible = solution.routes[j].isFeasible();
                    
                    if (c1 < minC1 && isFeasible)
                    {
                        minC1 = c1;
                        optimalU = i;
                        optimalRoute = j;
                        feasible = true;
                   }
                    solution.routes[j].deleteCustomer(i);
               }
            }
                    
            bestRoute.push_back(optimalRoute);     
            bestU.push_back(optimalU);
            c1Vals.push_back(minC1);
        }

        if (!feasible)
        {
            //for route in solution.routes:
                //route.optimize();

            problem.unplannedCustomers = unroutedCustomers;
            break;
        }
            
        else
        {
            int bestCust = 0;
            double minC2 = std::numeric_limits<double>::infinity();
            int route = 0;
            for (int i = 0; i < unroutedCustomers.size(); ++i)
            {
                if (bestU[i] != -1 && bestRoute[i] != -1)
                {
                    double c2 = criterionC2(unroutedCustomers[i], c1Vals[i], solution.routes[bestRoute[i]]);
                    if (c2 < minC2)
                    {
                        minC2 = c2;
                        bestCust = i;
                        route = bestRoute[i];
                   }
               }
           }
            solution.routes[route].insertCustomer(unroutedCustomers[bestCust], bestU[bestCust]);
            unroutedCustomers.erase(unroutedCustomers.begin()+bestCust);
        }
        if (unroutedCustomers.size() == 0)
        {
            //for route in solution.routes:
                //route.optimize()
            break;
       }
    }
    return solution;
}

struct custValueComp
{
    bool operator() (const Customer& x, const Customer& y)
    {
        return x.info.value < y.info.value;
    }
} customerCompObj;

Solution SolomonConstructive::solveOptional(Problem& problem, Solution solution) 
{
    vector<Customer> unroutedCustomers = problem.getCustomers();
    std::sort(unroutedCustomers.begin(), unroutedCustomers.end(), customerCompObj);

    int i = unroutedCustomers.size()-1;        
    while (i >= 0)
    {
        //cust = unroutedCustomers[i]
        vector<int> bestU = vector<int>();// = vector<int>();
        vector<double> c1Vals = vector<double>();
        bool feasible = false;
        for (int j = 0; j < solution.routes.size(); ++j)
        {
            double minC1 = std::numeric_limits<double>::infinity();
            int optimalU = -1;
            for (int k = 1; i < solution.routes[k].nodeList.size(); k++)
            {
                double c1 = criterionC1(k-1, unroutedCustomers[i], k, solution.routes[j]);
                solution.routes[j].insertCustomer(unroutedCustomers[i], k);
                bool isFeasible = solution.routes[j].isFeasible();
                
                if (c1 < minC1 && isFeasible)
                {
                    minC1 = c1;
                    optimalU = k;
                    feasible = true;
                }
                solution.routes[j].deleteCustomer(k);
            }    
            bestU.push_back(optimalU);
            c1Vals.push_back(minC1);
        }

        if (feasible)
        {
            double minC2 = std::numeric_limits<double>::infinity();
            int bestInsert = -1;
            for (int j = 0; j < solution.routes.size(); j++)
            {
                if (bestU[j] != -1)
                {
                    double c2 = criterionC2(unroutedCustomers[i], c1Vals[j], solution.routes[j]);
                    if (c2 < minC2)
                    {
                        minC2 = c2;
                        bestInsert = j;
                    }         
                }       
            }
            solution.routes[bestInsert].insertCustomer(unroutedCustomers[i], bestU[bestInsert]);
            unroutedCustomers.erase(unroutedCustomers.begin()+i);
       }     
        --i;
    }
    //for route in solution.routes:
            //route.optimize()

    problem.unplannedCustomers = unroutedCustomers;

    return solution;
}

Solution SolomonConstructive::solveOptional(Problem& problem) 
{
    vector<Customer> unroutedCustomers = problem.getCustomers();
    std::sort(unroutedCustomers.begin(), unroutedCustomers.end(), customerCompObj);
    //for (auto& cust: unroutedCustomers)
        //cout << "Customer " << cust.info.id << " " << cust.info.value << " ";
    //cout << endl;
    Solution solution = Solution(problem);
    for (int i = 0; i < problem.planningHorizon; i++)
        solution.routes.push_back(Route(problem));

    int i = unroutedCustomers.size()-1;        
    while (i >= 0)
    {
        //cust = unroutedCustomers[i]
        vector<int> bestU = vector<int>();// = vector<int>();
        vector<double> c1Vals = vector<double>();
        bool feasible = false;
        for (int j = 0; j < solution.routes.size(); ++j)
        {
            double minC1 = std::numeric_limits<double>::infinity();
            int optimalU = -1;
            for (int k = 1; k < solution.routes[j].nodeList.size(); k++)
            {
                double c1 = criterionC1(k-1, unroutedCustomers[i], k, solution.routes[j]);
                solution.routes[j].insertCustomer(unroutedCustomers[i], k);
                bool isFeasible = solution.routes[j].isFeasible();
                
                if (c1 < minC1 && isFeasible)
                {
                    minC1 = c1;
                    optimalU = k;
                    feasible = true;
                }
                solution.routes[j].deleteCustomer(k);
            }    
            bestU.push_back(optimalU);
            c1Vals.push_back(minC1);
        }
        
        if (feasible)
        {
            double minC2 = std::numeric_limits<double>::infinity();
            int bestInsert = -1;
            for (int j = 0; j < solution.routes.size(); j++)
            {
                if (bestU[j] != -1)
                {
                    double c2 = criterionC2(unroutedCustomers[i], c1Vals[j], solution.routes[j]);
                    if (c2 < minC2)
                    {
                        minC2 = c2;
                        bestInsert = j;
                    }         
                }       
            }
            //cout << "Customer " << unroutedCustomers[i].info.id << " " << c1Vals[bestInsert] << " " << minC2 << endl;
            solution.routes[bestInsert].insertCustomer(unroutedCustomers[i], bestU[bestInsert]);
            unroutedCustomers.erase(unroutedCustomers.begin()+i);
       }     
        --i;
    }
    //for route in solution.routes:
            //route.optimize()

    problem.unplannedCustomers = unroutedCustomers;

    return solution;
}
/* 
Solution SolomonConstructive::sequentialBestInsert(vector<Customer>& unroutedCustomers, Solution solution, int route) 
{
    int i = 0;        
    while (i < unroutedCustomers.size())
    {
        //cust = unroutedCustomers[i]
        vector<int> bestU = vector<int>();// = vector<int>();
        vector<double> c1Vals = vector<double>();
        bool feasible = false;
        for (int j = 0; j < solution.routes.size(); ++j)
        {
            double minC1 = std::numeric_limits<double>::infinity();
            int optimalU = -1;
            for (int k = 1; k < solution.routes[j].nodeList.size(); k++)
            {
                double c1 = criterionC1(k-1, unroutedCustomers[i], k, solution.routes[j]);
                solution.routes[j].insertCustomer(unroutedCustomers[i], k);
                bool isFeasible = solution.routes[j].isFeasible();
                
                if (c1 < minC1 && isFeasible)
                {
                    minC1 = c1;
                    optimalU = k;
                    feasible = true;
                }
                solution.routes[j].deleteCustomer(k);
            }    
            bestU.push_back(optimalU);
            c1Vals.push_back(minC1);
        }
        
        if (feasible)
        {
            double minC2 = std::numeric_limits<double>::infinity();
            int bestInsert = -1;
            for (int j = 0; j < solution.routes.size(); j++)
            {
                if (bestU[j] != -1)
                {
                    double c2 = criterionC2(unroutedCustomers[i], c1Vals[j], solution.routes[j]);
                    if (c2 < minC2)
                    {
                        minC2 = c2;
                        bestInsert = j;
                    }         
                }       
            }
            solution.routes[bestInsert].insertCustomer(unroutedCustomers[i], bestU[bestInsert]);
            unroutedCustomers.erase(unroutedCustomers.begin()+i);
       }     
        i = i + 1;
    }
    //for route in solution.routes:
            //route.optimize()

    return solution;
}
*/
Solution SolomonConstructive::sequentialBestInsert(vector<Customer>& unroutedCustomers, Solution& solution, int route) 
{
    int i = 0;        
    while (i < unroutedCustomers.size())
    {
        //cust = unroutedCustomers[i]
        bool feasible = false;
        double minC1 = std::numeric_limits<double>::infinity();
        int optimalU = -1;
        for (int k = 1; k < solution.routes[route].nodeList.size(); k++)
        {
            double c1 = criterionC1(k-1, unroutedCustomers[i], k, solution.routes[route]);
            solution.routes[route].addCustomer(unroutedCustomers[i], k);
            
            if (c1 < minC1 && solution.routes[route].isFeasible())
            {
                minC1 = c1;
                optimalU = k;
                feasible = true;
            }
            solution.routes[route].removeCustomer(k);
        }    
        if (feasible)
        {
            solution.routes[route].addCustomer(unroutedCustomers[i], optimalU);
            unroutedCustomers.erase(unroutedCustomers.begin()+i);
        }     
        i = i + 1;
    }
    //for route in solution.routes:
            //route.optimize()
    return solution;
}

double SolomonConstructive::criterionC1(int i, Customer& u, int j, Route& route) 
{
    return coefAlpha1*criterionC11(i, u, j, route) + coefAlpha2*criterionC12(i, u, j, route);
}

double SolomonConstructive::criterionC11(int i, Customer& u, int j, Route& route) 
{
    double distIu = route.nodeList[i].getTravelTime(u);
    double distUj = u.getTravelTime(route.nodeList[j]);
    double distJi = route.nodeList[j].getTravelTime(route.nodeList[i]);
    return distIu + distUj - coefMu*distJi;
}

double SolomonConstructive::criterionC12(int i, Customer& u, int j, Route& route) 
{
    double bI = route.serviceStartTimes[i];
    double bU = route.nextServiceStartTime(u, route.nodeList[i], bI);
    double bJ = route.serviceStartTimes[j];
    double bJu = route.nextServiceStartTime(route.nodeList[j], u, bU);
    return bJu - bJ;
}

double SolomonConstructive::criterionC2(Customer& u, double c1Value, Route& route) 
{
    double d0U = route.nodeList[0].getTravelTime(u);
    return coefLambda * d0U - c1Value;
}
