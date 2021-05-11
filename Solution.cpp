#include "..\include\Solution.h"
#include <iostream>
#include <algorithm>
using namespace std;

Route::Route(Problem newProblem): problem(newProblem){
    nodeList = vector<Customer>();
    serviceStartTimes = vector<double>();
    addDepot(problem.depot);
    solution = vector<Customer>();
    dominantSolution = vector<double>();
}

Route::Route(Problem problem, Customer seed): problem(problem){
    nodeList = vector<Customer>();
    serviceStartTimes = vector<double>();
    addDepot(problem.depot);
    setSeed(seed);
    solution = vector<Customer>();
    dominantSolution = vector<double>();
}

void Route::setSeed(Customer seed) {
    insertCustomer(seed, 1);
}

double Route::nextServiceStartTime(Customer newCustomer, Customer prevCustomer, double prevTime) {
    double travelTime = prevCustomer.getTravelTime(newCustomer);
    double serviceTime = prevCustomer.info.serviceTime;
    double startTime = newCustomer.info.startTime;
    return max(startTime, prevTime + serviceTime + travelTime);
}

void Route::addDepot(Customer depot) {
    insertCustomer(depot, 0);
    insertCustomer(depot, 1);
}

void Route::addCustomer(Customer customer, int position) {
    nodeList.insert(nodeList.begin()+position, customer);
    serviceStartTimes.insert(serviceStartTimes.begin()+position, 0.0);
}

void Route::removeCustomer(int position) {
    nodeList.erase(nodeList.begin()+position);
    serviceStartTimes.erase(serviceStartTimes.begin()+position);
}

void Route::insertCustomer(Customer customer, int position) {
    nodeList.insert(nodeList.begin()+position, customer);
    serviceStartTimes.insert(serviceStartTimes.begin()+position, 0.0);
    for (int i = position; i < nodeList.size() && i > 0; ++i)
    {
        double newTime = nextServiceStartTime(nodeList[i], nodeList[i-1], serviceStartTimes[i-1]);
        serviceStartTimes[i] = newTime;
    } 
}

void Route::setCustomers(vector<Customer> customers) {
    this->nodeList = customers;
    resetServiceTimes();
}

Customer Route::deleteCustomer(int position) {
    Customer cust = *(nodeList.erase(nodeList.begin()+position));
    serviceStartTimes.erase(serviceStartTimes.begin()+position);
    for (int i = position; i < nodeList.size() && i > 0; ++i)
    {
        double newTime = nextServiceStartTime(nodeList[i], nodeList[i-1], serviceStartTimes[i-1]);
        serviceStartTimes[i] = newTime;
    }
   
    return cust;
}

void Route::resetServiceTimes() {
    serviceStartTimes = vector<double> ();
    serviceStartTimes.insert(serviceStartTimes.begin() + 0, 0.0);
    for (int i = 1; i < nodeList.size(); ++i)
    {
            double newTime = nextServiceStartTime(nodeList[i], nodeList[i-1], serviceStartTimes[i-1]);
            serviceStartTimes.push_back(newTime);
    }
            
}

bool Route::isFeasible() {
    /*
    vector<Customer> solution = vector<Customer>();
    for (int i = 0; i < nodeList.size(); i++)
    {
        solution.push_back(nodeList[i]);

        if (solution.size() > 1 and solution.size() < nodeList.size())
        {
            solution[solution.size()-1].info.startTime = max(0.0, solution[solution.size()-1].info.startTime - solution[solution.size()-2].getTravelTime(solution[solution.size()-1]));
            solution[solution.size()-1].info.serviceTime += solution[solution.size()-2].getTravelTime(solution[solution.size()-1]);       
        } 
    }
            
    solution[solution.size()-2].info.startTime = max(0.0,  solution[solution.size()-2].info.startTime - solution[solution.size()-2].getTravelTime(solution[solution.size()-1]));
    solution[solution.size()-2].info.serviceTime += solution[solution.size()-2].getTravelTime(solution[solution.size()-1]);
    */    
    vector<Customer> solution = nodeList;
    bool feasible = tightenEndTimes(solution) and tightenStartTimes(solution);
    if (feasible)
    {
        this->solution = solution;
    }
        
    return feasible;
}

