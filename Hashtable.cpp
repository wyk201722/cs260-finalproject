#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include "Hashtable.h"
#include <math.h>
#include <unordered_map>

using namespace std;


int main(int argc, char** argv) {
	double loadfactor = 0.5;
	int sizeoftable = 4096 * 4 * 4 * 4 * 4 * 4 * 4;
	struct pairs *array0;
	struct pairs *array1;
	struct pairs *array2;
	array0 = new pairs[sizeoftable];
	array1 = new pairs[sizeoftable];
	array2 = new pairs[sizeoftable];
	struct Hashtable *map1 = new Hashtable(sizeoftable,loadfactor,array0);
        struct Hashtable *map2 = new Hashtable(sizeoftable,loadfactor,array1);
	struct Hashtable *map3 = new Hashtable(sizeoftable,loadfactor,array2);
	unordered_map<int, int> umap; 
 	double t0, t1,t2,t3;	
	int sizeofarray = 4096 * 4 * 4 * 4 * 4;
	int threshold = 256;
	if (argc < 2) {
                cout << "Usage: ./a.out [num_elements] [threshold=1]" << endl;
                return 0;
        }
        sizeofarray = pow(4,atoi(argv[1])) * 4096 ;
        if (argc == 3) {
                threshold = atoi(argv[2]);}

	cout<<"size of array is "<<sizeofarray<<endl;

	int range = sizeofarray * 2;
		
		struct pairs *arraya1 = new pairs[sizeofarray];
		struct pairs *arraya2 = new pairs[sizeofarray];

		generatePairs(arraya1,arraya2,sizeofarray,range);
		
		t2 = get_sec();
                for(int j = 0; j < sizeofarray; j++) {
                        insertSingle(arraya1,arraya1[j],map3,sizeofarray,range);


                }
                 t3 = get_sec();
                 //cout<<"Operation time for inserting single takes time"<< t3 - t2<<endl; 

		
		//cout<<"Collision in first Map"<<endl;
		t0 = get_sec();

		insert(arraya1,0,sizeofarray,map1,1,sizeofarray,threshold,range);
		t1 = get_sec();

		cout<<"Operation time for threshold with "<<threshold<<" takes time"<< t1 - t0<<endl;
		//cout<<endl;
		//cout<<endl;	
		//cout<<"Collision in second Map"<<endl;
		t2 = get_sec();
		insert(arraya2,0,sizeofarray,map2,1,sizeofarray,threshold,range);	
		t3 = get_sec();

		cout<<"Operation time for threshold with "<<threshold<<" takes time"<< t3 - t2<<endl;
		
		 /*t2 = get_sec();		
		for(int j = 0; j < sizeofarray; j++) {
			insertSingle(arraya1,arraya1[j],map3,sizeofarray,range);


		}
		 t3 = get_sec();
		 cout<<"Operation time for inserting single takes time"<< t3 - t2<<endl; */
		if(compareTwoArray(map1->hashtable,map2->hashtable,map1->capacity,range)) {
                	cout<<"Determinism"<<endl;

        	}
		t2 = get_sec();	
		cilk_for(int j = 0; j < sizeofarray; j++) {
	
			if(!find(arraya1[j],map1,arraya1,sizeofarray)) {

			cout<<"Could not find, fail"<<endl;
		        
			}
		}
		t3 = get_sec();
		cout<<"Find Operation time for takes time"<< t3 - t2<<endl;
		
	t0 = get_sec();	
	for(int i = 0; i < sizeofarray; i++) {
		umap.insert(make_pair(arraya1[i].key,arraya1[i].value));
	}
	t1 = get_sec();

	t2 = get_sec();
	unordered_map<int,int>::const_iterator got;
	for(int i = 0; i < sizeofarray; i++) {
		got = umap.find(arraya1[i].key);
		if(got == umap.end()) {
			cout<<"Counld not find"<<endl;
		}
	}
	t3 = get_sec();

	cout<<"Sequential find time for threshold with takes time"<< t3 - t2<<endl;
	cout<<"Sequential add for sequential takes time"<< t1 - t0<<endl;	
}
	/*
	struct pairs *arraya1 = new pairs[sizeofarray];
	struct pairs *arraya2 = new pairs[sizeofarray];

	struct pairs *arrayb1 = new pairs[sizeofarray * 3];
	struct pairs *arrayb2 = new pairs[sizeofarray * 3];
	
	generatePairs(arraya1,arraya2,sizeofarray);
	generatePairs(arrayb1,arrayb2,sizeofarray*3);

	compareTwoArray(arraya1,arraya2,sizeofarray);
	compareTwoArray(arrayb1,arrayb2,sizeofarray*3);

	if(compareTwoArray(map1->hashtable,map2->hashtable,map1->capacity)) {
                cout<<"Determinism"<<endl;

        }


	cout<<"first phase"<<endl;
	insert(arraya1,0,sizeofarray,map1,1,sizeofarray);
	insert(arraya1,0,sizeofarray,map2,1,sizeofarray);
        	
	if(compareTwoArray(map1->hashtable,map2->hashtable,map1->capacity)) {
                cout<<"Determinism"<<endl;

        }



	cout<<"second phase"<<endl;
	insert(arrayb2,0,sizeofarray * 3,map1,1,sizeofarray*3);
	insert(arrayb2,0,sizeofarray * 3,map2,1,sizeofarray*3);



	if(compareTwoArray(map1->hashtable,map2->hashtable,map1->capacity)) {
		cout<<"Determinism"<<endl;

	}*/
		

