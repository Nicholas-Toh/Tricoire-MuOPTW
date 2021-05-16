#ifndef VNS_H
#define VNS_H
#include "..\include\TSP3Opt.h"
#include "..\include\Solomon.h"
#include "..\include\Problem.h"
#include "..\include\Solution.h"

const int CROSS_EXCHANGE_START = 1;
const int OPT_EXCHANGE_1_START = 9;
const int OPT_EXCHANGE_2_START = 13;
const int END = 18;

Solution variableNeighborhoodSearch(Solution bestSolution, Problem& problem, int maxLastAcceptedDuration=16000, int maxDuration=20000, int timeLimit = 120);
std::pair<int, int> crossExchange(vector<Route>& routes, int neighborhood);
int optExchange1(vector<Route>& routes, vector<Customer>&customers, int neighborhood);
int optExchange2(vector<Route>& routes, vector<Customer>&customers, int neighborhood);
bool acceptanceDecision(Solution& bestSolution, Solution& candidateSolution, int neighborhood, string mode, bool ascending=false);

#endif 