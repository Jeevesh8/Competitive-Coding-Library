/*
RetTypeOfCalc :- Type of the value to be calculated over various ranges
BaseContainer :- Type of Container over which value is to be calculated
BaseCases :- Function returning values for the thing to be calculated over single elements. Arguments :- const BaseContainer& and index
MergeFunc :- Function returning calculated value over some range given the outputs of its 2 half ranges , as input
MergeFunc2 :- Same as MergeFunc, but takes a const reference to base container as 1 extra argument
USAGE :- 
1.    vector<int> a  {1,2,3,4,5};
2.    SegmentTree<vector<int>,float> st(a,BaseCases,Merge,a.size());
3.    st.BuildTree();
4.    cout<<st.RangeQuery(1,3);
5.    st.PointUpdate(3,7);

2nd line makes a segment tree over the vector 'a' for calculting the metric of type float specified by BaseCases and Merge Funcions
together.
4th line prints the output of range 1 to 3, both inclusive.
5th line updates the tree by changing the value at index 3 to 7.
*/
#include<bits/stdc++.h>
using namespace std;

template<class BaseContainer, class RetTypeOfCalc>

class SegmentTree1d{
    class SegmentTreeNode{
        public :
            RetTypeOfCalc val;
            SegmentTreeNode* left_child;
            SegmentTreeNode* right_child;
            SegmentTreeNode* parent;
            SegmentTreeNode() : left_child(nullptr), right_child(nullptr), parent(nullptr){}
            SegmentTreeNode(SegmentTreeNode* x) : left_child(nullptr), right_child(nullptr), parent(x){}
    };
private :
    BaseContainer Base;
    
    RetTypeOfCalc (*BaseCases)(const BaseContainer& obj, size_t idx);
    RetTypeOfCalc (*MergeFunc)(RetTypeOfCalc val_1, RetTypeOfCalc val_2);
    RetTypeOfCalc (*MergeFunc2)(const BaseContainer& obj, RetTypeOfCalc val_1, RetTypeOfCalc val_2);
    //RetTypeOfCalc (*MergeFunc3)(BaseContainer& obj, RetTypeOfCalc val_1, RetTypeOfCalc val_2, size_t beg1, size_t beg2, size_t end1, size_t end2);

    size_t beg;
    size_t end;
    
    SegmentTreeNode* root;

    RetTypeOfCalc BuildTreeNow(SegmentTreeNode* root, size_t i=1, size_t start_idx=beg, size_t end_idx=end)
    {
        
        if(start_idx==end_idx)
            return(root->val = BaseCases(Base, start_idx));
        
        root->left_child = new SegmentTreeNode(root);
        root->right_child = new SegmentTreeNode(root);
        RetTypeOfCalc val1 = BuildTreeNow(left_child, i*2, start_idx, (start_idx+end_idx)/2);
        RetTypeOfCalc val2 = BuildTreeNow(right_child, i*2+1, ((start_idx+end_idx)/2)+1, end_idx);
        if (MergeFunc2 != nullptr)
            return(root->val = MergeFunc2(Base, val1, val2));
        else
            return(root->val = MergeFunc(val1, val2));       
    }

    pair<RetTypeOfCalc,bool> RangeQueryNow(size_t start_idx, size_t end_idx, size_t beg=beg, size_t end=end, SegmentTreeNode* root=root) const
    {
        
        if(beg == start_idx && end == end_idx)
            return make_pair(root->val,true);
        
        if(beg>=end_idx||end<start_idx)
            return make_pair(root->val,false);
        
        pair<RetTypeOfCalc,bool> val1 = RangeQueryNow(start_idx, end_idx, beg, (beg+end)/2, root->left_child);
        pair<RetTypeOfCalc,bool> val2 = RangeQueryNow(start_idx, end_idx, (beg+end)/2+1, end, root->right_child);
        
        if(val1.second&&val2.second)
        {
            if(MergeFunc2!=nullptr){
                return(make_pair(MergeFunc2(Base, val1, val2),true));
            }
            else{
                return(make_pair(MergeFunc(val1, val2),true));
            }
        }
        
        else if(val1.second)
        {
            return(val1);
        }
        
        else
        {
            return(val2);
        }
    }

    void PointUpdateNow(size_t index, size_t range_start=beg, size_t range_end=end, SegmentTreeNode* root=root)
    {
        if(range_start==range_end)
        {   
            root->val = BaseCases(Base, index);
            return;
        }
        
        if(index<=(range_start+range_end)/2)
            PointUpdateNow(index,range_start,(range_start+range_end)/2,root->left_child);
        else
            PointUpdateNow(index,(range_start+range_end)/2,range_end,root->right_child);
        
        if(MergeFunc2!=nullptr)
        {
            root->val = MergeFunc2(Base,root->left_child->val,root->right_child->val);
        }
        else
        {
            root->val = MergeFunc(root->left_child->val,root->right_child->val);
        }
        
    }

public:    
    
    SegmentTree1d(BaseContainer _Base, RetTypeOfCalc (*_BaseCases)(const BaseContainer&, size_t), RetTypeOfCalc (*_MergeFunc)(RetTypeOfCalc, RetTypeOfCalc), size_t size ,size_t beg_ = 0)
        : Base(_Base), BaseCases(_BaseCases), MergeFunc(_MergeFunc), beg(_beg), end(size-1)
    {
        MergeFunc2 = nullptr;
        root = new SegmentTreeNode; 
    }
    
    SegmentTree1d(BaseContainer _Base, RetTypeOfCalc (*_BaseCases)(const BaseContainer&, size_t ), RetTypeOfCalc (*_MergeFunc)(const BaseContainer& , RetTypeOfCalc, RetTypeOfCalc), size_t size, size_t beg_ = 0)
        : Base(_Base), BaseCases(_BaseCases), MergeFunc2(_MergeFunc), beg(_beg), end(size-1)
    {
        MergeFunc = nullptr;
        root = new SegmentTreeNode; 
    }
    
    SegmentTree1d& BuildTree(){
        BuildTreeNow(root);
        return *this;
    }

    RetTypeOfCalc RangeQuery(size_t start_idx, size_t end_idx) const
    {
        return RangeQueryNow(start_idx, end_idx).first;
    }

    SegmentTree1d& PointUpdate(size_t index, typename BaseContainer::value_type new_val){
        Base[index] = new_val;
        PointUpdateNow(index);
    }
};

int main(){
    int BaseCases(vector<int>& a, int index){return a[index];}
    int Merge(int a, int b){return max(a,b);}
    vector<int> a  {1,2,3,4,5};
    SegmentTree<vector<int>,int> st(a,BaseCases,Merge,a.size());
    st.BuildTree();
    cout<<st.RangeQuery(1,3);
    st.PointUpdate(3,7);

}
