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
        vector<edge*> lvl_graph_edges ;
        edge* next_edge_ptr ;
        // Edit/add more data

        node(int i=p_infty(int), bool vis=false, vector<int> neighbor={}, bool dir=false, int weight=1) : 
            info(i), visited(vis), directed(dir)
        {
            for(size_t j=0; j<neighbor.size(); ++j)
            {
                neighbors.push_back( new node(neighbors[j]) ) ;
                edges.push_back( new edge( weight, directed, this, neighbors[j] ) ) ;
                if(!directed)
                {   
                    neighbors[j]->neighbors.push_back(this) ;
                    neighbors[j]->edges.push_back( new edge( weight, directed, neighbors[j], this ) ) ; 
                }
            }
        }

        edge* get_next_lvl_graph_edge()
        {
            edge* cur = next_edge_ptr ;
            if(cur==lvl_graph_edges.end())
                return nullptr ;
            next_edge_ptr++ ;
            return cur ;
        }

        vector<edge*>& get_edge_iter()
        { 
            vector<edge*> non_zero_edges ;
            for(auto& x : edges)
            {
                if(x->rem_capacity>0)   non_zero_edges.push_back(x) ;
            }
            return non_zero_edges ;
        }
        vector<node*>& get_neighbors() { return neighbors ; }
};

class edge{
    
    public :
        int weight ;
        bool directed ;
        node* s, t ;
        bool visited ;
        int rem_capacity, flow ; 
        edge_t* rev_edge_ptr ;

        // Edit/add more data
        edge(int i=1, bool dir=false, node s=node(), node t=node(), int flow=0, int rem_capacity=p_infty(int)) : 
            weight(i), directed(dir), s(s), t(t), flow(flow), rem_capacity(rem_capacity)
        {  
            visited = false ;
            rev_edge_ptr = nullptr ; //Should set if adding reverse edges while taking input .
        }

        void add_reverse_edge()
        {
            rev_edge_ptr = new edge(1, true, t, s, 0, p_infty(int)) ;
            rev_edge_ptr->rev_edge_ptr = this ;
            rev_edge_ptr->s->edges.push_back(rev_edge_ptr) ;    
        }

};

