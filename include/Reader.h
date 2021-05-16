#ifndef READER_H
#define READER_H

#include "Problem.h"
#include "Solution.h"
#include <fstream>

const string DATASET_PATH = ".\\Datasets\\Zhang\\";
const string SOLUTION_PATH = ".\\Solutions\\";

int readCustomers(string filename, vector<Customer>& customers);
int readDistMatrix(string filename, double**& matrix);
fstream writeResults(string filename);
#endif