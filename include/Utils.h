#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdlib> 
#include <iostream>
#include <random> 
#include <chrono>
#include <algorithm>

inline unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
inline std::mt19937 generator(seed);

inline int randint(int x, int y) 
{
    std::uniform_int_distribution<> distr(x, y);
    return distr(generator);
}

template<typename T>
inline std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs) 
{
    std::vector<T> copy(lhs);
    copy.insert(copy.end(), rhs.begin(), rhs.end());
    return copy;
}

template<typename T>
inline std::vector<T> slice(const std::vector<T>& v, int start, int end) //[start, end)
{
    std::vector<T> copy(v.begin()+start, v.begin()+end);
    return copy;
}

template<typename T>
inline std::vector<T> slice(const std::vector<T>&& v, int start, int end) //[start, end)
{
    std::vector<T> copy(v.begin()+start, v.begin()+end);
    return copy;
}

template<typename T>
inline void swap_seq(std::vector<T>& v1, int s1, int l1, std::vector<T>& v2,  int s2, int l2) 
{
    std::vector<T> nv1(v1.begin(),v1.begin()+s1);
    std::vector<T> nv2(v2.begin(),v2.begin()+s2);
    nv1.insert(nv1.end(), v2.begin()+s2, v2.begin()+l2);
    nv2.insert(nv2.end(), v1.begin()+s1, v1.begin()+l1);
    nv1.insert(nv1.end(), v1.begin()+l1, v1.end());
    nv2.insert(nv2.end(), v2.begin()+l2, v2.end());
    v1.clear();
    v2.clear();
    v1=nv1;
    v2=nv2;
}

template<typename T>
inline T sum(std::vector<T> v)
{
    T sum = 0;
    for (auto& n: v) sum += n;
    return sum;
}

#endif