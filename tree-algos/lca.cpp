#include<iostream>
#include<vector>
#include<math.h>
using namespace std;

vector<vector<size_t>> precompute(size_t log_h, vector<size_t> parents)
{
    /*
    Time complexity = nlogn
    log_h :- g.i.f.[log (base 2) of max. possible height of tree]
    parents :-  all node's parents. parents[i] = index (of parent of node i) in the vector parents. parents[root_node] must be -1 
    Returns P;  P[i][j] :- (2^j)th parent of i^th node
    */
    size_t n = parents.size();
    vector<vector<size_t>> P(n, vector<size_t>(log_h, -1));
    
    for(size_t i=0; i<n;++i)    P[i][0] = parents[i];
    
    for(size_t j=1;j<log_h;++j)
    {
        for(size_t i=0;i<n;++i)
        {
            P[i][j] = P[P[i][j-1]][j-1];
        }
    }
}


size_t lca(size_t i, size_t j, vector<vector<size_t>> P, size_t level_i, size_t level_j)
{
    if(level_i>level_j)
    {
        swap(i,j);
        swap(level_i, level_j);
    }    
    
    size_t dist = level_j-level_i;
    size_t log_dist;
    
    while(dist>0)
    {
        log_dist = size_t(floor(log2(dist)));
        j = P[j][log_dist];
        dist-= (1<<log_dist);
    }

    for(size_t m=P[0].size(); m>0; --m)
    {
        if( (P[i][m]!=-1) && P[i][m]!=P[j][m])
        {
            i=P[i][m];
            j=P[j][m];
        }
    }
    return P[i][0];
}