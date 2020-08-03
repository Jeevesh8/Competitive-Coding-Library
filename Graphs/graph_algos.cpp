#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<cmath>

using namespace std ;

//Clean Breadth First Search Iteration On a Graph
template< typename node_t >
class bfs_iter{
    /*
    node_t :- 1.) Should have property 'visited' (bool) initialised with False
              2.) Should have function 'get_neighbors' that returns an iterator(**) to the neighbors(*) of a node. 
                  The iterator should give pointers to neighbors each time 1 is added. And should have begin() and end().
                  
                  (*)In case of directed graph, this is only the set of nodes having an inward edge from the cur node.
                  (**)In particular, iterator to the container having pointer to neighboring nodes.
    
    Usage :- 
        
        bfs_iter bfsi(&a) ;                     // a is start node
        while(bfsi!=nullptr){
            
            //process
            bfs->info -= 4 ;                    //etc.

            bfsi++ ;                            //ptr to next node 
        }
    */   
    
    node_t* cur ;
    node_t* begin ;
    queue<node_t*> bfs_queue ;

    public :

        bfs_iter(node_t* a) : cur(a) {
            begin = a ;
            bfs_queue.push(cur) ;
            cur->visited = true ;
            cur = this->operator++() ;
            
        }
            
        node_t& operator*(){
            return *cur;
        }
            
        node_t& operator ->(){
            return *cur;
        }
            
        node_t* operator ++(){

            if(!bfs_queue.empty())
            {
                cur = bfs_queue.front() ;
                    
                node_t* child ;
                    
                auto& j = cur->get_neighbors() ;
                    
                for(auto it = j.begin(); it!=j.end(); it++)
                {
                    child = *it ;
                    if(!child->visited) { bfs_queue.push( child ) ; child->visited = true ;}                                             //Or use *.?
                }
                    
                bfs_queue.pop() ;
                return cur ;

            }

            else{
                return nullptr;       
            }
        }

        node_t* end()   {return nullptr ;}
        node_t* begin() {return begin ; }
};


//Clean Depth First Search Iteration on a graph.
template< typename node_t >
class dfs_iter{
    
    /*
    node_t :- 1.) Should have property 'visited' (bool) initialised with False
              2.) Should have function 'get_neighbors' that returns an iterator(**) to the neighbors(*) of a node. 
                  The iterator should give pointers to neighbors each time 1 is added. And should have begin() and end().
                  
                  (*)In case of directed graph, this is only the set of nodes having an inward edge from the cur node.
                  (**)In particular, iterator to the container having pointer to neighboring nodes.
    
    Usage :- 
        
        dfs_iter dfsi(&a) ;                   // a is start node
        while(dfsi!=nullptr){                   
            //process
            dfsi->info = 3 ;                  //etc.

            dfsi++ ;                          //ptr to next node    
        }
    */   

    node_t* begin ;    
    node_t* cur ;
    stack<node_t*> dfs_stack ;

    public :
        
        dfs_iter(node_t* a) : cur(a) {    
            begin = a ;
            dfs_stack.push(cur) ;
            cur = this->operator++() ;
        }
            
        node_t& operator*(){
            return *cur ;
        }
        
        node_t& operator ->(){
            return *cur ;
        }
        
        node_t* operator ++(){
            
            node_t* child ;
            
            if(!dfs_stack.empty()){
                
                cur = dfs_stack.top() ;
                dfs_stack.pop() ;
                cur->visited = true ;
                
                auto& j = cur->get_neighbors() ;
                for(auto it=j.begin(); it!=j.end(); it++){
                    child = (*it) ;
                    if(child->visited==false) dfs_stack.push( child ) ; 
                }
                return cur ;
            }

            else{
                return nullptr ;       
            }

        }
        node_t* end()   {return nullptr ;}
        node_t* begin() {return begin ; }
};


//Clean Edge Iterator
template<typename NodeIter, typename EdgeIter, typename edge_t>
class edge_iterator{
     /*
    NodeIter :- Any iterator over nodes, should have ++ , *, .end()
    EdgeIter :- Iterator over outward edges from a node.[some container of edge_t*].
                In case of undirected edges make sure edges are added both ways in graph.
    edge_t :- type of edge.

    Usage :- 
        
        vector<edge_t*> edge_list ;
        bfs_iter<node_t> bfsi(source) ;
        edge_iterator<bfs_iter<node_t>, vector<edge_t*>, edge_t> ei(bfs_iter) ;
        while( ei!=ei.end() )
        {
            edge_list.push_back(*ei) ;
            ei++ ;
        }
    */   
    NodeIter node_iter ;
    edge_t* cur_edge, begin ; 
    EdgeIter per_node_edge_iter ;
    
    public :
        
        edge_iterator(NodeIter& ni) : node_iter(ni) {
            per_node_edge_iter =  node_iter->get_edge_iter() ;
            cur_edge = *(per_node_edge_iter) ;
            begin = cur_edge ;
        }

        edge_t* operator ++()
        {
            if( per_node_edge_iter==node_iter->get_edge_iter().end() )
            {
                node_iter++;
                if( node_iter == node_iter.end() ) { return nullptr ; }
                per_node_edge_iter = node_iter->get_edge_iter() ;
                cur_edge = *(per_node_edge_iter) ;
                return cur_edge ;
            }
            per_node_edge_iter++ ;
            cur_edge = *(per_node_edge_iter) ;
            return cur_edge ;
        }

        edge_t* operator *()
        {
            return *cur_edge ;
        }

        edge_t* operator ->()
        {
            return *cur_edge ;
        }

        edge_t* end()   {return nullptr ;}
        edge_t* begin() {return begin ; }
};


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

