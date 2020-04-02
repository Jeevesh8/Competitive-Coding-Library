#include<iostream>
#include<math.h>                                                                    //For sqrt
#include<utility>                                                                   //For pair
#include<vector>
#include<algorithm>                                                                 //For std::fill

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

template<typename T>
bool isPrime(T n)
{   
    /*
    Taken from :- https://stackoverflow.com/questions/48911748/finding-whether-a-number-is-prime-or-not-c
    with little modification.
    */
    if(n<=1) return false;
    if(n<=3) return true;

    if(n%2==0||n%3==0) return false;
    
    T max_factor_val = floor(sqrt(n));
    for(T i=5; i<max_factor_val; i+=6)
    {
        if(n%i==0||n%(i+2)==0)
            return false;
    }
    return true;
}


template<typename T>
vector<bool> sieve(T n)
{
    /*
    Returns vector<bool>, where 1 => prime and 0=>not prime
    i-th entry of vector tells if i is prime. len(vector) = n+1
    */
    vector<bool> isPrime(n+1);
    fill(isPrime.begin(), isPrime.end(), 1);
    
    isPrime[0]=0; isPrime[1]=0;
    for(size_t i = 4; i<=n; i+=2) isPrime[i]=0;
    for(size_t i = 9; i<=n; i+=6) isPrime[i]=0;

    for(size_t pick=5; pick<=n; pick+=4)
    {
        if(isPrime[pick])
            for(size_t num = pick*pick; num<=n; num+=2*pick)   isPrime[num]=0;                    //Cut nums using pick            
        pick+=2;
        if(isPrime[pick]&&pick<=n)
            for(size_t num = pick*pick; num<=n; num+=2*pick)   isPrime[num]=0;                    //Cut nums using pick+2    
    }
    return isPrime;
}

template<typename T>
vector<bool> segmented_sieve(T a, T b)
{
    /*
    returns a vector similar to simple sieve for the range [a,b]
    len(vector) = b-a+1. a and b both included.
    Uses the fact that any composite no. in [a,b] will have at least one factor in [2,sqrt(b)]
    */
    vector<bool> isPrime(b-a+1);
    fill(isPrime.begin(), isPrime.end(), 1);
    vector<bool> checker_sieve = sieve(floor(sqrt(b)));

    for(size_t i=a%2; i<=b; i+=2)    isPrime[i]=0;
    
    if(a%3==0)                       isPrime[0]=0;
    
    size_t m = 3-(a%3);
    if ( (a+m)%2==0 )                for(size_t i=m+3; i<=b; i+=6)    isPrime[i]=0;      
    else                             for(size_t i=m; i<=b; i+=6)      isPrime[i]=0;

    for(size_t pick = 5; pick<checker_sieve.size(); pick+=4)
    {
        if(isPrime[pick])
        {
            T start_point = pick*pick;
            start_point += ( floor((a-start_point)/2*pick) + 1)*2*pick;
            for(size_t num = start_point; num<=b; num+=2*pick)   isPrime[num-a]=0;
        }
        pick+=2;
        if(isPrime[pick]&&pick<checker_sieve.size())
        {
            T start_point = pick*pick;
            start_point += ( floor((a-start_point)/2*pick) + 1)*2*pick;
            for(size_t num = start_point; num<=b; num+=2*pick)   isPrime[num-a]=0;
        }
    }
    return isPrime;
}

