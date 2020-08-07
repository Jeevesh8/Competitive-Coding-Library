#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<cmath>

using namespace std ;

/*
All iterators below require that visited of all the nodes in a graph must be same, either true, or false.
*/


//Clean Breadth First Search Iteration On a Graph
template< typename node_t >
class bfs_iter{
    /*
    node_t :- 1.) Should have property 'visited' (bool) initialised with False for all nodes.
              2.) Should have function 'get_neighbors' that returns an iterator(**) to the neighbors(*) of a node. 
                  The iterator should give pointers to neighbors each time 1 is added. And should have begin() and end().
                  
                  (*)In case of directed graph, this is only the set of nodes having an inward edge from the cur node.
                  (**)In particular, iterator to the container having pointer to neighboring nodes.
    
    Usage :- 
        
        bfs_iter bfsi(&a) ;                     // a is start node
        while(bfsi!=nullptr){
            
            //process
            bfsi->info -= 4 ;                   //etc.

            bfsi++ ;                            //ptr to next node 
        }
    */   
    
    node_t* cur ;
    node_t* begin ;
    queue<node_t*> bfs_queue ;
    
    public :
        bool True, False ;

        bfs_iter(node_t* a) : cur(a) {
            True = !(cur->visited) ;
            False = cur->visited ;
            begin = a ;
            bfs_queue.push(cur) ;
            cur->visited = True ;
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
                    if(child->visited==False) { bfs_queue.push( child ) ; child->visited = True ;}                                             //Or use *.?
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



//Specialization for storing parent nodes and edges too
template< typename node_t, typename edge_t >
class bfs_iter{
    /*
    node_t :- 1.) Should have property 'visited' (bool) initialised with False
              2.) Should have function get_edge_iter() that returns an iterator for container having ptrs to edges.
              3.) Might need 'parent'(node_t*) for storing ptr to parent node/edge (See Usage) 

    edge_t :- 1.) Should have s,t

                  (*)In case of directed graph, this is only the set of nodes having an inward edge from the cur node.
                  (**)In particular, iterator to the container having pointer to neighboring nodes.
    
    Usage :- 
        
        bfs_iter bfsi(&a) ;                             // a is start node
        vector<node_t*> path ;
        node_t* target = &node_x ;

        while(bfsi!=nullptr){
            
            //process
            bfsi->info -= 4 ;                           //etc.
            bfsi->parent = bfsi.get_parent_node() ;
            
            if(*bfsi==target)
            {
                node_t* cur = target ;
                while(cur->parent!=nullptr)             //Print the reverse path when reached target node.
                {
                    cout<<cur->info ;
                    cur = cur->parent ;
                }
                print(a->info) ;
            }
            bfsi++ ;                                //ptr to next node 
        }
    */   
    
    node_t* cur ;
    edge_t* cur_edge ;
    node_t* begin ;
    queue<pair<node_t*,edge_t*>> bfs_queue ;
    
    public :
        bool True, False ;

        bfs_iter(node_t* a) : cur(a) {

            True = !(cur->visited) ;
            False = cur->visited ;
            begin = a ;
            bfs_queue.push(make_pair(cur, nullptr)) ;
            cur->visited = True ;
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
                cur = bfs_queue.front().first ;
                cur_edge = bfs_queue.front().second ;

                node_t* child ;
                edge_t* edge_to_child ;
                    
                auto& j = cur->get_edge_iter() ;
                    
                for(auto it = j.begin(); it!=j.end(); it++)
                {
                    edge_to_child = *it ;
                    child = edge_to_child->t ;
                    if(child->visited==False) { bfs_queue.push( make_pair(child, edge_to_child) ; child->visited = True ;}                                             //Or use *.?
                }
                    
                bfs_queue.pop() ;
                return cur ;

            }

            else{
                return nullptr;       
            }
        }

        node_t* get_parent_node() { return cur_edge->s ; }
        edge_t* get_cur_edge()  {return cur_edge ; }

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
        bool True, False ;

        dfs_iter(node_t* a) : cur(a) {    
            True = !(cur->visited) ;
            False = cur->visited ;
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
                cur->visited = True ;
                
                auto& j = cur->get_neighbors() ;
                for(auto it=j.begin(); it!=j.end(); it++){
                    child = (*it) ;
                    if(child->visited==False) dfs_stack.push( child ) ; 
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




//Specialization for storing parent nodes and edges too
template< typename node_t, typename edge_t >
class dfs_iter{
    
    /*
    node_t :- 1.) Should have property 'visited' (bool) initialised with False
              2.) Should have function get_edge_iter() that returns an iterator for container having ptrs to edges.
              3.) Might need 'parent'(node_t*) (initialized with nullptr) for storing ptr to parent node/edge (See Usage) 

    edge_t :- 1.) Should have s,t

                  (*)In case of directed graph, this is only the set of nodes having an inward edge from the cur node.
                  (**)In particular, iterator to the container having pointer to neighboring nodes.
    
    Usage :- 
        
        dfs_iter dfsi(&a) ;                             // a is start node
        vector<node_t*> path ;
        node_t* target = &node_x ;

        while(dfsi!=nullptr){
            
            //process
            dfsi->info -= 4 ;                           //etc.
            dfsi->parent = dfsi.get_parent_node() ;
            
            if(*dfsi==target)
            {
                node_t* cur = target ;
                while(cur->parent!=nullptr)             //Print the reverse path when reached target node.
                {
                    cout<<cur->info ;
                    cur = cur->parent ;
                }
                print(a->info) ;
            }
            dfsi++ ;                                //ptr to next node 
        }
    */   

    node_t* begin ;  
    node_t* cur ;
    edge_t* cur_edge ;
    stack<pair<node_t*, edge_t*>> dfs_stack ;
   
    public :
        bool True, False ;
        
        dfs_iter(node_t* a) : cur(a) {    
            True = !(cur->visited) ;
            False = cur->visited ;
            begin = a ;
            dfs_stack.push(make_pair(cur, nullptr)) ;
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
            edge_t* edge_to_child ;

            if(!dfs_stack.empty()){
                
                cur = dfs_stack.top().first ;
                cur_edge = dfs_stack.top().second ;

                dfs_stack.pop() ;
                cur->visited = True ;
                
                auto& j = cur->get_edge_iter() ;
                for(auto it=j.begin(); it!=j.end(); it++){
                    edge_to_child = (*it) ;
                    child = edge_to_child->t ;
                    if(child->visited==False) dfs_stack.push( make_pair(child, edge_to_child) ) ; 
                }
                return cur ;
            }

            else{
                return nullptr ;       
            }

        }
        node_t* end()   {return nullptr ;}
        node_t* begin() {return begin ; }

        node_t* get_parent_node() { return cur_edge->s ; }
        edge_t* get_cur_edge()  {return cur_edge ; }
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
        edge_iterator<bfs_iter<node_t>, vector<edge_t*>, edge_t> ei(bfsi) ;
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


