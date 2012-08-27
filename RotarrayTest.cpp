#include <iostream>
#include <string>
using namespace std;

#include "Rotarray.h"

int main(int argc,char** argv){
	
	Rotarray<string> sarray(3);
	sarray[0]="hello";
	sarray[1]="world";
	sarray[2]="Albert";
	
	cout<<sarray<<endl;
	
	Rotarray<string> sarray2(sarray);
	
	cout<<(sarray==sarray2)<<endl;
	
	sarray.shiftRight();
	cout<<sarray<<endl;
	
	cout<<sarray2<<endl;
	cout<<(sarray==sarray2)<<endl;
	
	
	for(int i=0;i<10;i++){
		sarray2.shiftRight();
		cout<<(i+1)<<":"<<sarray2<<endl;	
	}
	
	
	
	
	cout<<(sarray==sarray2)<<endl;
	
	sarray2.shiftRight();
	cout<<sarray<<endl;
	cout<<sarray2<<endl;
	cout<<(sarray<sarray2)<<endl;
	cout<<(sarray>sarray2)<<endl;
	return 0;	
}