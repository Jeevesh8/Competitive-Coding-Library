#include<iostream>
#include<vector>
#include<utility>
#include<list>
using namespace std;


template<typename node_t>
size_t hld(node_t* root)
{
    /*
    Input :-  Pointer to root of
              Tree of Nodes: Node :- {children(a vector of pointers to children of node);
                                      color -> integer;}
                                      NOTE :- children vector is empty if no children.
    Output :- Color is assigned to each node.
    */
    static size_t color_counter = 0;

    if( root->children.empty() )
    {
        root->color = color_counter;
        color_counter++;
        return 1;
    }
    vector<size_t>  child_results;
    
    size_t my_result=0 ;
    
    for(auto& x : root->children)
    {
        child_results.push_back(hld(x));  
    }
    
    for(auto& x : child_results)
    {
        my_result+=x;
    }
    
    for(size_t i =0; i< child_results.size(); ++i)
    {
        if(child_results[i]>my_result/2)
        {
            root->color = root->children[i]->color;
            return my_result;
        }
    }
    root.color = root->children[0]->color;
    return my_result;
}