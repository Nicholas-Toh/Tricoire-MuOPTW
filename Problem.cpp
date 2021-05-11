#include "..\include\Problem.h"
#include <memory>
using namespace std;

shared_ptr<double**> Customer::timeMatrix = nullptr;

Problem::Problem()
{
    depot = Customer();
    customers = vector<Customer>();
    unplannedCustomers = vector<Customer>();
    routeDurationConstraint = 0;
    planningHorizon = 7;
}

Problem::Problem(const Problem& rhs) : depot(rhs.depot)
{
    depot = rhs.depot;
    customers = rhs.customers;
    unplannedCustomers = rhs.unplannedCustomers;
    routeDurationConstraint = rhs.routeDurationConstraint;
    planningHorizon = rhs.planningHorizon;
}

Customer::Customer(){}

Customer::Customer(Info info)
{
    this->info = info;
}

Customer::Customer(const Customer& rhs)
{
    info = rhs.info;
    //timeMatrix = rhs.timeMatrix;
}

double Customer::getTravelTime(const Customer& node)
{
    return (*timeMatrix)[info.id][node.info.id];
}

double Customer::getTravelTime(vector<Customer>::iterator node)
{
    return (*timeMatrix)[info.id][node->info.id];
}

void Customer::setTimeMatrix(double** ptr) 
{
    timeMatrix = make_shared<double**>(ptr);
}

void Problem::resetCustomers(vector<Customer> newCustomers, int depot)
{
    customers = newCustomers;
    this->depot = customers[depot];
    routeDurationConstraint = this->depot.info.endTime;
    customers.erase(customers.begin()+depot);
}

vector<Customer> Problem::getCustomers() {
    return customers;
}

void Problem::setCustomers(vector<Customer> newCustomers) {
    customers = newCustomers;
}

void Problem::setUnplannedCustomers(vector<Customer> newCustomers) {
    unplannedCustomers = newCustomers;
}


vector<Customer> Problem::getUnplannedCustomers() {
    return unplannedCustomers;
}