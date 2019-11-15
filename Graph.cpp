#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<map>
#include<utility>
#include<type_traits>
#include<limits>

#define defEdgeInfo 0
using namespace std;

template<class NodeType>
class Node{
    protected : 
        NodeType info;
        void* calc_info_ptr;
    public :
        Node() : calc_info_ptr(nullptr) {}
        Node(NodeType a) : info(a), calc_info_ptr(nullptr) {}
        void set_info(NodeType a) {info=a;}
        NodeType get_info() {//cout<<"Returning node info = "<<info<<endl;
        return info;}
        void*& get_calc_info_ptr() { return calc_info_ptr; }
};

template<class NodeType,class edge>
class NodeWithEdgeList : public Node<NodeType>{
        vector<edge*> adjacent_nodes;
        using f = typename result_of<decltype(&edge::get_info)(edge&)>::type;
    public :    
        NodeWithEdgeList() {}
        NodeWithEdgeList(NodeType a) : Node<NodeType>(a) {}
        NodeWithEdgeList(NodeType a, NodeWithEdgeList* parent) : Node<NodeType>(a) { parent->add_edge(this); }  
        NodeWithEdgeList(NodeType a, NodeWithEdgeList* parent, f edge_info) : Node<NodeType>(a) { parent->add_edge(this, edge_info); }
        vector<edge*>& get_adjacent_nodes() {//cout<<"SIz "<<adjacent_nodes.size()<<endl; 
        //cout<<adjacent_nodes[0].get_adjacent()<<endl;
        return adjacent_nodes; }
        edge& add_edge(NodeWithEdgeList* other_end, f edge_info=defEdgeInfo) {
                edge* e1 = new edge(this, edge_info, other_end);
                //cout<<"main"<<e1<<endl; 
                adjacent_nodes.push_back( e1 ); //cout<<"Node pushed"<<adjacent_nodes.size(); 
                //cout<<(*e1).get_adjacent()<<endl;
                return *e1;
        }
        bool operator >(NodeWithEdgeList* b){return *(this->calc_info_ptr) > *(b->calc_info_ptr); };
};
//template<class NodeType,class edge>
//bool NodeWithEdgeList::operator >(NodeWithEdgeList* a,NodeWithEdgeList* b) {return *(a->calc_info_ptr) > *(b->calc_info_ptr);}
class edge{
    protected :
        void* adjacent;
    public :
        edge(void* child) : adjacent(child){//cout<<"Setting adjacent "<<child<<endl;
        }
        void* get_adjacent() { //cout<<"Heyyy"<<adjacent<<endl; 
        return adjacent; }
};


template<class EdgeType>
class edge_with_info : public edge{
    protected:
        EdgeType info;
    public :    
        edge_with_info(void* child) : edge(child){ info = defEdgeInfo; }
        edge_with_info(void* a, EdgeType eg_in, void* child) : info(eg_in), edge(child) {}
        EdgeType get_info() { return info; }
};

template<class EdgeType>
class edge_with_parent : public edge_with_info<EdgeType>{
        void* parent;
    public :    
        edge_with_parent(void* child, void* paren) : parent(paren), edge(child) {}
        edge_with_parent(void* paren, EdgeType eg_in, void* child) : edge_with_info<EdgeType>(paren,eg_in, child), parent(paren) {}
        void* get_parent(){ return parent; }
};


template<class SelEdge, class SelNode>
class Graph{
    
    using u = typename result_of<decltype(&SelNode::get_info)(SelNode&)>::type;
    using y = typename result_of<decltype(&SelNode::get_info)(SelNode&)>::type;
    vector<SelNode*> head;
    
    class bfs_iter{
        
        SelNode* cur;
        queue<SelNode*> bfs_queue;
        map<SelNode*,bool> vis;
        Graph* gptr;
        public :
            bfs_iter(SelNode* a, Graph* g) : cur(a), gptr(g){
                bfs_queue.push(cur);
                for(int i=0;i<gptr->get_head().size();++i){
                    vis[gptr->get_head()[i]]=false;
                }
                vis[cur]=true;
                cur=this->operator++();
            }
            
            SelNode& operator*(){
                return *cur;
            }
            SelNode& operator ->(){
                return *cur;
            }
            SelNode* operator ++(){
                if(!bfs_queue.empty()){
                    
                    cur = bfs_queue.front();
                    SelNode* child;
                    auto& j=cur->get_adjacent_nodes();
                    for(auto it=j.begin();it!=j.end();it++){
                        child = static_cast<SelNode*>((*it)->get_adjacent());
                        if(!vis[child]) {bfs_queue.push( child ); vis[child]=true;}                                             //Or use *.?
                    }
                    bfs_queue.pop();
                    //cout<<"Check This "<<(*cur).get_adjacent_nodes()[0].get_adjacent()<<"  "<<j[0].get_adjacent()<<endl;
                    return cur;
                }
                else{
                    return nullptr;       
                }
            }
    };

