
#include <string>
#include <regex>
#include <algorithm>
#include <fstream>

#include <StringUtil.h>
using namespace std;

/*
 NOTE: need to compile with -std=c++0x flag
*/

bool descender(int i, int j){
    return i>j;
}

int ord(char c){
    return (int)c;
}

int excelIndxToCol0(const string& alphaString){
    int idx=0;
    int multiplier=1;
    for(int i=alphaString.length()-1;i>-1;i--){
        idx+=(ord(alphaString[i])-ord('A')+1)*multiplier;
        multiplier*=26;
    }
    
    return idx-1;
}


bool charInString(char test, const char * str){
    //cerr<<"cis:a"<<endl;
	while(*str){
      //  cerr<<*str<<" "<<int(*str)<<endl;
		if(test==*str)
			return true;
		str++;
	}
	
    //cerr<<"cis:b"<<endl;
	return false;
}

void getHeader(vector<string> &header,vector<string>& prestartRows,istream& fil,int headerRow, int startRow, const string& FS){
    
    int c=0;
    while(fil.good()){
        string lin;
        
        c++;
        if(c>=startRow){
            break;
        }
        
        getline(fil,lin);
        
        if(c==headerRow){
            StringUtil::split(lin,FS,header);
        }
        
        if(c<startRow){
            prestartRows.push_back(lin);
        }

    }
}


void multiColIndicesFromHeaderMergedByString(vector<int>& indices,const vector<string>& headerFields,const string& searchString,bool clearIndices=true){
    if(clearIndices){
        indices.clear();
    }
    
    for(int i=0;i<headerFields.size();i++){
        
        if(headerFields[i]==searchString){
            indices.push_back(i);
        }
    }
}

string replaceSpecialChar(const string& src){
    string newString;
    const char* cstr=src.c_str();
    for(int i=0;i<src.length();i++){
        if(i==src.length()-1){
            //last one
            newString+=cstr[i];
            break;
        }
        if(cstr[i]=='\\'){
            switch(cstr[i+1]){
                case 'n':
                    newString+='\n';
                    i++;
                    break;
                case 'r':
                    newString+='\r';
                    i++;
                    break;
                default:
                   
                    newString+='\\';
                    
                    
            }
        }else if(cstr[i]=='^'){
            switch(cstr[i+1]){
                case 't':
                    newString+='\t';
                    i++;
                    break;
                case 'b':
                    newString+=' ';
                    i++;
                    break;
                case 's':
                    newString+='<';
                    i++;
                    break;
                case 'g':
                    newString+='>';
                    i++;
                    break;
                case 'c':
                    newString+=',';
                    i++;
                    break;
                case 'd':
                    newString+='.';
                    i++;
                    break;
                case '^':
                    newString+='^';
                    i++;
                    break;
                case 'M':
                    newString+='$';
                    i++;
                    break;
                case 'm':
                    newString+='-';
                    i++;
                    break;
                case 'o':
                    newString+=':';
                    i++;
                    break;
                case 'S':
                    newString+='\\';
                    i++;
                    break;
                default:
                    newString+='^';
                    
            }
        }else{
            newString+=cstr[i];
        }
        
    }
    
    return newString;
}


int minOfIntVector(const vector<int>& V){
    vector<int>::const_iterator i=V.begin();
    int curMin=*i;
    for(;i!=V.end();i++){
        if(*i<curMin){
            curMin=*i;
        }
    }
    
    return curMin;
}


int maxOfIntVector(const vector<int>& V){
    vector<int>::const_iterator i=V.begin();
    int curMax=*i;
    for(;i!=V.end();i++){
        if(*i>curMax){
            curMax=*i;
        }
    }
    
    return curMax;
}

void multiColIndicesFromHeaderMergedByRegex(vector<int>& indices,const vector<string>& headerFields, const string& regexString,bool clearIndices=true){
    
    if(clearIndices){
        
        indices.clear();
    }
    
    regex regx(regexString);
    
    for(int i=0;i<headerFields.size();i++){
        if(regex_match(headerFields[i],regx)){
            indices.push_back(i);
        }
        
    }
    
}

