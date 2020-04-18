#include<iostream>
#include<complex>
#include<vector>
#include<math.h>
#define PI 3.14159265
using namespace std;

vector<complex<double>> fft(vector<complex<double>> X)
{
    size_t N = X.size();
    
    if(N<=1)
        return X;
    
    vector<complex<double>> X_odd, X_even;

    for(size_t i=0;i<N/2;++i)
    {
        X_odd.push_back(X[2*i+1]);
        X_even.push_back(X[2*i]);
    }
    
    double theta = -2*PI/N;
    complex<double> e_term(1), expo_term(cos(theta), sin(theta));
    
    vector<complex<double>> even_res = fft(X_even);
    vector<complex<double>> odd_res = fft(X_odd);
    
    for(size_t j=0;j<even_res.size();++j)
    {
        X[j] = even_res[j]+e_term*odd_res[j];
        X[j+N/2] = even_res[j]-e_term*odd_res[j];
        e_term*=expo_term;
    }
    return X;            
}