    class dfs_iter{
        
        SelNode* cur;
        stack<SelNode*> dfs_stack;
        map<SelNode*,bool> vis;
        Graph* gptr;
        public :
            dfs_iter(SelNode* a,Graph* g) : cur(a) , gptr(g){
                dfs_stack.push(cur);
                size_t x = gptr->get_head().size();
                for(int i=0;i<x;++i){
                    vis[gptr->get_head()[i]]=false;
                }
                cur=this->operator++();
            }
            
            SelNode& operator*(){
                return *cur;
            }

            SelNode* operator ++(){
                SelNode* child;
                if(!dfs_stack.empty()){
                    cur = dfs_stack.top();
                    dfs_stack.pop();
                    vis[cur]=true;
                    for(auto it=cur->get_adjacent_nodes().begin();it<cur->get_adjacent_nodes().end();it++){
                        child = static_cast<SelNode*>((*it)->get_adjacent());
                        if(vis[child]==false) dfs_stack.push( child ); 
                    }                                              //Or use *.?
                    return cur;
                }
                else{
                    return nullptr;       
                }
            }
    };
    
    template<class T>
    class edge_iter{
        T node_iter;
        size_t i;
        SelEdge* cur_edge;
        Graph* gptr;
        public :
            edge_iter(SelNode* a,Graph* g) : gptr(g),node_iter(a,g),i(0)  {
                if(i<(*node_iter).get_adjacent_nodes().size())
                {   //cout<<"Node "<<(*(node_iter)).get_info()<<endl<<((*node_iter).get_adjacent_nodes()[i]).get_adjacent()<<endl;
                    //cout<<"main"<<&a[0]<<endl; 
                    cur_edge = (*node_iter).get_adjacent_nodes()[i];
                    //cout<<"hellooo  "<<(*cur_edge).get_adjacent()<<endl;
                }
                else {
                    cout<<"Initial node has no edges from it"<<endl;
                }
            }
            SelEdge* operator ++(){
                if(i<(*node_iter).get_adjacent_nodes().size()-1)
                {   
                    i++;
                    cur_edge=(*node_iter).get_adjacent_nodes()[i];
                    return cur_edge;
                }
                else{
                    //cout<<"Here"<<endl;
                    if((++node_iter)==nullptr) {//cout<<"huhu"<<endl;
                    return nullptr;}
                    if(!((*node_iter).get_adjacent_nodes().size()==0))
                    {   
                        i=0;
                        cur_edge=(*node_iter).get_adjacent_nodes()[i];
                        return cur_edge; }
                    else {
                        //cout<<"kj"<<endl;
                        while(((*node_iter).get_adjacent_nodes().size()==0)){
                            if((++node_iter)==nullptr) return nullptr;
                        }
                        cur_edge=(*node_iter).get_adjacent_nodes()[i];
                        return cur_edge;
                    }

                }
            }
            SelEdge& operator *(){
                return *cur_edge;
            }
    };

public:

    Graph() {}
    SelNode& add_node() {SelNode* ptr = new SelNode(); head.push_back(ptr); return *ptr; }
    
    void add_nodes(size_t no_of_nodes){ for(size_t i=0; i<no_of_nodes;++i) add_node(); }
    
    SelNode& add_node(u info) { SelNode* ptr = new SelNode(info); head.push_back(ptr); return *ptr; }
    
    
    SelEdge& add_directed_edge(SelNode* from, SelNode* to, y edge_info = defEdgeInfo)
    { return from->add_edge(to,edge_info); }
    
    void add_undirected_edge(SelNode* from, SelNode* to, y edge_info = defEdgeInfo)
    { from->add_edge(to,edge_info); to->add_edge(from,edge_info); }
    
    vector<SelNode*>& get_head() { return head; }
    bfs_iter bfs_begin(SelNode* a=nullptr) { return a==nullptr?bfs_iter(head[0],this):bfs_iter(a,this); }
    dfs_iter dfs_begin(SelNode* a=nullptr) { return a==nullptr?dfs_iter(head[0],this):dfs_iter(a,this); }
    edge_iter<bfs_iter> edge_iter_begin(SelNode* a=nullptr){ return a==nullptr?edge_iter<bfs_iter>(head[0],this):edge_iter<bfs_iter>(a,this); }

