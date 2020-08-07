#include<iostream>
#include<vector>
#include<limits>

using namespace std;

#define p_infty(t) +numeric_limits<t>::infinity()

class edge;

class node{

    public :
        int dist_info ;
        bool visited ;
        vector<node*> neighbors ;
        vector<edge*> edges ;
        bool directed ;
        // Edit/add more data

        node(int i=p_infty(int), bool vis=false, vector<int> neighbor={}, bool dir=false, int weight=1) : info(i), visited(vis), directed(dir)
        {
            for(size_t j=0; j<neighbor.size(); ++j)
            {
                neighbors.push_back( &node(neighbor[j]) ) ;
                edges.push_back( new edge( weight, directed, this, neighbors[j] ) ) ;
                if(!directed)   
                    neighbors[j]->edges.push_back( new edge( weight, directed, neighbors[j], this ) ) ; 
            }
        }
        vector<edge*>& get_edge_iter() { return edges ; }            
};

class edge{
    
    public :
        int weight ;
        bool directed ;
        node* s, t ;
        bool visited ;

        // Edit/add more data
        edge(int i=1, bool dir=false, node s=node(), node t=node()) : weight(i), directed(dir), s(s), t(t)
        { visited = false; }

};