//#include "../include/Utils.h"
#include "../include/RouteUtils.h"
#include "../include/Solution.h"
#include "../include/Problem.h"
#include "../include/VNS.h"
#include "../include/Reader.h"
#include "../include/Utils.h"
#include <iostream>
#include <vector>
using namespace std;
double ** matrix;
int main()
{
    string FILENAME= "c101_100";
            //string FILENAME= "c102_100";
    cout << "hi" << endl;
    readDistMatrix(FILENAME, matrix);
    cout << "hi" << endl;
    vector<Customer> customers = vector<Customer>();
    readCustomers(FILENAME, customers); 
    cout << "hi" << endl;
    customers = slice(customers, 0, 11);
    Customer::setTimeMatrix(matrix);
    cout << "hi" << endl;
    for(int i = 0; i < customers.size()/2; i++) customers[i].info.mandatory = false;
    for(int i = customers.size()/2; i < customers.size(); i++) customers[i].info.mandatory = false;
    cout << "hi" << endl;
    auto depot = customers[0];
    depot.info.mandatory = true;
    cout << "hi" << endl;
    Problem problem = Problem();  
    problem.resetCustomers(customers, customers[0].info.id);
    cout << "hi" << endl;
    problem.planningHorizon = 5;
    cout << "hi" << endl;
    Route route1 = Route(problem);
    Route route2 = Route(problem);
    cout << "hi" << endl;
    auto tempCustomers = slice(customers, 5, 10);
    route1.setCustomers(vector<Customer>{depot, customers[1], depot});
    vector<Route> routes = vector<Route>{route1};
    for (int i = 0; i < 100; i++)
    {
        //for (int n = CROSS_EXCHANGE_START; n < OPT_EXCHANGE_1_START; n++)
        for (int n = OPT_EXCHANGE_1_START; n < OPT_EXCHANGE_2_START; n++)
        //for (int n = OPT_EXCHANGE_2_START; n < END; n++)
        {
            //route1.setCustomers(vector<Customer>{depot, customers[1], customers[2], customers[3], customers[4], depot});
            //route2.nodeList = vector<Customer>{depot, customers[3], customers[4], depot};
            //vector<Route> routes = vector<Route>{route1};
            //crossExchange(routes, n);
            //optExchange1(routes, tempCustomers, n);
            route1.optimize();
            cout << "Neighborhood: " << n << endl;
            cout << "Iteration: " << i << endl;
            printCustomers(routes[0]);
            //printCustomers(tempCustomers);
            //printCustomers(routes[1]);
        }
    }

}