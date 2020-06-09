#include <sys/time.h>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <atomic>
#include <set>
#include <bits/stdc++.h>
#include "utils.h"
using namespace std;
using namespace utils;


struct pairs{
  	unsigned int key;
	unsigned int value;
	unsigned int index;
	/*pairs(int key,int value) {
		this->key = key;
		this->value = value;
	}*/
};

struct Hashtable{
	int size;
	double loadfactor;
	pairs *hashtable;
	int capacity;	
	int probing;
	Hashtable(int size, double loadfactor, pairs *array) {
		this->size = 0;
		this->capacity = size;
		this->loadfactor = loadfactor;
		hashtable = array;
		probing = 0;
	}
};

inline unsigned int hashI(unsigned int a)
{
   a = (a*0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   return a;
}
//inline bool SCAS(int *ptr, int oldv, int newv);
int insert(pairs *array,int startpoint, int sizeofarray,Hashtable *map,int resizingflag,int totalsize,int thresholds,int range);
int compare_and_swap(Hashtable* a,int index, pairs b, pairs c);

void generatePairs(pairs *array1,pairs *array2, int amount,int range) {

	srand (time(NULL));
	int index;
	int i = 0;
	pairs pair1;
	pairs pair2;
	
	set<int> myset;
	set<int>::iterator it;
	while(i < amount ) {
	
		pair1.key = rand() % range + 1;
		pair2.key = pair1.key;
		pair1.value = rand() % range + 1;
		pair2.value = pair1.value;
		pair1.index = i;
		pair2.index = i;
		it = myset.find(pair1.key);
	
		if(it == myset.end() ) {
			myset.insert(pair1.key);			
			array1[i] = pair1;
			array2[i] = pair2;
			i++;
		} else {
			
			continue;
		}
	}

}
int compareTwoArray(pairs *array1, pairs *array2,int size,int range) { 
	for(int i = 0; i < size; i++) {
		if(array1[i].key != array2[i].key) {
			return 0;
		}
	}
	return 1;

}


int resizing(Hashtable *map,int sizeofarray,int totalsize,int threshold,int resizingtime,int range){
	cout<<"before resizing"<<endl;	
	if(resizingtime == 1) {
	int oldcapacity = map->capacity;
	 int newcapacity = (oldcapacity +sizeofarray)/(map->loadfactor-0.1);
        struct pairs *newarray = new pairs[newcapacity]; 
        double loadfactor = map->loadfactor;

  	struct pairs *oldarray = new pairs[totalsize]; 
       	int count = 0; 
	for(int i = 0; i < oldcapacity; i++) {
		if(map->hashtable[i].key != 0 && map->hashtable[i].value != 0) {	
			oldarray[count].key  = map->hashtable[i].key;
			oldarray[count].value = map->hashtable[i].value;
			count++;
		
		}

	}

        	map->hashtable =  newarray;
		map->capacity = newcapacity;
        	insert(oldarray,0,count,map,0,count,threshold,range);
	} else {
		 int oldcapacity = map->capacity;
         int newcapacity = (oldcapacity +sizeofarray)/(map->loadfactor-0.1);
        struct pairs *newarray = new pairs[newcapacity];
        double loadfactor = map->loadfactor;

     
    
        map->hashtable =  newarray;
        map->capacity = newcapacity;
       


	}

}

int linearProbing(pairs c, Hashtable *map, int index) {
	int index1 = index;
	while(map->hashtable[index1].key != 0 && map->hashtable[index1].value != 0) {
		index1 = (index1 + 5 * map->probing) % map->capacity;		
		map->probing += 1;
	}
	struct pairs v = map->hashtable[index1];
	unsigned int* ptr = &(map->hashtable[index1].key);
	unsigned int* ptr1 = &(map->hashtable[index1].value);
	if(CAS(ptr,v.key,c.key) && CAS(ptr1,v.value,c.value)) {
		
	}
	return 0;

}

/*
int prioritycheck(pairs* array, int totalsize, pairs c){
	int flag = totalsize + 2;
	
		for(int j = 0; j < totalsize; j++) {
             		if(c.key == array[j].key && c.value == array[j].value) {                                       
                       		return j;
               		}
        	}
		for(int j = 0; j < totalsize; j++) {
			if(c.key == array[j].key) {
                                return j;
                        }
		}
	cout<<"Priority check not found C "<<c.key<<"   "<<c.value<<endl;
	return flag;
}*/
double get_sec()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec + 1e-6 * time.tv_usec);
}
int insertSingle(pairs *array, pairs p, Hashtable *map,int totalsize,int range) {	
	int toHash1 = p.key;
	int index = (int)(hashI(toHash1) % map->capacity);
        struct pairs v = p;
                    while(v.key != 0 && v.value != 0) {
			int index2 = -1;
			int index3 = -1;
                        struct pairs c = map->hashtable[index];
			if(c.key == v.key) {
                                return 1;
                        } else { 
				if(c.key != 0 && c.key < range) {
				
					/*index2 = prioritycheck(array,totalsize,c);
					index3 = prioritycheck(array,totalsize,v);*/
					index2 = c.index;
					index3 = v.index;
					if(index2 < index3) {
							index += 1;
							continue;
                                         } else if (index2 > index3 && index2 != totalsize+2) {
				
						unsigned int* ptr = &(map->hashtable[index].key); 
						unsigned int* ptr1 = &(map->hashtable[index].value);
						unsigned int* ptr2 = &(map->hashtable[index].index);
						if(CAS(ptr,c.key,v.key)&& CAS(ptr1,c.value,v.value)&&CAS(ptr2,c.index,v.index)) {
						                v = c;
                                                                index += 1;
								if(v.value == 0) {
									return 1;
								}
                                                }
					 } else if(index2 == totalsize +2 && c.key < range) {
                                                if(linearProbing(v, map,index)) {
                                                        return 1;
                                                }	
					 }     
                           	} else  {
					unsigned int* ptr = &(map->hashtable[index].key);
					unsigned int* ptr1 = &(map->hashtable[index].value);
					unsigned int* ptr2 = &(map->hashtable[index].index);
					if(CAS(ptr1,c.value,v.value) && CAS(ptr,c.key,v.key)&&CAS(ptr2,c.index,v.index)) {                  
                                                                return 1;

                                         }
				}
				continue;	
                        }

                    }
		return 1;
		
}