    y dijkstra(SelNode* target=nullptr, SelNode* source = nullptr, y zero_dist = 0){
        if(source==nullptr) source = head[0];
        priority_queue<SelNode*,vector<SelNode*>,greater<SelNode*>> pq;
        pq.push(source);
        source->get_calc_info_ptr() = new y;
        y* sptr = static_cast<y*>(source->get_calc_info_ptr());
        *(sptr) = zero_dist;
        SelNode* ptr;
        while(!pq.empty()){
            ptr = pq.top();
            if(ptr==target){
                return *(static_cast<y*>(target->get_calc_info_ptr()));
            }
            pq.pop();
            y paren_node_dist;
            y* p = static_cast<y*>(ptr->get_calc_info_ptr());
            if(p!=nullptr) paren_node_dist = *p;
            else paren_node_dist = numeric_limits<y>::max();
            for( auto& x : ptr->get_adjacent_nodes()){
                SelNode* adj = static_cast<SelNode*>(x->get_adjacent());
                if(adj->get_calc_info_ptr()==nullptr)
                    adj->get_calc_info_ptr() = new y(x->get_info()+paren_node_dist);
                else
                    *(static_cast<y*>(adj->get_calc_info_ptr())) = min( x->get_info()+paren_node_dist, *(static_cast<y*>((adj->get_calc_info_ptr()))));
                pq.push(adj);
            }
        }
        return zero_dist;
    }
    
    y get_calc_info(SelNode* a){ //cout<<"Here"<<endl; 
    return *(static_cast<y*>(a->get_calc_info_ptr()));}
    
    void bellman_ford(SelNode* source = nullptr, y zero_dist=0){
        if(source==nullptr) source = head[0];
        source->get_calc_info_ptr() = new y;
        y* sptr = static_cast<y*>(source->get_calc_info_ptr());
        *(sptr) = zero_dist;
        for(int i=0;i<head.size()-1;++i){
            for(auto it = edge_iter_begin(source);;){
                y *parent,*child;
                SelNode *al,*be;
                al = static_cast<SelNode*>((*it).get_adjacent());
                be = static_cast<SelNode*>((*it).get_parent());
                //cout<<"al = "<<al<<endl;
                if(be->get_calc_info_ptr()==nullptr){
                    //cout<<"Hey"<<endl;
                    be->get_calc_info_ptr() = new y(numeric_limits<y>::max());
                }
                parent = static_cast<y*>(be->get_calc_info_ptr());
                if(al->get_calc_info_ptr()==nullptr){
                    //cout<<"Hey"<<endl;
                    al->get_calc_info_ptr() = new y(numeric_limits<y>::max());
                }
                child = static_cast<y*>(al->get_calc_info_ptr());                
                //cout<<"Here   "<<*(parent)<<"   "<<*static_cast<y*>(be->get_calc_info_ptr())<<endl;
                //cout<<((*it).get_info())<<endl;
                if(*(child)>*(parent)+((*it).get_info()))
                    *(child) = *(parent)+(*it).get_info();
                if((++it)==nullptr){                     
                    //cout<<"NOOOO"<<endl;
                    break;
                }
            }       
        //cout<<"end"<<endl;      
        }
    }
    
    void clear_calc_data(){
        for(auto &node : head){
            
            if(node->get_calc_info_ptr()!=nullptr){
                delete static_cast<y*>(node->get_calc_info_ptr());
                node->get_calc_info_ptr()=nullptr;
            }
        }
    }
    
};

int main(){
    Graph<edge_with_parent<int>,NodeWithEdgeList<int,edge_with_parent<int>>> G;
    G.add_node(4);
    G.add_node(5);
    G.add_node(7);
    G.add_node(8);
    auto& vec = G.get_head();
    G.add_directed_edge(vec[0],vec[1],1);
    G.add_directed_edge(vec[1],vec[2],2);
    auto e3 = G.add_directed_edge(vec[0],vec[2],3);
    G.add_directed_edge(vec[1],vec[3],6);
    for(auto it=G.edge_iter_begin();;){
        cout<<((*it).get_info())<<endl;
        if ((++it)==nullptr) break;
    }
    //cout<<"Here";
    for(auto it=G.dfs_begin();;){
        cout<<(*it).get_info()<<endl;
        if((++it)==nullptr)  break;
    }
    cout<<G.dijkstra(vec[3],vec[0])<<endl;
    //cout<<"Dijks done"<<endl;
    G.clear_calc_data();
    G.bellman_ford();
    cout<<G.get_calc_info(vec[3]); 
    G.clear_calc_data();  
}