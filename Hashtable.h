#include <sys/time.h>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <atomic>
#include <bits/stdc++.h>
using namespace std;



struct pairs{
 	int key;
	int value;
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
inline bool SCAS(int *ptr, int oldv, int newv);
int insert(pairs *array,int startpoint, int sizeofarray,Hashtable *map,int resizingflag,int totalsize,int thresholds,int range);
int compare_and_swap(Hashtable* a,int index, pairs b, pairs c);

void generatePairs(pairs *array1,pairs *array2, int amount,int range) {

	srand (time(NULL));
	int index;
	pairs pair1;
	pairs pair2;
	for(int i; i < amount; i++) {
		pair1.key = rand() % range; 
		pair2.key = pair1.key;

		pair1.value = rand() % range;
		pair2.value = pair1.value;

		array1[i] = pair1;
		array2[i] = pair2;
	}

}
int compareTwoArray(pairs *array1, pairs *array2,int size,int range) {
	int flag = 1;
	for(int i = 0; i < size; i++) {
		if(array2[i].key <= range && array1[i].key <= range &&  array1[i].key != array2[i].key) {
			cout<<i<<"    "<<array1[i].key<<"   "<<array2[i].key<<endl;
			flag = 0;
		}

	}
	return flag;

}


int resizing(Hashtable *map,int sizeofarray,int totalsize,int threshold,int resizingtime,int range){
	//cout<<"before resizing"<<endl;	
	if(resizingtime == 1) {
	int oldcapacity = map->capacity;
	 int newcapacity = (oldcapacity +sizeofarray)/(map->loadfactor-0.1);
        struct pairs *newarray = new pairs[newcapacity]; 
        double loadfactor = map->loadfactor;

      	int count = 0;
        pairs *oldarray = new pairs[oldcapacity];
	for(int i; i < oldcapacity; i++) {
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
//	cout<<"linear probing1"<<endl;
	int index1 = index;
	while(map->hashtable[index1].key != 0 && map->hashtable[index1].value != 0) {
		index1 = (index1 + 5 * map->probing) % map->capacity;		
		map->probing += 1;
	}
	struct pairs v = map->hashtable[index1];
	int* ptr = &(map->hashtable[index1].key);
	if(SCAS(ptr,v.key,c.key)) {
		cout<<"linear compare  "<<v.key<<"     "<<c.key<<endl;
		//cout<<"linear probing successful"<<endl;
	} else {

		cout<<"linear probing fail"<<endl;
	}
	return 0;

}
inline bool SCAS(int *ptr, int oldv, int newv) {
  unsigned char ret;
  /* Note that sete sets a 'byte' not the word */
  __asm__ __volatile__ (
                "  lock\n"
                "  cmpxchgl %2,%1\n"
                "  sete %0\n"
                : "=q" (ret), "=m" (*ptr)
                : "r" (newv), "m" (*ptr), "a" (oldv)
                : "memory");
  return ret;
}


int compare_and_swap(Hashtable* a,int index, pairs b, pairs c) {

	if(a->hashtable[index].key == b.key && a->hashtable[index].value == b.value) {

		a->hashtable[index].key = c.key;
		a->hashtable[index].value = c.value;
	
		return 1;
	
	} else {

		a->hashtable[index].key= b.key;
		a->hashtable[index].value = b.value;
		return 0;

	}



}

int prioritycheck(pairs* array, int totalsize, pairs c){
	for(int j = 0; j < totalsize; j++) {
             	if(c.key == array[j].key) {                                       
                       return j;
               	}
        }
	return (totalsize + 2);
}

int insertSingle(pairs *array, int i, Hashtable *map,int totalsize,int range) {
	int toHash = array[i].key;	
	int index = (int)(hashI(toHash) % map->capacity);
                    struct pairs v;
			v.key =  array[i].key;
			v.value = array[i].value;
                    while(v.key != 0 && v.key < range) {
			int index2 = -1;
                        struct pairs c;
			c.key = map->hashtable[index].key;
			c.value = map->hashtable[index].value;
			if(c.key == v.key) {
                                return 1;
                        } else { 
				if(c.key != 0) {
					index2 = prioritycheck(array,totalsize,c);
					if( index2 <= i) {
							index += 1;
							continue;
                                         } else if (index2 > i && index2 != totalsize+2) {
						int* ptr = &(map->hashtable[i].key); 
						if(SCAS(ptr,c.key,v.key)) {
								cout<<"compare priority  "<<c.key<<"    "<<v.key<<endl;
                                                                v.key = c.key;
                                                                v.value = c.value;
                                                                index += 1;
                                                } else {

							cout<<"priority compare fail"<<endl;
						}
					 }        
                           	} else  {
					int* ptr = &(map->hashtable[i].key);

					if(SCAS(ptr,c.key,v.key)) {                  
                                                                return 1;

                                         } else {
						cout<<"zero compare fail  "<<c.key<<"      "<<v.key<<endl;
					}	
				}

				if(index2 == totalsize + 2) {
					if(v.key < range && c.key <range) {
                                        	if(linearProbing(v, map,index)) {
                                                	return 1;
						}
					} else {
						return 0;	
					}
                                }
				continue;	
                        }

                    }
		return 1;
		
}

int insert(pairs *array,int startpoint, int sizeofarray,Hashtable *map,int resizingflag,int totalsize,int thresholds,int range){
	int threshold = thresholds;

	if(resizingflag == 1 && ((map->size + sizeofarray) > (map->loadfactor * map->capacity))) {

		if(map->size != 0) {

			resizing(map,sizeofarray,totalsize,threshold,1,range);
		} else {
			resizing(map,sizeofarray,totalsize,threshold,0,range);
		}
	}


	if(sizeofarray <= threshold) {
		int index;
		for(int i = startpoint; i < (startpoint + sizeofarray); i++) {
			if(array[i].key < range) {
				insertSingle(array,i,map,totalsize,range);
			}
		}
	} else {
		cilk_spawn insert(array,startpoint,sizeofarray/4, map,0,totalsize,threshold,range);
		cilk_spawn insert(array,startpoint+sizeofarray/4,sizeofarray/4, map,0,totalsize,threshold,range);
		cilk_spawn insert(array,startpoint+sizeofarray/2,sizeofarray/4, map,0,totalsize,threshold,range);
			   insert(array,startpoint+(sizeofarray*3/4),sizeofarray/4, map,0,totalsize,threshold,range);
		cilk_sync;
		if(resizingflag == 1) {
			map->size = map->size + sizeofarray;
		}
	}
}