int insert(pairs *array,int startpoint, int sizeofarray,Hashtable *map,int resizingflag,int totalsize,int thresholds,int range){
	if(resizingflag == 1 && ((map->size + sizeofarray) > (map->loadfactor * map->capacity))) {
		if(map->size != 0) {
			resizing(map,sizeofarray,totalsize,thresholds,1,range);
		} else {
			resizing(map,sizeofarray,totalsize,thresholds,0,range);
		}		
	}
	if(sizeofarray <= thresholds) {
		for(int i = startpoint; i < (startpoint + sizeofarray); i++) {			
				insertSingle(array,array[i],map,totalsize,range);
			
		}
	} else {
		cilk_spawn insert(array,startpoint,sizeofarray/4, map,0,totalsize,thresholds,range);
		cilk_spawn insert(array,startpoint+sizeofarray/4,sizeofarray/4, map,0,totalsize,thresholds,range);
		cilk_spawn insert(array,startpoint+sizeofarray/2,sizeofarray/4, map,0,totalsize,thresholds,range);
			   insert(array,startpoint+(sizeofarray*3/4),sizeofarray/4, map,0,totalsize,thresholds,range);
		cilk_sync;
		if(resizingflag == 1) {
			map->size = map->size + sizeofarray;
		}
	}
}

bool find(pairs p, Hashtable *map,pairs *array, int sizeofarray) {
	int toHash1 = p.key;
        
        int index = (int)(hashI(toHash1) % map->capacity);

        int index1 = -1;
	int index2 = -1;
	
	struct pairs v = p;
	index1 = v.index;
	//index1 = prioritycheck(array,sizeofarray,v);
	int count = 0;
	
	while(map->hashtable[index].key != 0) {
		count += 1;
		struct pairs c = map->hashtable[index];
        	//index2 = prioritycheck(array,sizeofarray,c);
        	index2 = c.index;
		if(index1 == sizeofarray + 2) {
			
			cout<<"not found"<<endl;
			return false;
		} else if(index1 == index2) {
			if((c.key == v.key && c.value == v.value)) {
				return true;
			} 
		} else if(index1 >  index2){
			index += 1;
		} else {
			return false;

		}
	}


}