void multiColJoinField(vector<int>& indices,const vector<string>& headerFields,const string& joinString, bool clearIndices=true){
    if(clearIndices){
        indices.clear();
        
    }
    
    vector<string> splitons;
    StringUtil::split(joinString,"%",splitons);
    
    string filename=splitons[0];
    string sep;
    if(splitons.size()>1){
        sep=replaceSpecialChar(splitons[1]);
    }else{
        sep="\t";
    }
    
    //vector<string> fieldsToInclude;
    
    ifstream fil(filename.c_str());
    
    while(fil.good()){
        string line;
        getline(fil,line);
    
        vector<string> lineSplittons;
        StringUtil::split(line,sep,lineSplittons);
        for(vector<string>::iterator i=lineSplittons.begin();i!=lineSplittons.end();i++){
            //fieldsToInclude.push_back(*i);
            string fieldKey=(*i);
            for(int j=0;j<headerFields.size();j++){
                if(headerFields[j]==fieldKey){
                    indices.push_back(j);
                }
                
            }
        }
        
        
    }
    
    
    
    
    
    fil.close();
    
    
    //to be implemented
    //cerr<<"multiColJoinField Not yet implemented"<<endl;
}

void rangeListFromSingleDirectonNoClear(int& sortMode,vector<int>& rangeValues,const vector<string>& headerFields, string col1rangesplit0,int transform){
    
    char director=col1rangesplit0[0];
    //cerr<<"director=="<<director<<endl;
    int adder=0;
    //cerr<<"a"<<endl;
    vector<string> col1rangesplitsplit;
    StringUtil::splitStringByString(col1rangesplit0,">>",col1rangesplitsplit);
    //cerr<<"b"<<endl;
    
    if(col1rangesplitsplit.size()>1){
        //cerr<<"adder?"<<col1rangesplitsplit[0]<<" : "<<col1rangesplitsplit[1]<<endl;
        col1rangesplit0=col1rangesplitsplit[0];
        adder=StringUtil::atoi(col1rangesplitsplit[1]);
    }else{
        StringUtil::splitStringByString(col1rangesplit0,"<<",col1rangesplitsplit);
        if(col1rangesplitsplit.size()>1){
            col1rangesplit0=col1rangesplitsplit[0];
            adder=-1*StringUtil::atoi(col1rangesplitsplit[1]);
        }
    }
    
    //cerr<<"c"<<endl;
    
    if(charInString(director,".@%")){
        vector<int> indices;
        if(director=='.'){
            multiColIndicesFromHeaderMergedByString(indices,headerFields,col1rangesplit0.substr(1));
        }else if(director=='%')
        {
            multiColJoinField(indices,headerFields,col1rangesplit0.substr(1));
        }
        else if(director=='@')
        {
            //regex to implement
            multiColIndicesFromHeaderMergedByRegex(indices,headerFields,col1rangesplit0.substr(1));
        }
        
        if(indices.size()<1)
        {
            //error
            cerr<<"Error:"<<col1rangesplit0.substr(1)<<" not matched in header"<<endl;
            //raise error
        }
        
        //cerr<<"adder="<<adder<<endl;
        //now put into rangeValues
        for(vector<int>::iterator vi=indices.begin();vi!=indices.end();vi++)
        {
            rangeValues.push_back((*vi)+1+transform+adder);
        }
        
    }else if(director=='_')
    {
        rangeValues.push_back(headerFields.size()-StringUtil::atoi(col1rangesplit0.substr(1))+1+transform);
    }else if(director=='a'){
        
        sortMode=1;
    }
    else if(director=='d'){
        sortMode=-1;
    }
    else if(director=='+'){
        for(int i=0;i<headerFields.size();i++){
            rangeValues.push_back(i);
        }
    }else if(director=='x'){
        rangeValues.push_back(excelIndxToCol0(col1rangesplit0.substr(1)));
    }else{
        rangeValues.push_back(StringUtil::atoi(col1rangesplit0)+transform);
    }

    
}

