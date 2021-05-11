#include "..\include\RouteUtils.h"
#include <iostream>

double calculate_infeasibility(vector<Customer>& route)
{
    double actualStartTime = 0; 
    double infeasibility = 0;
    for (int i = 1; i < route.size(); i++)
    {    
        actualStartTime = max(route[i].info.startTime - route[i-1].getTravelTime(route[i]), actualStartTime + route[i-1].info.serviceTime + route[i-1].getTravelTime(route[i]));
        double infeasibleTime = max(0.0, min(route[i].info.serviceTime, actualStartTime + route[i].info.serviceTime - route[i].info.endTime));
        infeasibility += infeasibleTime;
    }
    return infeasibility;
}


void printCustomers(Route route)
{
    std::cout << "[";
    for (Customer& cust : route.nodeList)
    {
        std::cout << cust.info.id << ", ";
    }
    std::cout << "]" << std::endl;
}

void printCustomers(vector<Customer> route)
{
    std::cout << "[";
    for (Customer& cust : route)
    {
        std::cout << cust.info.id << ", ";
    }
    std::cout << "]" << std::endl;
}

void printSolution(Solution solution)
{
    for (Route& route : solution.routes)
    {
        printCustomers(route);
    }
}