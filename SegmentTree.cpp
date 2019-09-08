/*
RetTypeOfCalc :- Type of the value to be calculated over various ranges
BaseContainer :- Type of Container over which value is to be calculated
BaseCases :- Function returning values for the thing to be calculated over single elements. Arguments :- const BaseContainer&
             and index of type size_t
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
            ~SegmentTreeNode(){
                if(left_child!=nullptr) {
                    delete left_child;
                    left_child=nullptr;
                }
                if(right_child!=nullptr){
                    delete right_child;
                    right_child=nullptr;
                }
            }
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

    RetTypeOfCalc BuildTreeNow(SegmentTreeNode* root, size_t start_idx, size_t end_idx)
    {
        
        if(start_idx==end_idx)
            return(root->val = BaseCases(Base, start_idx));
        root->left_child = new SegmentTreeNode(root);
        root->right_child = new SegmentTreeNode(root);
        RetTypeOfCalc val1 = BuildTreeNow(root->left_child, start_idx, (start_idx+end_idx)/2);
        RetTypeOfCalc val2 = BuildTreeNow(root->right_child, ((start_idx+end_idx)/2)+1, end_idx);
        if (MergeFunc2 != nullptr)
            return(root->val = MergeFunc2(Base, val1, val2));
        else
            return(root->val = MergeFunc(val1, val2));       
    }

    pair<RetTypeOfCalc,bool> RangeQueryNow(size_t start_idx, size_t end_idx, size_t beg, size_t end, SegmentTreeNode* root) const
    {
        
        if(beg >= start_idx && end <= end_idx)
            return make_pair(root->val,true);
        
        if(beg>end_idx||end<start_idx)
            return make_pair(root->val,false);
        
        pair<RetTypeOfCalc,bool> val1 = RangeQueryNow(start_idx, end_idx, beg, (beg+end)/2, root->left_child);
        pair<RetTypeOfCalc,bool> val2 = RangeQueryNow(start_idx, end_idx, (beg+end)/2+1, end, root->right_child);
        
        if(val1.second&&val2.second)
        {
            if(MergeFunc2!=nullptr){
                return(make_pair(MergeFunc2(Base, val1.first, val2.first),true));
            }
            else{
                return(make_pair(MergeFunc(val1.first, val2.first),true));
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

    bool PointUpdateNow(size_t index, size_t range_start, size_t range_end, SegmentTreeNode* root)
    {
    
        if(range_start==range_end)
        {   
            root->val = BaseCases(Base, index);
            return true;
        }
        
        bool check;
        
        if(index<=(range_start+range_end)/2)
            check = PointUpdateNow(index,range_start,(range_start+range_end)/2,root->left_child);
        else
            check = PointUpdateNow(index,(range_start+range_end)/2+1,range_end,root->right_child);
        
        if(check)
        {
            if(MergeFunc2!=nullptr)
            {
                root->val = MergeFunc2(Base,root->left_child->val,root->right_child->val);
                return true;
            }
            else
            {
                RetTypeOfCalc x=root->val;
                root->val = MergeFunc(root->left_child->val,root->right_child->val);
                if(x==root->val)
                    return false;
                else
                    return true;
            }
        }
    }

public:    
    
    SegmentTree1d(BaseContainer _Base, RetTypeOfCalc (*_BaseCases)(const BaseContainer&, size_t), RetTypeOfCalc (*_MergeFunc)(RetTypeOfCalc, RetTypeOfCalc), size_t size ,size_t _beg = 0)
        : Base(_Base), BaseCases(_BaseCases), MergeFunc(_MergeFunc), beg(_beg), end(size-1)
    {
        MergeFunc2 = nullptr;
        root = new SegmentTreeNode; 
    }
    
    SegmentTree1d(BaseContainer _Base, RetTypeOfCalc (*_BaseCases)(const BaseContainer&, size_t ), RetTypeOfCalc (*_MergeFunc)(const BaseContainer& , RetTypeOfCalc, RetTypeOfCalc), size_t size, size_t _beg = 0)
        : Base(_Base), BaseCases(_BaseCases), MergeFunc2(_MergeFunc), beg(_beg), end(size-1)
    {
        MergeFunc = nullptr;
        root = new SegmentTreeNode; 
    }
    
    SegmentTree1d& BuildTree(){
        BuildTreeNow(root,beg,end);
        return *this;
    }

    RetTypeOfCalc RangeQuery(size_t start_idx, size_t end_idx) const
    {
        return RangeQueryNow(start_idx, end_idx, beg, end, root).first;
    }

    SegmentTree1d& PointUpdate(size_t index, typename BaseContainer::value_type new_val){
        Base[index] = new_val;
        PointUpdateNow(index, beg, end, root);
    }

    SegmentTree1d& ClearTree(){
        delete root;
    }
    
    ~SegmentTree1d(){
        delete root;
    }
};

int BaseCases(const vector<int>& a, size_t index)
{return a[index];}

int Merge(int a, int b)
{return a+b;}
  

int main(){
    vector<int> a  {2,7,9,8,10,5};
    SegmentTree1d<vector<int>,int> st(a,BaseCases,Merge,a.size()); //Segment Tree to find maximum in any range of array
    st.BuildTree();
    cout<<st.RangeQuery(0,3)<<endl;
    st.PointUpdate(3,10);
    cout<<st.RangeQuery(1,3)<<endl;
    st.ClearTree();
}
