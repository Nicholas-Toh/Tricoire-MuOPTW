#include "../include/Problem.h"
#include "../include/Solution.h"
#include "../include/Reader.h"
#include "../include/Solomon.h"
#include "../include/RouteUtils.h"
#include "../include/Utils.h"
#include "../include/VNS.h"
#include <iostream>
#include <chrono> 
#include <string>

double ** matrix;

int main()
{
    for (int i = 1; i <= 1; i++ )
    {
        int j_max;
        if (i == 1)
        {
            j_max = 9;
        }
        else
        {
            j_max = 8;
        }

        j_max = 1;

        for (int j = 1; j <= j_max; j++)
        {
            //string FILENAME= "c" + to_string(i) + "0" + to_string(j) + "_100";
            string FILENAME= "c204_100";
            

            readDistMatrix(FILENAME, matrix);
            vector<Customer> customers = vector<Customer>();
            readCustomers(FILENAME, customers);

            Customer::setTimeMatrix(matrix);

            Problem optionalProblem = Problem();    
            for(auto& cust: customers) cust.info.mandatory = false;
            customers[0].info.mandatory = true;
            optionalProblem.resetCustomers(customers, customers[0].info.id);
            optionalProblem.planningHorizon = 5; //Monday to friday

            SolomonConstructive constructive = SolomonConstructive();
            auto start = chrono::high_resolution_clock::now(); 
            Solution solution = constructive.solveOptional(optionalProblem);
            auto stop = chrono::high_resolution_clock::now(); 
            auto execTimeInitial = chrono::duration_cast<chrono::milliseconds>(stop - start); 
       
            start = chrono::high_resolution_clock::now(); 
            Solution bestSolution = variableNeighborhoodSearch(solution, optionalProblem, 3500, 800000, 120);
            stop = chrono::high_resolution_clock::now(); 
            auto execTimeFinal = chrono::duration_cast<chrono::milliseconds>(stop - start); 

            for (auto& route : solution.routes) 
            {
                route.optimize();
            }

            for (auto& route : bestSolution.routes) 
            {
                route.optimize();
            }
            /*
            cout << FILENAME << endl;
            cout << "Initial solution" << endl;
            printSolution(solution);

            double distance = sum(solution.calculateDistance());
            double time = sum(solution.calculateTime());
            double value = sum(solution.calculateValue());
            int numCustomers = sum(solution.calculateCustomers());
            
            cout << "Solution distance: " << distance << endl;
            cout << "Solution time: " << time << endl;
            cout << "Solution value: " << value << endl;
            cout << "Number of customers: " << numCustomers << endl;

            cout << "Final solution" << endl;
            printSolution(bestSolution);
            distance = sum(bestSolution.calculateDistance());
            time = sum(bestSolution.calculateTime());
            value = sum(bestSolution.calculateValue());
            numCustomers = sum(bestSolution.calculateCustomers());


            cout << "Solution distance: " << distance << endl;
            cout << "Solution time: " << time << endl;
            cout << "Solution value: " << value << endl;
            cout << "Number of customers: " << numCustomers << endl;
            */
            
            //objectives: distance, value
            //Solution bestSolution= Solution(optionalProblem);            

            /*cout << value << ",";
            cout << distance << ",";
            cout << time << ",";
            cout << numCustomers << ",";
            cout << execTimeInitial.count() << endl << endl;*/

            /*
            fstream f = writeResults(FILENAME);
            cout << f.is_open();
            f << "Initial solution" << endl;
            printSolution(solution);
            f << value << ",";
            f << distance << ",";
            f << time << ",";
            f << numCustomers << ",";
            f << execTimeInitial.count() << endl << endl;*/

            
            /*
            cout << value << ",";
            cout << distance << ",";
            cout << time << ",";
            cout << numCustomers << ",";
            cout << execTimeInitial.count() << endl << endl;

            f << "Final solution" << endl;
            f << value << ",";
            f << distance << ",";
            f << time << ",";
            f << numCustomers << ",";
            f << execTimeInitial.count() << endl << endl;
            f.close();
            */
        }
        
    }
    cout << "Finished" << endl;
}