void Route::optimize() {
    /*
    vector<Customer> solution = vector<Customer>();
    for (int i = 0; i < nodeList.size(); i++)
    {
        solution.push_back(nodeList[i]);

        if (solution.size() > 1 and solution.size() < nodeList.size())
        {
            solution[solution.size()-1].info.startTime = max(0.0, solution[solution.size()-1].info.startTime - solution[solution.size()-2].getTravelTime(solution[solution.size()-1]));
            solution[solution.size()-1].info.serviceTime += solution[solution.size()-2].getTravelTime(solution[solution.size()-1]);
        }
    }
    */
    vector<Customer> solution = nodeList;
    //solution[solution.size()-2].info.startTime = max(0.0,  solution[solution.size()-2].info.startTime - solution[solution.size()-2].getTravelTime(solution[solution.size()-1]));
    //solution[solution.size()-2].info.serviceTime += solution[solution.size()-2].getTravelTime(solution[solution.size()-1]);
                
                    
    bool feasible = tightenEndTimes(solution) and tightenStartTimes(solution);
    if (feasible)
    {
        this->solution = solution;
        dominantSolution = minRouteDuration(solution);
    }
}

bool Route::tightenStartTimes(vector<Customer>& solution) {
    int i = 1;
    while (i < nodeList.size() - 1)
    {
        double startTime = solution[i].info.startTime;
        double serviceTime = solution[i].info.serviceTime;
        double endTime = solution[i].info.endTime;
        if (startTime + serviceTime > endTime) ////TODO or next time window is feasible
            return false; //if that one time window doesnt work, the route is infeasible
        else
        {
            double nextStartTime = solution[i+1].info.startTime;
            
            if (startTime + serviceTime < nextStartTime)
            {
                solution[i].info.startTime = min(nextStartTime - serviceTime, endTime - serviceTime);
            }
            else if (startTime + serviceTime > nextStartTime)
            {
                solution[i+1].info.startTime = max(startTime + serviceTime, nextStartTime);
            }
            
        }
        i += 1;
    }
    if (solution[solution.size()-2].info.startTime + solution[solution.size()-2].info.serviceTime > problem.routeDurationConstraint)
        return false; //reject any solution that has the last node's earliest departure exceed the maximum time allowed
    
    return true;
}
/*
bool Route::tightenStartTimes(vector<Customer>& solution) {
    int i = 1;
    while (i < nodeList.size() - 1)
    {
        double startTime = solution[i].info.startTime;
        double endTime = solution[i].info.endTime;
        double serviceTime = solution[i].info.serviceTime;
        if (startTime + serviceTime > endTime) ////TODO or next time window is feasible
            return false; //if that one time window doesnt work, the route is infeasible
        else
        {

            double nextStartTime = solution[i+1].info.startTime;
            
            if (startTime + serviceTime < nextStartTime)
            {
                solution[i].info.startTime = min(nextStartTime - serviceTime, endTime - serviceTime);
            }
            else if (startTime + serviceTime > nextStartTime)
            {
                solution[i+1].info.startTime = max(startTime + serviceTime, nextStartTime);
            }
            
        }
        i += 1;
    }
    if (solution[solution.size()-2].info.startTime + solution[solution.size()-2].info.serviceTime > problem.routeDurationConstraint)
        return false; //reject any solution that has the last node's earliest departure exceed the maximum time allowed
    
    return true;
}
*/
bool Route::tightenEndTimes(vector<Customer>& solution) {
    int i = solution.size() - 2;
    while (i > 0)
    {
        double startTime = max(0.0, solution[i].info.startTime - solution[i-1].getTravelTime(solution[i]));
        double serviceTime = solution[i].info.serviceTime + solution[i-1].getTravelTime(solution[i]);
        if (i == nodeList.size() - 2)
        {
            startTime = max(0.0, startTime - solution[i+1].getTravelTime(solution[i]));
            serviceTime = serviceTime + solution[i+1].getTravelTime(solution[i]);
        }
        double endTime = solution[i].info.endTime;

        solution[i].info.startTime = startTime;
        solution[i].info.serviceTime = serviceTime;

        if (endTime - serviceTime < startTime) ////TODO or previous time window is feasible
            return false; //if that one time window doesnt work, the route is infeasible
        else
        {
            double previousEndTime = solution[i-1].info.endTime;
            
            if (endTime - serviceTime > previousEndTime)
            {
                solution[i].info.endTime = max(previousEndTime + serviceTime, startTime + serviceTime);
            }
                
            else if (endTime - serviceTime < previousEndTime)
            {
                solution[i-1].info.endTime = min(endTime - serviceTime, solution[i-1].info.endTime);
            }
            i -= 1;
        }  
    }

    if (solution[solution.size()-2].info.startTime + solution[solution.size()-2].info.serviceTime > problem.routeDurationConstraint)
        return false; //reject any solution that has the last node's earliest departure exceed the maximum time allowed

    return true;
}

