#include<iostream>
#include<vector>
#include<cmath>
#include<limits>
#include<map>

using namespace std;

#define p_infty(t) +numeric_limits<t>::infinity()

//Function that takes in a Node Iterator/Source Node and returns a vector of pointers to nodes.
template< typename node_t, typename NodeIter = bfs_iter<node_t> >
vector<node_t*>& get_nodes_in_a_vec(node_t* source=nullptr, NodeIter ni=nullptr)
{
    /*
    Usage :-
        Provide Node iterator if you have your own iterator already. And set source to nullptr, explicitly.
        If you don't have a node iterator, simply call get_nodes_in_a_vec(&source_node) ;
        Iterator must have .end() and ++ implemented. *ni == pointer to node
    */
    vector<node_t*> node_list ;
    if(ni==nullptr)
    {
        ni = NodeIter(source) ;
    }
    while(ni!=ni.end())
    {
        node_list.push_back(*ni) ;
        ni++ ;
    }
    return node_list ;
}


//Function that takes in a Node Iterator/Source node and returns adjacency maatrix
template<typename node_t, typename edge_t, typename dist_t, typename NodeIter = bfs_iter<node_t> >
vector< vector<dist_t> >& get_adjacency_matrix(node_t* source=nullptr, NodeIter ni=nullptr)
{
    /*
    Time Complexity :-  
                  O(max(E, NlogN))
    Conditions :- 
                  Edges must have weight and s and t . 
                  Nodes must have get_edge_iter() that returns a iterator for container having pointers to outward edges.
                  For un-directed edges, edges in both directions must be present .
    Usage :-
                  Provide Node iterator if you have your own iterator already. And set source to nullptr, explicitly.
                  If you don't have a node iterator, simply call get_nodes_in_a_vec(&source_node) ;
                  Iterator must have .end() and ++ implemented. *ni == pointer to node
    */
    vector<node_t*> node_list = get_nodes_in_a_vec(source, ni) ;
    map<node_t*, size_t> node_num_map ;
    
    for(size_t i=0; i<node_list.size(); ++i)
    {
        node_num_map[node_list[i]] = i ;
    }

    vector< vector<dist_t> > adjacency_mat(node_list.size(), vector<dist_t> (node_list.size(), p_infty(dist_t))) ;
    for(size_t i=0; i<node_list.size(); ++i)
    {
        adjacency_mat[i][i] = 0 ;
        for( auto& x : node_list[i].get_edge_iter() )
        {
            adjacency_mat[i][node_num_map[x->t]] = x->weight ;
        }
    }

    return adjacency_mat ;
}

