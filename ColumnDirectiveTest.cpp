#include <iostream>
#include <string>
#include <regex>
#include "ColumnsDirectives.h"

using namespace std;


int main(int argc,char**argv)
{
    vector<string> headerFields;
    headerFields.push_back("GeneName"); //1
    headerFields.push_back("Start"); //2
    headerFields.push_back("End"); //3
    headerFields.push_back("Exp1"); //4
    headerFields.push_back("Ctl3"); //5
    headerFields.push_back("Ctl2"); //6
    headerFields.push_back("Exp2"); //7
    headerFields.push_back("Exp3"); //8
    headerFields.push_back("Ctl1"); //9
    headerFields.push_back("Ctl1"); //10
    
    string request;
    
    while(cin.good()){
        request="";
        getline(cin,request);
        
        if(request==""){
            break;
            
        }
        
        cout<<"request "<<request<<endl;
        vector<int> indices=getCol0ListFromCol1ListStringAdv(headerFields,request);
        for(vector<int>::iterator i=indices.begin();i!=indices.end();i++){
            cout<<"column "<<(*i)<<endl;
        }
    }
    
    
}