#ifndef ROUTE_UTIL_H
#define ROUTE_UTIL_H

#include "Problem.h"
#include "Solution.h"
#include <vector>

double calculate_infeasibility(vector<Customer>& route);
void printCustomers(Route route);
void printCustomers(vector<Customer> route);
void printSolution(Solution solution);
#endif