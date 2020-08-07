#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<cmath>
#include<limits>

using namespace std ;

#define p_infty(t) +numeric_limits<t>::infinity()

//Clean Dijkstra's algorithm
template<typename node_t, typename edge_t, typename dist_t>
dist_t dijkstra( node_t* target=nullptr, node_t* source ){
    /*
    Dijkstra can't be applied for graphs with negative edges because it assumes that distance of closest node in priority
    queue can't change. 
    
    node_t :- 1.) Should have property 'dist_info' of type dist_t . Initialized to value numeric_limits<dist_t>::max() .
              2.) Should have comparison operator (>) defined . That takes in pointer to 2 nodes.
              3.) Should have a function 'get_edge_iter' that returns an iterator for a container having(pointers to) 
                  outward edges(from the current node) of type edge_t .  
    
    edge_t :- 1.) Should have source(pointer to node_t), target(pointer to node_t), directed(bool), weight(dist_t) .
    */

    priority_queue<node_t*,vector<node_t*>,greater<node_t*>> pq ;
    source->dist_info = 0 ;
    pq.push(source) ;
    node_t* ptr ;
    
    while(!pq.empty()){
        
        ptr = pq.top() ;
        if(ptr==target){
            return ptr->dist_info ;
        }

        pq.pop() ;
        
        node_t* adj ;
        for( auto& x : ptr->get_edge_iter() )
        {
            if( x->target == ptr && !x->directed )     adj = x->source ;
            else                                       adj = x->target ;
            adj->dist_info = min( x->weight + ptr->dist_info, adj->dist_info ) ;
            pq.push(adj);
        }
    }
    return 0 ;
}


//Clean Bellman-Ford Algorithm on Graphs
template<typename node_t, typename edge_t, typename EdgeIter>
void bellman_ford( node_t* source, size_t N ){
    
    /*
    Calculates distances of all nodes from source, even when graph contains negative edges.
    It doesn't work in case there exists cycle(s) with net negative weight reachable from source. 
    As distances to nodes in cycle can be reduced infinitely.
    You can update the edges in different order in each phase.

    N:- number of nodes
    node_t :- 1.) Should have property 'dist_info' of type dist_t . Initialized to value numeric_limits<dist_t>::max() .  
              2.) Each undirected edge must correspond to 2 directed edges s.t. it is relaxed two times, with source and target flipped.
    
    edge_t :- 1.) Should have source(pointer to node_t), target(pointer to node_t), directed(bool), weight(dist_t) .
              2.) Needs to have 'visited'(bool) property.

    EdgeIter :- Type of Iterator over outward edges from a single node. [some container of edge_t*].
                In case of undirected edges make sure edges are added both ways in graph.
    */

    source->dist_info = 0 ;
    
    //Making a list of all edges
    vector<edge_t*> edge_list ;
    bfs_iter<node_t> bfsi(source) ;
    edge_iterator<bfs_iter<node_t>, EdgeIter, edge_t> ei(bfs_iter) ;
    while( ei!=ei.end() )
    {
        edge_list.push_back(*ei) ;
        ei++ ;
    }
    
    //Relaxing edges during N-1 phases
    for(size_t i=0; i<N-1; ++i)
    {
        for(auto it = edge_list.begin(); it!=edge_list.end(); ++it)
        {    
            if( isinf(it->target->dist_info) )
                it->target->dist_info = it->source->dist_info ;
            else
                it->target->dist_info = min( it->source->dist_info , it->target->dist_info + it->weight ) ;    
        }            
    }
}


//All Pair Shortest path (Floyd-Warshall Algo)
template<typename dist_t>
vector<vector<dist_t>>& floyd_warshall(vector<vector<dist_t>>& adj_mat)
{
    /*
    Much Better than running Bellman-Ford N times, if graph is not so sparse(E^2>V). O(V-cube). Running bellman ford V times = O((VE)^2)
    If adjacency matrix not available , use get_adjacency_matrix() of graph_utils.cpp
    */
    for(size_t k=0; k<adj_mat[0].size(); ++k)
        for(size_t i=0; i<adj_mat[0].size(); ++i)
            for(size_t j=0; j<adj_mat[0].size(); ++j)
                if(! (isinf(adj_mat[i][k])||isinf(adj_mat[j][k])) )
                    adj_mat[i][j] = min(adj_mat[i][j], adj_mat[i][k]+adj_mat[k][j]) ;
    return adj_mat ;
}
