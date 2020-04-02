#include<iostream>
#include<math.h>
#include<utility>

using namespace std;

template<typename T>
T gcd(T a, T b)
{
    return (b==0) ? a : gcd(b,a%b) ;
}

template<typename T>
T lcm(T a, T b)
{
    return (a*b)/gcd(a,b) ;
}

template<typename T>
pair<T,T> extended_euclid(pair<T,T> a)
{   
    /*
    For finding an integer solution of Ax+By = 1; A,B are integers; GCD(A,B) = 1 .
    a :- pair (A,B)
    returns :- solution pair(x,y)
    Note :- Works for negative numbers too, as x = x%y + (y*floor(x/y)) holds for both +ve and -ve nums in C++
    */

    if(a.second==0)
    {
        return make_pair(1,0);                                                          //make_pair(1,1), make_pair(1,2).. will all give different valid solutions
    }
    
    pair<T,T> prev_sol = extended_euclid( make_pair(a.second, a.first%a.second) );
    
    T temp = prev_sol.first;
    prev_sol.first = prev_sol.second;
    prev_sol.second = temp - (a.first/a.second)*prev_sol.second;
    
    return prev_sol;

}

template<typename T>
T modulo_inverse(T a, T m)
{
    /*
    Finds modulo inverse of a w.r.t. m.
    GCD(a,m) must be 1.
    */
    return extended_euclid( make_pair(a, -m) ).first;
}




