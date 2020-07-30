#include<iostream>
#include<vector>
#include<cmath>
#include<limits>
using namespace std;

#define n_infty(t) -numeric_limits<t>::infinity()
#define p_infty(t) +numeric_limits<t>::infinity()


// Rod Cutting Problem
template<typename priceType, typename lengthType>
priceType max_rod_profit(vector<lengthType>& lengths, vector<priceType>& prices, lengthType rod_length, size_t begin=0)
{
    static vector<vector<priceType>> dp(rod_length, vector<priceType> (lengths.size(), n_infty(priceType))); 
    
    /*
    lengths :- vector of possible lengths that can be cut (in descending order) 
    prices :- vector of corresponding prices 
    rod_length and lengths should be whole numbers
    begin :- You can use rod lengths[begin:]
    dp[i][j] = max. profit from splitting i-length rod given you could use rod lengths[j:]
    */
    
    if(begin==prices.size() or rod_length<=0)   return 0;

    priceType x;
    if(isinf(dp[rod_length][begin+1])) 
        x = max_rod_profit(lengths, prices, rod_length, begin+1);
    else
        x = dp[rod_length][begin+1];
    
    priceType y=0;
    if(rod_length>lengths[begin])
    {
        lengthType new_rod_length = rod_length-lengths[begin];
        
        if(isinf(dp[new_rod_length][begin+1]))
            y = max_rod_profit(lengths, prices, new_rod_length, begin) + prices[begin];
        else
            y = dp[new_rod_length][begin];
    }
    
    dp[rod_length][begin] = max(x,y);    
    return dp[rod_length][begin] ;
}


//Matrix Chain Multiplication
template<typename costType, typename elemType, typename T = pair<costType, elemType> >
pair<costType, elemType> mat_chain_mul( vector<elemType>& chain, 
                                        T (*cost_n_merge_func)(elemType, elemType), 
                                        size_t begin=0, size_t end=-1)
{
    /*
    calculates and returns min. cost(and final shape) for merging all elements (as specified by cost_n_merge_func)
    in chain[begin:end] (both inclusive) where cost of each merge is specified by cost_func .
    
    Mergiing should be associative.
    */
    
    if(end==-1)   end = chain.size() - 1 ;

    if(begin==end) return make_pair(0, chain[begin]) ;
    
    static vector<vector<T>> dp(chain.size(), vector<T> (chain.size(), make_pair(p_infty(costType), chain[0])));
    
    if(!isinf(dp[begin][end].first))    return dp[begin][end] ;

    T left, right;                                           //To store results of merging all elements left/right of current position
    T min_cost;                                              //To store (min_cost,output_shape of min_cost merging)
    T left_first1, left_first2, right_first1, right_first2 ; //To store (cost, shape) when doing (AB)C and A(BC) respectively
    
    right = mat_chain_mul(chain, cost_n_merge_func, begin+1, end);
    min_cost = cost_n_merge_func(chain[begin], right.second);
    
    for(size_t i=begin+1; i<end-1; ++i)
    {
        
        left = mat_chain_mul(chain, cost_n_merge_func, begin, i-1);
        right = mat_chain_mul(chain, cost_n_merge_func, i+1, end);
        
        //Way 1
        left_first1 = cost_n_merge_func( left.second, chain[i] );
        left_first2 = cost_n_merge_func(left_first1.second, right.second ) ;
        
        if(left_first1.first + left_first2.first <min_cost.first)
        {
            min_cost.first = left_first1.first + left_first2.first ;
            min_cost.second = left_first2.second ;
        }
        
        //Way 2
        right_first1 = cost_n_merge_func( chain[i], right.second );
        right_first2 = cost_n_merge_func( left.second, right_first1.second ) ;
        
        if(right_first1.first + right_first2.first <min_cost.first)
        {
            min_cost.first = right_first1.first + right_first2.first ;
            min_cost.second = right_first2.second ;
        }
    }

    left = mat_chain_mul(chain, cost_n_merge_func, begin, end-1) ;
    left_first1 = cost_n_merge_func(left, chain[end]) ;
    if( left_first1.first < min_cost.first )
    {
        min_cost.first = left_first1.first ;
        min_cost.second = left_first1.second ; 
    }
    
    dp[begin][end] = min_cost ;
    return dp[begin][end] ;    
}


