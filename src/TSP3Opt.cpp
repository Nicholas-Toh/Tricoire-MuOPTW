#include "../include/TSP3Opt.h"
#include "../include/RouteUtils.h"
#include "../include/Utils.h"
#include <map>
#include <algorithm>
#include <iostream>

vector<std::tuple<int, int, int>> possibleSegments(int n) 
{
    vector<std::tuple<int, int, int>> segments = vector<std::tuple<int, int, int>>();

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            int end;
            if (n - (j + 1) < 3)
                end = n;
            else
                end = j + 4;
            for (int k = j + 1; k < end; ++k)
            {
                segments.push_back(std::tuple<int, int, int>{i, j, k});
            }
        } 
    }

    return segments;
}

double getSolutionCostChange(vector<Customer>& route, OptCase optcase, int i, int j, int k) 
{
    //A route[i - 1], B route[i], C route[j - 1], D route[j], E route[k - 1], F route[k % route.size()];
    //A.getTravelTime(B) + C.getTravelTime(D) + E.getTravelTime(F) - (A.getTravelTime(D) + C.getTravelTime(F) + B.getTravelTime(E))
    int A = i-1;
    if (A < 0)
        A = route.size()-1;
    int B = i;
    int C = j-1;
    int D = j;
    int E = k-1;
    int F = k % route.size();
    if (optcase == OptCase::OPT_CASE_8)
        //A'B'C' = ACB
        return route[A].getTravelTime(route[B]) + route[C].getTravelTime(route[D]) + route[E].getTravelTime(route[F]) 
        - (route[A].getTravelTime(route[D]) + route[C].getTravelTime(route[F]) + route[B].getTravelTime(route[E]));

}

vector<Customer> reverseSegments(vector<Customer>& route, OptCase optcase, int i, int j, int k) 
{
    vector<Customer> solution, first_segment, second_segment, third_segment;
    //cout << "here???";
    if ((i - 1) < (int)(k % route.size()))
    {
        //cout << "hel";
        first_segment = slice(route, k % route.size(), route.size()) + slice(route, 0, i);
    }
    else
    {
         //cout << (i-1)  << " " << (k % route.size()) <<  " " << ((i - 1) > (k % route.size()));
        first_segment = slice(route, k % route.size(), i);
    }
    //cout << "this";
    //second_segment = slice(route, i, j);
    //cout << "that";
    //third_segment = slice(route, j, k);
    //cout << "lol";
    if (optcase == OptCase::OPT_CASE_8)
        //A'B'C' = ACB This is the only case that does not involve chain inversion
        //solution = first_segment + third_segment + second_segment;
        first_segment.insert(first_segment.end(), route.begin()+j, route.begin()+k);
        first_segment.insert(first_segment.end(), route.begin()+i, route.begin()+j);
        return first_segment;

        //cout << "kek";
    //printCustomers(route);
    //printCustomers(solution);
    //cout << i << " "  << j << " "  << k << " "  << optcase << " " << endl;
    //return solution;
}

void cycleToDepot(int depot, vector<Customer>& route) 
{
    for (int i = 0; i < route.size(); ++i)
    {
        if (route[i].info.id == depot)
            route = slice(route, i, route.size()) + slice(route, 0, i);
    }   
}

void tsp3Opt(int depot, vector<Customer>& bestRoute) 
{
    std::map<OptCase, double> moveCost = {{OptCase::OPT_CASE_8, 0.0}};
    double worstInfeasiblity = calculate_infeasibility(bestRoute);
    bool improved = true;
    const OptCase optCases[] {OptCase::OPT_CASE_8}; //translated from python, need better way to iterate over enums
    while (improved)
    {
        improved = false;
        //cout << "here" << endl;
        double infeasiblity = calculate_infeasibility(bestRoute);
        //cout << "there" << endl;
        for (auto& segment : possibleSegments(bestRoute.size()))
        {
            int i = std::get<0>(segment);
            int j = std::get<1>(segment);
            int k = std::get<2>(segment);
            
            for (auto& opt_case : optCases)
                    moveCost[opt_case] = getSolutionCostChange(bestRoute, opt_case, i, j, k);
            OptCase bestCase = max_element(moveCost.begin(), moveCost.end(), [] (const std::pair<OptCase,double>& a, const std::pair<OptCase,double>& b)->bool{ return a.second < b.second;})->first;
            vector<Customer> candidateRoute = reverseSegments(bestRoute, bestCase, i, j, k);
            double candidateInfeasibility = calculate_infeasibility(candidateRoute);
            //std::cout << moveCost[bestCase] << " " << candidateInfeasibility << " " << worstInfeasiblity << std::endl;
            //int temp = (int)(moveCost[bestCase]*10000 + 0.5); //round up
            double temp = ((double)(int)(moveCost[bestCase] *10000 + 0.5))/10000;
            if (temp >= 0.0 && candidateInfeasibility <= worstInfeasiblity)
            {
                if (temp > 0.0 || candidateInfeasibility < worstInfeasiblity)
                
                {   //cout << "ACCEPTED***********" << endl;
                   // printCustomers(candidateRoute);
                    //printCustomers(bestRoute);
                   // std::cout << moveCost[bestCase] << " " << candidateInfeasibility << " " << worstInfeasiblity << std::endl;
                    //std::cout << (int)(moveCost[bestCase]*10000 + 0.5) << (((int)(moveCost[bestCase]*10000 + 0.5))/10000) << ((double)((int)(moveCost[bestCase]*10000 + 0.5)))/10000 << std::endl;
                    bestRoute = candidateRoute;
                   // printCustomers(candidateRoute);
                    //printCustomers(bestRoute);
                    worstInfeasiblity = candidateInfeasibility;
                    improved = true;
                    break;
                }
            }
        }
        //std::cout <<improved << std::endl;
    }                          
    cycleToDepot(depot, bestRoute);
}
