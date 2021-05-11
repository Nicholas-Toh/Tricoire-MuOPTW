#include "..\include\Utils.h"
#include <iostream>
#include <random> 
#include <chrono>
#include <algorithm>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);

int randint(int x, int y) 
{
    std::uniform_int_distribution<> distr(x, y);
    return distr(generator);
}


/*
int main()
{
    std::vector<int> a {1, 2, 3, 4};
    std::vector<int> b {5, 6, 7, 8};
    sum(a);
    //auto c = slice(a, 1, 4) + slice(b, 2, 4);
    auto start = std::chrono::high_resolution_clock::now(); 
    swap_seq(a, 0, 1, b, 0, 1);
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto execTime = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start); 
    std::cout << "Execution time: " << execTime.count() << std::endl;
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
    for (int i = 0; i < b.size(); ++i) std::cout << b[i] << " ";

    for (int i = 0; i < 100000; i++)
    {
        std::cout << randint(1, 10) << " ";
    }
}
*/

