#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <memory>
using namespace std;

struct Info
{
    int id;
    int x;
    int y;
    int value;
    double startTime;
    double endTime;
    double serviceTime;
    bool mandatory;
};

class Customer{
    public:
        Info info;

        Customer(Info info);
        Customer(const Customer& rhs);
        Customer();

        double getTravelTime(const Customer& cust);
        double getTravelTime(vector<Customer>::iterator cust);

        static void setTimeMatrix(double**);

    private:
        static shared_ptr<double**> timeMatrix;
};

class Problem{
    public:
        Customer depot;
        vector<Customer> customers;
        vector<Customer> unplannedCustomers;
        double routeDurationConstraint;
        int planningHorizon;

        Problem(const Problem& rhs);
        Problem();
        void resetCustomers(vector<Customer> newCustomers, int depot);

        vector<Customer> getCustomers();
        vector<Customer> getUnplannedCustomers();

        void setCustomers(vector<Customer> newCustomers);
        void setUnplannedCustomers(vector<Customer> newCustomers);
};

#endif