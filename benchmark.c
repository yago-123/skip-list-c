#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include"skip_list/skip_list.h"


#define BENCHMARKNUMBERS 1000000

float benchmarkInsert(skip_list *sl, int randomNumbers[]); 
float benchmarkDelete(skip_list *sl, int randomNumbers[]); 
float benchmarkSearch(skip_list *sl, int randomNumbers[]); 
int benchmarkCost_Search(skip_list *sl, int randomNumbers[]); 
float timedifference_msec(struct timeval t0, struct timeval t1); 

struct timeval start, end;
float total_time; 
int comparations;

int main() {
	srand(0); 

	printf("Creating random numbers array...\n"); 
	int i, randomNumbers[BENCHMARKNUMBERS]; 
	for (i = 0; i < BENCHMARKNUMBERS; i++) {
		randomNumbers[i] = rand();	
	}

	skip_list sl; 
	if (Create(&sl) == SUCCESS) { 
		total_time = benchmarkInsert(&sl, randomNumbers); 
		printf("Inserted %d values in %0.2f milliseconds = %0.2f seconds\n", BENCHMARKNUMBERS, total_time, total_time/1000.0); 
		
		total_time = benchmarkSearch(&sl, randomNumbers); 
		printf("Searched %d random numbers in %0.2f milliseconds = %0.2f seconds\n", BENCHMARKNUMBERS, total_time, total_time/1000.0); 

		comparations = benchmarkCost_Search(&sl, randomNumbers); 
		printf("Search random number over %d values, cost %0.2f comparations/search\n", BENCHMARKNUMBERS, (float)comparations/(float)BENCHMARKNUMBERS);

		total_time = benchmarkDelete(&sl, randomNumbers);
		printf("Deleted %d numbers, in %0.2f miliseconds = %0.2f seconds\n", BENCHMARKNUMBERS, total_time, total_time/1000.0); 	


		// free memory 
		Destroy(&sl); 
	} else {
		printf("Could not load skip list\n"); 
	}	
}

float benchmarkInsert(skip_list *sl, int randomNumbers[]) {
	int i;	
	gettimeofday(&start, 0); 	
		
	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		Insert(&(*sl), randomNumbers[i]);  	
	}

	gettimeofday(&end, 0); 
	return timedifference_msec(start, end);   
}

float benchmarkSearch(skip_list *sl, int randomNumbers[]) {
	int i;
       	bool endnd; 
	gettimeofday(&start, 0);  

	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		// endnd will check if number has been found  
		Search(*sl, randomNumbers[i], &endnd); 
	}

	gettimeofday(&end, 0);  
	return timedifference_msec(start, end); 
}

int benchmarkCost_Search(skip_list *sl, int randomNumbers[]) {
	int total = 0, tmp, i; 
	
	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		// tmp will return number of comparations 
		Cost_Search(*sl, randomNumbers[i], &tmp); 
		total += tmp;  
	}

	return total; 
}

float benchmarkDelete(skip_list *sl, int randomNumbers[]) {
	int i; 
	gettimeofday(&start, 0); 

	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		Delete(&(*sl), randomNumbers[i]); 
	}

	gettimeofday(&end, 0); 
	return timedifference_msec(start, end);  
}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
	    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

