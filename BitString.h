#ifndef __BITSTRING_H
#define __BITSTRING_H

#include <iostream>
#include <stdint.h>
#include <fstream>

typedef unsigned char Byte;

using namespace std;


class BitString{
public:

    uint64_t numBytes;
    uint64_t numBits;

    Byte* _data;
    
    BitString(const string& filename){
        ifstream ifil(filename.c_str(),ios::binary|ios::in);
        ifil.seekg(0,ios::end);
        numBytes=ifil.tellg();
        numBits=numBytes*8;
        //ifil.close();
        _data=new Byte[numBytes];
        ifil.clear();
        ifil.seekg(0);
        ifil.read((char*)_data,numBytes);
        ifil.close();
    }
    
    
    void init(uint64_t _numBits)
    {
        numBits=_numBits;
        numBytes=numBits/8;
        if(numBits%8>0){
            numBytes++;
        }
        
        _data=new Byte[numBytes];
        for(uint64_t i=0;i<numBytes;i++){
            _data[i]=0;
        }
    }
    
    BitString(uint64_t _numBits)
    {

        init(_numBits);
    }
    
    BitString():numBytes(0),numBits(0),_data(NULL){
    }
    
    
    ~BitString(){
        delete[] _data;
    }
    inline void getByteBitIndex(uint64_t index,uint64_t& byteIndex,uint64_t& bitIndex){
        byteIndex=index/8;
        bitIndex=index%8;
    }
    void setBit(uint64_t index,bool value){
        uint64_t byteIndex;
        uint64_t bitIndex;
        getByteBitIndex(index,byteIndex,bitIndex);
        if(value){
            _data[byteIndex]|=(1<<bitIndex);
        }else{
            _data[byteIndex]&=(~(1<<bitIndex));
        }
    }
    
    bool getBit(uint64_t index){
        uint64_t byteIndex;
        uint64_t bitIndex;
        getByteBitIndex(index,byteIndex,bitIndex);
        return _data[byteIndex]&(1<<bitIndex);
    }
    
    void print(ostream& os,bool printReturn=false){
        for(uint64_t i=0;i<numBytes;i++){
            for (uint64_t j=0;j<8;j++){
                os<<bool(_data[i]&(1<<j));
            }
            
            os<<" ";
        }
        
        if(printReturn)
            os<<endl;
    }
    
    void writeToFile(const string& filename)
    {
        ofstream ofil(filename.c_str(),ios::binary|ios::out|ios::trunc);
        ofil.write((const char*)_data,numBytes);
        ofil.close();
    }
    
   	string getBitsAsString(uint64_t start0, uint64_t end1)
   	{
   		string s;
   		for(uint64_t i=start0;i<end1;i++){
   			s+=getBit(i)?"1":"0";	
   		}
   		return s;
   	}	
    void setBitsOnRange(uint64_t start0, uint64_t end1, bool value)
    {
        for(uint64_t i=start0;i<end1;i++){
            setBit(i,value);
        }
    }
    
};

class FileBitString{
private:
    
    string filename;
    uint64_t numBytes;
    uint64_t numBits;
    fstream *fil;
public:

    FileBitString(const string& _filename,uint64_t _numBits):numBits(_numBits),filename(_filename){
        numBytes=numBits/8;
        if(numBits%8>0){
            numBytes++;
        }
        
        //cerr<<numBytes<<"\t"<<numBits<<endl;
        
        fil=new fstream(_filename.c_str(),ios::in|ios::out|ios::binary);
        if(!fil->good())
        {
            //cerr<<"new file"<<endl;
            fil->close();
            delete fil;
            fil=new fstream(_filename.c_str(),ios::in|ios::out|ios::binary|ios::trunc);
        }
        fil->seekp(0,ios::end);
        
        uint64_t curFileSize=fil->tellp();
        while (fil->tellp()<numBytes) {
            fil->put(0);
            //cerr<<"put"<<fil->tellp()<<endl;
        }
        
        fil->clear();
        
        
    }

inline void getByteBitIndex(uint64_t index,uint64_t& byteIndex,uint64_t& bitIndex){
    byteIndex=index/8;
    bitIndex=index%8;
}
void setBit(uint64_t index,bool value){
    uint64_t byteIndex;
    uint64_t bitIndex;
    getByteBitIndex(index,byteIndex,bitIndex);
    
    fil->seekg(byteIndex);

    char _data=fil->get();
    
    if(value){
        _data|=(1<<bitIndex);
    }else{
        _data&=(~(1<<bitIndex));
    }
    fil->seekp(byteIndex);
    fil->put(_data);
}

bool getBit(uint64_t index){
    uint64_t byteIndex;
    uint64_t bitIndex;
    getByteBitIndex(index,byteIndex,bitIndex);
    
    fil->seekg(byteIndex);
    char _data=fil->get();
    return _data&(1<<bitIndex);
}
    
    void print(ostream& os,bool printReturn=false){
        fil->seekg(0);
        while(fil->good()){
            Byte _data=fil->get();
            if(fil->good()){
            for(int j=0;j<8;j++){
                os<<bool(_data&(1<<j));
                
            }
                os<<" ";
            }
        }
        fil->clear();
        if(printReturn)
            os<<endl;
    }


    ~FileBitString(){
        fil->close();
        delete fil;
    }

};

#endif