#include<iostream>
#include<set>
#include<vector>

using namespace std;

template<typename T>
T calculateMex(set<T> s)
{
    T mex = 0;
    while(s.find(mex) != set.end())
        mex++;
    return mex;
}

template<typename T>
T xor_sum(vector<T> s)
{
    T ans=s[0];
    for(size_t i=1;i<s.size();++i)
    {
        ans=ans^s[i];
    }
    return ans;
}