vector<double> Route::calculateDominantSolution(vector<double>departureTimes, vector<Customer>solution)
{
    vector<double> dominantSolution = departureTimes;
    for (int i = solution.size()-3, j = departureTimes.size()-2; i > 0 && j >= 0; --i, --j)
    { //starting from the second last customer till the first,
        //drag the starting time to the latest possible time of the time window
            //print(i)
            //print(j)
        double nextServiceTime = solution[i+1].info.serviceTime;
        double nextStartTime = dominantSolution[j+1] - nextServiceTime;// - solution[i].getTravelTime(solution[i+1])
        double endTime = solution[i].info.endTime; ////Departure time is bordered by end time of the time window
        //print(f'{dominantSolution[j] - solution[i].info.serviceTime - solution[i].getTravelTime(solution[solution[solution.size()-1]])}')
        dominantSolution[j] = min(nextStartTime, endTime); ////either drag it behind the starting time of the next service or the end of the time window
        //print(f'{dominantSolution[j]}, {departureTimes[j+1]}, {solution[i+1].info.startTime - solution[i].getTravelTime(solution[i+1])}')
        //print(f'{dominantSolution[j] - solution[i].info.serviceTime - solution[i].getTravelTime(solution[solution[solution.size()-1]])}')
    }
    return dominantSolution;
}

vector<double> Route::minRouteDuration(vector<Customer> solution) {
    vector<double>departureTimes = vector<double>();
    for (int i = 1; i < nodeList.size()-1; ++i)
    {
        double startTime = solution[i].info.startTime;
        double serviceTime = solution[i].info.serviceTime;
        departureTimes.push_back(startTime + serviceTime);
    }
    return calculateDominantSolution(departureTimes, solution);
}

double Route::calculateTime() {
    double time = dominantSolution[dominantSolution.size()-1] - dominantSolution[0] + solution[0].info.serviceTime + solution[0].getTravelTime(solution[1]);

    return time;
}

double Route::calculateDistance() {
    double distance = 0;
    for (int i = 0; i < nodeList.size()-1; i++)
    {
        distance += nodeList[i].getTravelTime(nodeList[i+1]);
    }
    return distance;
}

double Route::calculateValue() {
   double value = 0;
    for (int i = 0; i < nodeList.size(); i++)
    {
        if (!nodeList[i].info.mandatory)
            value += nodeList[i].info.value;
            //cout << nodeList[i].info.value << " " << endl;
    }
    //cout << value << endl;
    return value; 
}

int Route::calculateOptionalCustomers() {
    int num = 0;
    for (int i = 0; i < nodeList.size(); i++)
    {
        if (!nodeList[i].info.mandatory && !nodeList[i].info.id == problem.depot.info.id)
            num++;
    }
    return num; 
}

int Route::calculateMandatoryCustomers() {
    int num = 0;
    for (int i = 0; i < nodeList.size(); i++)
    {
        if (nodeList[i].info.mandatory && !nodeList[i].info.id == problem.depot.info.id)
            num++;
    }
    return num; 
}
int Route::calculateCustomers() {
    int num = 0;
    for (int i = 1; i < nodeList.size()-1; i++)
    {
        num++;
    }
    return num;
}
Solution::Solution(Problem problem): problem(problem){
    routes = vector<Route>();
}

void Solution::addRoute(Route route) {
    routes.push_back(route);
}

vector<double> Solution::calculateTime() {
    vector<double>times = vector<double>();
    for (auto route: routes)
    {
        times.push_back(route.calculateTime());
    }
    return times;
}

vector<double> Solution::calculateDistance() {
    vector<double>distances = vector<double>();
    for (auto route: routes)
    {
        distances.push_back(route.calculateDistance());
    }
    return distances;
}

vector<double> Solution::calculateValue() {
    vector<double>values = vector<double>();
    for (auto route: routes)
    {
        values.push_back(route.calculateValue());
    }
    return values;
}

vector<int> Solution::calculateCustomers() {
    vector<int>values = vector<int>();
    for (auto route: routes)
    {
        values.push_back(route.calculateCustomers());
    }
    return values;
}