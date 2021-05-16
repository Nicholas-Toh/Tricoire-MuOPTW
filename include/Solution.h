#ifndef SOLUTION_H
#define SOLUTION_H

#include "Problem.h"
#include <vector>
using namespace std;

class Route{
    public:
        Problem problem;
        vector<Customer> nodeList;
        vector<double> serviceStartTimes;
        vector<Customer> solution;
        vector<double> dominantSolution; 

        Route(Problem problem);
        Route(Problem problem, Customer seed);

        void setSeed(Customer seed);
        double nextServiceStartTime(Customer newCustomer, Customer prevCustomer, double time);
        void addDepot(Customer depot);
        void addCustomer(Customer customer, int position);
        void removeCustomer(int position);
        void insertCustomer(Customer customer, int position);
        void setCustomers(vector<Customer> customers);
        Customer deleteCustomer(int position);
        void resetServiceTimes(); 
        bool isFeasible();
        void optimize();
        bool tightenStartTimes(vector<Customer>& solution); 
        bool tightenEndTimes(vector<Customer>& solution); 
        vector<double> minRouteDuration(vector<Customer> solution); 
        vector<double> calculateDominantSolution(vector<double> departureTimes, vector<Customer> solution);
        double calculateTime();
        double calculateDistance();
        double calculateValue();
        int calculateOptionalCustomers();
        int calculateMandatoryCustomers();
        int calculateCustomers();
};

class Solution
{
    public:
        vector<Route> routes;
        Problem problem;

        Solution(Problem problem);

        void addRoute(Route route);
        vector<double> calculateTime();
        vector<double> calculateDistance();
        vector<double> calculateValue();
        vector<int> calculateCustomers();
};

#endif