
#include <iostream>
#include <vector>
using namespace std;




class ListPermutator
{
public:
    virtual int numOfLists()=0;
    virtual int sizes(int i)=0;
    virtual bool onItem(const vector<int>& idx)=0; //bool return true to continue or false for not
    virtual void onStart(){} //run first when runThroughList is called
    virtual void onEnd(){} //run when runThroughList is finished
    
    
    void runThroughList()
    {
        vector<int> idx;
        int N=numOfLists();
        
        for(int i=0;i<N;i++){
            idx.push_back(0);
        }
        
        if(N==1){
            for( idx[0]=0;idx[0]<sizes(0);idx[0]++){
                if(!onItem(idx)){
                    return;
                }
            }
            
            return;
        }
        
        
        
        while(idx[0]<sizes(0)){
            if(idx[N-2]==sizes(N-2)){
                int k=N-2;
                while(k>0 && idx[k]==sizes(k)){
                    idx[k]=0;
                    idx[k-1]++;
                    k--;
                }
            }
            
            if(idx[0]==sizes(0))
                break;
            
            
            for( idx[N-1]=0;idx[N-1]<sizes(N-1);idx[N-1]++){
                if(!onItem(idx)){
                    return;
                }
            }
            
            idx[N-2]++;
            
        }
        
        
    }
    
    
    
};

class SimpleIntPermutator: public ListPermutator
{
public:
    
    
    
    
    vector<int> sizesList;
    int pcount;
    SimpleIntPermutator(const vector<int>& _sizesList):sizesList(_sizesList),pcount(0){}
    
    
    
    void print(const vector<int>& L){
        pcount++;
        cerr<<pcount<<":";
        for(int i=0;i<L.size();i++){
            cerr<<" "<<L[i];
            
        }
        
        
        cerr<<endl;
    }
    
    
    
    int numOfLists(){
        return sizesList.size();
    }
    int sizes(int i){
        return sizesList[i];
    }
    bool onItem(const vector<int>& idx){
        print(idx);
        return true;
    }
    
    void onStart(){
        cerr<<"start"<<endl;
    }
    void onEnd(){
        cerr<<"end"<<endl;
    }
    
    static int main(int argc, const char * argv[])
    {
        cerr<<"helllo"<<endl;
        vector<int> sizes;
        for(int i=1;i<argc;i++){
            cerr<<"sizes["<<(i-1)<<"]="<<atoi(argv[i])<<endl;
            sizes.push_back(atoi(argv[i]));
        }
        
        SimpleIntPermutator perm(sizes);
        perm.runThroughList();
        
        return 0;
    }
    
    
    
};
