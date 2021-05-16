#include "../include/VNS.h"
#include "../include/Utils.h"
#include "../include/TSP3Opt.h"
#include "../include/RouteUtils.h"
#include <iostream>
#include <algorithm>
#include <chrono>

Solution variableNeighborhoodSearch(Solution bestSolution, Problem& problem, int maxLastAcceptedDuration, int maxIterations, int timeLimit) 
{
    int count = 0;
    int lastAccepted = 0;
    SolomonConstructive constructive = SolomonConstructive();
    chrono::time_point<chrono::system_clock> end;
    auto ms = chrono::milliseconds(timeLimit*1000);
    end = chrono::system_clock::now() + ms;
    while (count < maxIterations && chrono::system_clock::now() < end)
    {

        for (int i = CROSS_EXCHANGE_START; i < END; ++i)
        {
            vector<Customer> unroutedCustomers = problem.getUnplannedCustomers();
            Solution candidate = bestSolution;
            bool feasible = false;
            if (i < OPT_EXCHANGE_1_START)
            {
                if (candidate.routes.size() >= 2)
                {    
                    //cout << "Started at CROSS_EXCHANGE" << endl;
                    std::pair<int, int> routeIdx = crossExchange(candidate.routes, i);
                    int routeIdx1 = std::get<0>(routeIdx);
                    int routeIdx2 = std::get<1>(routeIdx);
                    auto route1 = slice(candidate.routes[routeIdx1].nodeList, 0, candidate.routes[routeIdx1].nodeList.size()-1);
                    auto route2 = slice(candidate.routes[routeIdx2].nodeList, 0, candidate.routes[routeIdx2].nodeList.size()-1);
                    //cout << "started 3opt" << " OPT_EXCHANGE_1_START " << endl;
                    tsp3Opt(problem.depot.info.id, route1);
                    tsp3Opt(problem.depot.info.id, route2);
                    //cout << "finished 3opt" << " OPT_EXCHANGE_1_START " << endl;
                    candidate.routes[routeIdx1].nodeList = route1;
                    candidate.routes[routeIdx2].nodeList = route2;
                    candidate.routes[routeIdx1].nodeList.push_back(candidate.routes[routeIdx1].problem.depot);
                    candidate.routes[routeIdx2].nodeList.push_back(candidate.routes[routeIdx2].problem.depot);
                    feasible = candidate.routes[routeIdx1].isFeasible() and candidate.routes[routeIdx2].isFeasible();
                    //cout << "Ended at CROSS_EXCHANGE" << endl;
                }
            }
            if (i < OPT_EXCHANGE_2_START and i >= OPT_EXCHANGE_1_START)
            {
                //cout << "Started at OPT_EXCHANGE_1_START" << endl;
                int routeIdx = optExchange1(candidate.routes, unroutedCustomers, i);
                auto route = slice(candidate.routes[routeIdx].nodeList, 0, candidate.routes[routeIdx].nodeList.size()-1);
                //cout << "2" << endl;
                tsp3Opt(problem.depot.info.id, route);
                //cout << "3";
                candidate.routes[routeIdx].nodeList = route;
                candidate.routes[routeIdx].nodeList.push_back(candidate.routes[routeIdx].problem.depot);
                //cout << "4";
                feasible = candidate.routes[routeIdx].isFeasible();
                //cout << "5";
                //cout << "Ended at OPT_EXCHANGE_1_START" << endl;
            }
            if (i >= OPT_EXCHANGE_2_START and i < END)
            {
                //cout << "Started at OPT_EXCHANGE_2_START" << endl;
                int routeIdx = optExchange2(candidate.routes, unroutedCustomers, i);
                constructive.sequentialBestInsert(unroutedCustomers, candidate, routeIdx);
                //cout << unroutedCustomers.size() << " " << endl;
                //feasible = candidate.routes[routeIdx].isFeasible();
                feasible = true;
                //cout << "Ended at OPT_EXCHANGE_2_START" << endl;
            }
            if (feasible)
            {
                bool ascending;
                if (lastAccepted >= maxLastAcceptedDuration) ascending = true; else ascending = false;
                if (acceptanceDecision(bestSolution, candidate, i, "profit", ascending))
                {
                    bestSolution = candidate;
                    lastAccepted = -1;
                    problem.setUnplannedCustomers(unroutedCustomers);
                    break;
                 }   
            }
            else
            {
            }
        }        
        ++count;
        ++lastAccepted;
        
        std::cout << "Iteration = " << count << "-> Distance " << sum(bestSolution.calculateDistance()) << " Value " << sum(bestSolution.calculateValue()) << std::endl;
    }
    cout << "Iterations: " << count << endl;
    return bestSolution;
}

