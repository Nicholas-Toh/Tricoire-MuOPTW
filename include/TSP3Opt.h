#ifndef TSP3OPT_H
#define TSP3OPT_H

#include "Problem.h"
#include "Solution.h"
#include "Solomon.h"
#include <vector>
#include <tuple>

enum OptCase {OPT_CASE_8 = 8};

vector<std::tuple<int, int, int>> possibleSegments(int n);
double getSolutionCostChange(vector<Customer>& route, OptCase optcase, int i, int j, int k);
vector<Customer> reverseSegments(vector<Customer>& route, OptCase optcase, int i, int j, int k);
void cycleToDepot(int depot, vector<Customer>& route);
void tsp3Opt(int depot, vector<Customer>& route);

#endif