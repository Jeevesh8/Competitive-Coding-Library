#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<cmath>
#include<limits>

using namespace std ;

#define p_infty(t) +numeric_limits<t>::infinity()

//Dinitz's Algorithm for Max-Flow/Min-Cut
/*
It converges to correct solution in at most N-2 steps because :-
1.) No augmenting path in N-2 level graphs => no augmenting path in original graph.
2.) The level of target node always increases, after 1 blocking flow is used. 
3.) And maximum level the target node can reach(starting from 1) before disconnecting from the source is N-1

Using DFS doesn't guarantee convergence in general graph; but it does so in Level Graph, so we can use it here, unlike Edmonds-Karp .
*/
template<typename node_t, typename edge_t>
node_t* make_level_graph(node_t* source)
{
    /*
    Makes lvl_graph_edges so that resultant is a level graph(centered at source) of the original graph.
    
    node_t :- 1.) Must have property 'level'(size_t) and get_edge_iter() should return only non-zero edges, 
                  as only those must be used in BFS for assigning labels.

    Time complexity O(E)
    */

    bfs_iter<node_t, edge_t> bfsi(source) ;
    size_t cur_level = 0 ;
    source->level = cur_level ;
    source->lvl_graph_edges.clear() ; 

    bfsi++ ;
    while(bfsi!=bfsi.end())
    {
        bfsi->level = bfsi->cur_edge->s->level + 1 ;                                //Assigning levels
        bfsi->lvl_graph_edges.clear() ;                                             //Clearing previous level graph, if any
        bfsi++ ;
    }

    bfs_iter<node_t> bfsi_1(source) ;
    edge_iterator<bfs_iter<node_t>, vector<edge_t*>, edge_t> ei(bfsi_1) ;
    
    while( ei!=ei.end() )
    {
        if(ei->s->level+1==ei->t->level)
        {
            ei->s->lvl_graph_edges.push_back(ei) ;
            if(ei->s->lvl_graph_edges.begin()+1==ei->s->lvl_graph_edges.end())      //if first edge was added.
                ei->s->next_edge_ptr = ei->s->lvl_graph_edges.begin() ;
        }
    }
    
    return source ;
}

template<typename node_t, typename edge_t, typename flow_t>
flow_t dfs(node_t* source, node_t* target, flow_t max_flow=p_infty(flow_t))
{
    /*
    Does DFS in level graph till target node is found. During forward pass, max_flow on the path is calculated. 
    During backward pass edges are updated. When no flow (from s to t) can be found, 0 is returned .

    edge_t :-  1.) Must have ptr to the reverse edge (rev_edge_ptr) and add_reverse_edge() function to add one.
               2.) Must have properties 'rem_capacity' denoting remaining capacity for the directional edge and 'flow' denoting the
                   current flow through the edge.
 
    node_t :-  1.) Must have a container having ptrs to edges in level graph('lvl_graph_edges') . This container is to be filled by 
                   'make_level_graph()' function above .
               2.) Must have ptr to next edge 'next_edge_ptr' (with '--' operator) that points to edge being next edge which would be used
                   for source node. This pointer is not to be reset for each dfs pass, but will be reset by 'make_level_graph()'.
    
    Note that, when you call this function again and again, the visited, rem_capacity, flows, next_edge_ptr and new edges added are preserved. 
    */
    if(source==target)   return max_flow ;
    
    if(source->visited)  return 0 ;
    else    source->visited = true ;
    
    edge_t* edge_ptr = source->get_next_lvl_graph_edge() ;
    if(edge_ptr==nullptr)    return 0 ;

    while( edge_ptr!=nullptr )
    {
        flow_t flow = dfs(edge_ptr->t, target, min(max_flow, edge_ptr->rem_capacity)) ; 
        
        if(flow!=0)
        {
            edge_ptr->flow += flow ;
            edge_ptr->rem_capacity -= flow ;
            
            //Add or update reverse edge
            if(edge_ptr->rev_edge_ptr==nullptr)
            {    
                edge_ptr->add_reverse_edge() ;
                edge_ptr->rev_edge_ptr->rem_capacity = flow ;
                edge_ptr->rev_edge_ptr->flow = 0 ;
            }
            else
            {
                edge_ptr->rev_edge_ptr->rem_capacity += flow ;
            }

            //Only move to next edge in those nodes' "lvl_graph_edges" for which rem_capacity==0
            if(edge_ptr->rem_capacity!=0)
                edge_ptr->s->next_edge_ptr-- ;
            
            source->visited = false ;

            return flow ;
        }
        edge_ptr = source->get_next_lvl_graph_edge() ;
    }
    return 0 ;
}


template<typename node_t, typename edge_t, typename flow_t>
flow_t blocking_flow(node_t* source, node_t* target)
{
    /*
    To be used only when 'make_level_graph()' is already called .
    Iteratively updates pushes flow through various dfs paths in level graph.
    Use 'get_nodes_in_a_vec()' function from graph_utils.cpp
    */
    flow_t per_dfs_flow = 0 ;
    flow_t per_lvl_graph_flow = 0 ;

    for(auto& x : get_nodes_in_a_vec(source))
        x->visited = false ;                    //Visited of all nodes should be set to false before starting to find blocking flows

    do{
        per_dfs_flow = dfs<node_t, edge_t, flow_t>(source, target) ;
        per_lvl_graph_flow+=per_dfs_flow ; 
    }while(per_dfs_flow) ;

    return per_lvl_graph_flow ;
}

template<typename node_t, typename edge_t, typename flow_t>
flow_t dinitz(node_t* source, node_t* target)
{
    /*
    Makes a level graph using the current edges(reverse + forward) . 
    Then does dfs on the level graph to find blocking flow, while adding/modifying edges.
    The above is done till the target becomes disconnected from source.   
    */

    flow_t per_blocking_flow, ans = 0 ;
    
    do{
        source = make_level_graph<node_t, edge_t>(source) ;
        per_blocking_flow = blocking_flow<node_t, edge_t, flow_t>(source, target) ;
        ans += per_blocking_flow ;
    }while(per_blocking_flow) ;
    
    return ans ;
}
