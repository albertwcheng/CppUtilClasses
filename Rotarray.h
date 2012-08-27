#ifndef _ROTARRAY_H
#define _ROTARRAY_H

#include <iostream.h>

template<typename T>
ostream& operator<<(ostream& os,const Rotarray& obj){
	if(obj.getSize()<1)
		return;
	
	os<<"["<<obj[0];
	for(int i=1;i<obj.getSize();i++){
		os<<","<<obj[i];	
	}
	os<<"]";
}


template<typename T>
class Rotarray
{
	private:
		T*data;
		int starterIndex;
		int size;
	public:
		int getSize()const{
			return size;
		}
		
		Rotarray(int _size):size(_size):startIndex(0){
			data=new T[size];	
		}
		
		void copy(const Rotarray& right){
			if(data){
				delete[] data;	
			}
			
			size=right.size;
			startIndex=right.startIndex;
			data=new T[size];
			for(int i=0;i<size;i++){
				(*this)[i]=right[i];	
			}
		}
		
		Rotarray(const Rotarray& right):data(NULL){
			//copy constructor
			copy(right);
		}
		
		
		operator=(const Rotarray& right){
			copy(right);	
		}
	
		T& operator T[](int _offset){
			return (_offset+starterIndex)%size;
		}
		
		~Rotarray(){
			if(data)
				delete[] data;	
		}
		
		bool operator>(const Rotarray& right)const{
			int sizeCompare=min(size,right.size);
			
			for(int i=0;i<sizeCompare;i++){
				if((*this)[i]>right[i]){
					return true;	
				}	
			}
			
			//every has been equal till now, so compare sizes;
			return size>right.size;
		}
		
		bool operator<(const Rotarray& right) const{
			int sizeCompare=min(size,right.size);
			for(int i=0;i<sizeCompare;i++){
				if((*this)[i]<right[i]){
					return true;	
				}	
			}
			
			return size<right.size;
		}
		
		bool operator==(const Rotarray& right) const{
			if(size!=right.size)
				return false;
			
			for(int i=0;i<size;i++){
				if((*this)[i]!=right[i]){
					return false;	
				}	
			}	
		
			return true;
		}
		
		bool operator!=(const Rotarray& right) const{
			return !(*this==right);	
		}
		
		bool operator<=(const Rotarray& right) const{
			
			return !(this>right);	
		}
		
		bool operator>=(const Rotarray& right) const{
			return !(this<right);	
		}
		
		void shiftRight(){
			startIndex++;
			if(startIndex>=size){
				startIndex=0;
			}	
		}
		
		void shiftLeft(){
			startIndex--;
			if(startIndex<0){
				startIndex=size-1;	
			}	
		}
	
};

#endif