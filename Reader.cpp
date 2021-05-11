#include "..\include\Reader.h"
#include <fstream>
#include <iostream>
#include <vector>

int initFile (string filename) //Creates file if none exists
{ 
	fstream f;
    f.open(filename);
    if (!f.is_open()) 
	{
        f.open(filename, std::ios::out|std::ios::in|std::ios::trunc);
		if (!f.is_open())
		{
			cout << "Error creating file " << filename << "!" << endl;
			f.close(); 
			return 1;
		}
	}
	return 0;
}

int readCustomers(string filename, vector<Customer>& customers)
{
    string full_path = DATASET_PATH + filename +".txt";
    fstream f;
    f.open(full_path);
    if (!f.is_open())
	{
		std::cout << "Error reading file " << full_path << "!" << endl;
		f.close(); 
		return 1;
	}
	else 
	{
        string temp;
        for (int i = 0; i < 6; i++)
            getline(f, temp);

        while(f.peek() != EOF)
        {
            Info info;
            f >> info.id;
            f >> info.x;
            f >> info.y;
            f >> info.value;
            f >> info.startTime;
            f >> info.endTime;
            f >> info.serviceTime;
            if (info.id != 0)
                info.endTime += info.serviceTime;

            Customer customer = Customer(info);
            customers.push_back(customer);
        }   
        customers.erase(customers.end());
		f.close(); 	
	}
	return 0;
}

int readDistMatrix(string filename, double**& matrix)
{
    string full_path = DATASET_PATH + filename + "_distmatrix_cpp.txt";
    fstream f;
    f.open(full_path);
    if (!f.is_open())
	{
		std::cout << "Error reading file " << full_path << "!" << endl;
		f.close(); 
		return 1;
	}
	else 
	{
        int size;
        f >> size;

        matrix = new double*[size];
        for (int i = 0; i < size; i++)
        {
            matrix[i] = new double[size];
            for (int j = 0; j < size; j++)
            {
                f >> matrix[i][j];
            }
        }
    }
    return 0;
}

fstream writeResults(string filename)
{
    string full_path = SOLUTION_PATH + filename + "_results.txt";
    if (initFile(full_path))
    {
        throw(full_path);
    }

    fstream f;
    f.open(full_path, std::ios::in|std::ios::app);
    if (!f.is_open())
	{
		std::cout << "Error reading file " << full_path << "!" << endl;
		f.close(); 
		throw(full_path);
	}
	else 
    {
        return f;
    }
}