std::pair<int, int> crossExchange(vector<Route>& routes, int neighborhood) 
{
    if (routes.size() < 2)
        std::cout << "There must be at least 2 routes for cross exchange";

    if (neighborhood < CROSS_EXCHANGE_START or neighborhood >= OPT_EXCHANGE_1_START)
        std::cout << "Neighborhood size of cross exchange must be between " << CROSS_EXCHANGE_START<< " and " << OPT_EXCHANGE_1_START-1;

    int minLength1 = 1;
    int minLength2 = 0;
    int routeIdx1 = randint(0, routes.size()-1);
    int routeIdx2 = randint(0, routes.size()-1);
    while (routeIdx2 == routeIdx1)
        routeIdx2 = randint(0, routes.size()-1);

    if (routes[routeIdx1].nodeList.size()-2 == 0 or routes[routeIdx2].nodeList.size()-2 == 0)
        return std::pair<int, int>(routeIdx1, routeIdx2);
        
    int maxLength1 = min((int)routes[routeIdx1].nodeList.size()-2, neighborhood);
    int maxLength2 = min((int)routes[routeIdx2].nodeList.size()-2, neighborhood);
    int newLength1 = randint(minLength1, maxLength1);
    int newLength2 = randint(minLength2, maxLength2);
    int exchangePt1 = randint(1, routes[routeIdx1].nodeList.size()-1 - newLength1);
    int exchangePt2 = randint(1, routes[routeIdx2].nodeList.size()-1 - newLength2);
    swap_seq(routes[routeIdx1].nodeList, exchangePt1, exchangePt1 + newLength1, routes[routeIdx2].nodeList, exchangePt2,  exchangePt2 + newLength2);
    routes[routeIdx1].resetServiceTimes();
    routes[routeIdx2].resetServiceTimes();

    return std::pair<int, int>(routeIdx1, routeIdx2);
}

int optExchange1(vector<Route>& routes, vector<Customer>&unplannedCustomers, int neighborhood) 
{
    if (neighborhood < OPT_EXCHANGE_1_START or neighborhood >= OPT_EXCHANGE_2_START)
        std::cout << "Neighborhood size of cross exchange must be between " << OPT_EXCHANGE_1_START<< " and " << OPT_EXCHANGE_2_START-1;

    int p = 0;
    int q = 0;
    
    if (neighborhood == OPT_EXCHANGE_1_START)
    {
        p = 0;
        q = 1;
    }
    else if (neighborhood == OPT_EXCHANGE_1_START + 1)
    {
        p = 1;
        q = 1;
    }
    else if (neighborhood == OPT_EXCHANGE_1_START + 2)
    {
        p = 2;
        q = 1;
    }
    else if (neighborhood == OPT_EXCHANGE_1_START + 3)
    {
        p = 0;
        q = 2;
    }
    int routeIdx = randint(0, routes.size()-1);
    int numOptCustomers = routes[routeIdx].calculateOptionalCustomers();
    vector<Customer> route = slice(routes[routeIdx].nodeList, 1, routes[routeIdx].nodeList.size()-1);
    vector<Customer> removedCustomers = vector<Customer>();
    p = min(p, numOptCustomers);
    q = min(q, (int)unplannedCustomers.size());
    
    while (p > 0 || q > 0)
    {
        int position;
        if (route.size()-p < 0) 
            position = 0;
        else 
            position = randint(0, route.size()-p);
        
        //cout << "Before remove" << endl;
        //printCustomers(route);
        if (p > 0)
        {
            while (route.at(position).info.mandatory)
            {
                position = (position + 1) % (route.size());
            }
            Customer cust = route.at(position);
            route.erase(route.begin()+position);
            removedCustomers.push_back(cust);
            //cout << "just before" << endl;
            //cout << cust.info.id << " " << position << endl;
            //cout << "just after" << endl;
            --p;
        }
        //cout << "After remove" << endl;
        //printCustomers(route);
        if (q > 0)
        {
            int i = randint(0, unplannedCustomers.size()-1);
            route.insert(route.begin()+position, unplannedCustomers.at(i));
            unplannedCustomers.erase(unplannedCustomers.begin()+i);
            --q;
        }
        //cout << "After insert" << endl;
        //printCustomers(route);
    }
    //cout << "here" << endl;
    routes[routeIdx].nodeList = slice(routes[routeIdx].nodeList, 0, 1) + route + slice(routes[routeIdx].nodeList, routes[routeIdx].nodeList.size()-1, routes[routeIdx].nodeList.size());
    routes[routeIdx].resetServiceTimes();
    //printCustomers(unplannedCustomers);
    //printCustomers(removedCustomers);
    unplannedCustomers =  unplannedCustomers + removedCustomers;

    return routeIdx;
}

