#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include "Hashtable.h"


using namespace std;


int main(int argc, char** argv) {
	double loadfactor = 0.5;
	int sizeoftable = 2000;
	struct pairs *array0;
	struct pairs *array1;
	array0 = new pairs[sizeoftable];
	array1 = new pairs[sizeoftable];
	struct Hashtable *map1 = new Hashtable(sizeoftable,loadfactor,array0);
        struct Hashtable *map2 = new Hashtable(sizeoftable,loadfactor,array1);

	
	int sizeofarray = 1024;
	int range = 2000;
	int threshold = 4000;
	/*		
	struct pairs *arraya1 = new pairs[sizeofarray];
	struct pairs *arraya2 = new pairs[sizeofarray];
	
	generatePairs(arraya1, arraya2, sizeofarray);
	cout<<"fisrt insertion"<<endl;
	insert(arraya1,0,sizeofarray,map1,1,sizeofarray,100);
	cout<<"second insertion"<<endl;
	insert(arraya2,0,sizeofarray,map2,1,sizeofarray,100);

	if(compareTwoArray(map1->hashtable,map2->hashtable,map1->capacity)) {
                cout<<"Determinism"<<endl;

        }
	*/
	
	for(int i = 1; i < 2; i++) {
		cout<<"The "<< i <<" iteration"<<endl;
		struct pairs *arraya1 = new pairs[sizeofarray];
		struct pairs *arraya2 = new pairs[sizeofarray];

		generatePairs(arraya1,arraya2,sizeofarray,range);
				
		cout<<"Collision in first Map"<<endl;

		insert(arraya1,0,sizeofarray,map1,1,sizeofarray,256,range);
		
		cout<<"Collision in second Map"<<endl;
		insert(arraya2,0,sizeofarray,map2,1,sizeofarray,256,range);


		if(compareTwoArray(map1->hashtable,map2->hashtable,map1->capacity,range)) {
                cout<<"Determinism"<<endl;

        	}
	}

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
		