vector<int> rangeListFromRangeStringAdv(const vector<string>& headerFields, const string& rangestring, int transform){
	
	int sortMode=0;
	
	//sortMode=0 #0:no sort, 1: ascending, -1: descending
	vector<string> col1splits;
	StringUtil::split(rangestring,",",col1splits);
	vector<int> rangeValues;
	
	for(int curColSplitI=0;curColSplitI<col1splits.size();curColSplitI++)
	{
		string col1split=col1splits[curColSplitI];
		vector<string> col1rangesplit;
		StringUtil::split(col1split,"-",col1rangesplit);
		//cerr<<col1split<<endl;
		//replace special chars
		for(int i=0;i<col1rangesplit.size();i++)
		{
            //cerr<<"before replace:"<<col1rangesplit[i];
			col1rangesplit[i]=replaceSpecialChar(col1rangesplit[i]);
            //cerr<<"after replace:"<<col1rangesplit[i]<<endl;
		}
		
        //cerr<<"replaced"<<endl;
		if(col1rangesplit.size()==1)
		{
			rangeListFromSingleDirectonNoClear(sortMode,rangeValues,headerFields,col1rangesplit[0],transform);
		}
        else{
            //cerr<<"x-y not implemented yet"<<endl;
            int dummy;//we don't want it to affect the sortMode
            vector<int> rangeLs;
            vector<int> rangeRs;
            rangeListFromSingleDirectonNoClear(dummy,rangeLs,headerFields,col1rangesplit[0],transform);
            
            rangeListFromSingleDirectonNoClear(dummy,rangeRs,headerFields,col1rangesplit[1],transform);
            
            int rangeL=minOfIntVector(rangeLs);
            int rangeR=maxOfIntVector(rangeRs);
            
            if(rangeL>rangeR){
                cerr<<"error: rangeL>rangeR"<<endl;
                //raise error;
            }
            
            for(int colToAdd=rangeL;colToAdd<=rangeR;colToAdd++){
                rangeValues.push_back(colToAdd);
            }
            
            
        }
    }
    
    
    if(sortMode==1){
        sort(rangeValues.begin(),rangeValues.end());
        
    }else if(sortMode==-1){
        sort(rangeValues.begin(),rangeValues.end(),descender);
        
    }
    
    return rangeValues;
		
	
}

vector<int> getCol0ListFromCol1ListStringAdv(const vector<string>& headerFields, const string& rangestring){
    return rangeListFromRangeStringAdv(headerFields,rangestring,-1);
}

//getHeader(vector<string> &header,vector<string>& prestartRows,istream& fil,int headerRow, int startRow, const string& FS)

class FileColSelector{
public:

    
    string filename;
    string colstring;
    int headerRow;
    int startRow;
    string fs;
    
    vector<string> header;
    vector<string> preStartRows;
    vector<int> columns0;
    
    FileColSelector():filename(""),colstring("1"),headerRow(1),startRow(2),fs("\t"){}
    
    void parse(){
        
        //cerr<<"parse "<<filename<<endl;
        
        columns0.clear();
        header.clear();
        preStartRows.clear();
        //cerr<<"headerRow "<<headerRow<<endl;
        //cerr<<"startRow "<<startRow<<endl;
        //cerr<<"fs "<<fs<<endl;
        ifstream fil(filename);
        getHeader(this->header,this->preStartRows,fil,headerRow,startRow,fs);
        fil.close();
        
        //cerr<<"preparse"<<endl;
        
        //cerr<<"fields# "<<this->header.size()<<endl;
        
        //for(vector<string>::iterator i=this->header.begin();i!=this->header.end();i++){
        //    cerr<<(*i)<<endl;
        //}
        
        columns0=getCol0ListFromCol1ListStringAdv(this->header,colstring);
        
        
        
        //cerr<<"size:"<<columns0.size()<<endl;
        //for(vector<int>::iterator i=columns0.begin();i!=columns0.end();i++){
        //    cerr<<(*i)<<endl;
        //}
        
        
    }
    
    FileColSelector(const string& _filename):filename(_filename),colstring("1"),headerRow(1),startRow(2),fs("\t"){
        columns0.push_back(0);
    }
    
    FileColSelector(const string& _filename, const string& _colstring,int _headerRow, int _startRow, const string& _fs,bool autoparse=true):filename(_filename),colstring(_colstring),headerRow(_headerRow),startRow(_startRow),fs(_fs){
        if(autoparse){
            parse();
        }
    }
    
    void setHeaderRow(int _headerRow){
        headerRow=_headerRow;
    }
    
    void setStartRow(int _startRow){
        startRow=_startRow;
    }
    
    void setFS(const string& _fs){
        fs=_fs;
    }
    
    void setColString(const string& _colstring,int _headerRow=-1,int _startRow=-1,const string& _fs="",bool autoparse=true){
        this->colstring=_colstring;
        if(_headerRow!=-1){
            headerRow=_headerRow;
        }
        if(_startRow!=-1){
            startRow=_startRow;
        }
        if(_fs!=""){
            fs=_fs;
        }
        if(autoparse){
            parse();
        }

    }
    
};