int optExchange2(vector<Route>& routes, vector<Customer>&unplannedCustomers, int neighborhood) 
{
    if (neighborhood < OPT_EXCHANGE_2_START or neighborhood >= END)
        std::cout << "Neighborhood size of cross exchange must be between " << OPT_EXCHANGE_2_START<< " and " << END-1;

    int routeIdx = randint(0, routes.size()-1);
    int numOptCustomers = routes[routeIdx].calculateOptionalCustomers();
    vector<Customer> route = slice(routes[routeIdx].nodeList, 1, routes[routeIdx].nodeList.size()-1);
    vector<Customer> removedCustomers = vector<Customer>();
    
    int p = min(neighborhood - 12, numOptCustomers);
    int position;
    if (route.size()-p < 0) 
        position = 0;
    else 
        position = randint(0, route.size()-p);
    for (int i = 0; i < p; i++)
    {
        
        while (route[position].info.mandatory)
        {
            position = (position + 1) % (route.size());
        }
        if (!route[position].info.mandatory)
        {
            Customer cust = route[position];
            route.erase(route.begin()+position);
            removedCustomers.push_back(cust);
            if (route.size() > 0)
                position = position % (route.size());
            else break;
        }  
    }     
    routes[routeIdx].nodeList = slice(routes[routeIdx].nodeList, 0, 1) + route + slice(routes[routeIdx].nodeList, routes[routeIdx].nodeList.size()-1, routes[routeIdx].nodeList.size());
    routes[routeIdx].resetServiceTimes();
    unplannedCustomers = unplannedCustomers + removedCustomers;

    return routeIdx;
}

bool acceptanceDecision(Solution& bestSolution, Solution& candidateSolution, int neighborhood, string mode, bool ascending) 
{
    if (neighborhood < CROSS_EXCHANGE_START or neighborhood >= END)
        std::cout << "Invalid neighborhood" << std::endl;
    
    int bestSolutionLength = sum(bestSolution.calculateDistance());
    int candidateSolutionLength = sum(candidateSolution.calculateDistance());
    
    if (mode == "profit")
    {
        if (sum(candidateSolution.calculateValue()) >  sum(bestSolution.calculateValue()))
            return true;
        else if (candidateSolutionLength < bestSolutionLength and (sum(candidateSolution.calculateValue()) ==  sum(bestSolution.calculateValue())))
            return true;
    }

    if (mode == "distance")
    {
        if (candidateSolutionLength < bestSolutionLength)
            return true;
    }
        
    if (ascending)
    {
        if (neighborhood < OPT_EXCHANGE_1_START)
        {
            if (candidateSolutionLength/bestSolutionLength < 1.005)
                return true;
            else
                return false;
        }
        else
            return true;
    }

    return false;
}