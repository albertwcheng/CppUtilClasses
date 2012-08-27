#include <iostream>
#include <string>
using namespace std;

include "Rotarray.h"

int main(int argc,char** argv){
	
	Rotarray<string> sarray(3);
	sarray[0]="hello";
	sarray[1]="world";
	sarray[2]="Albert";
	
	cout<<sarray<<endl;
	
	return 0;	
}