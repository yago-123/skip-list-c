#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include"skip_list/skip_list.h"


#define BENCHMARKNUMBERS 1000000

float benchmarkInserir(skip_list *sl, int randomNumbers[]); 
float benchmarkEsborrar(skip_list *sl, int randomNumbers[]); 
float benchmarkBuscar(skip_list *sl, int randomNumbers[]); 
int benchmarkCost_Buscar(skip_list *sl, int randomNumbers[]); 
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
	if (Crear(&sl) == SUCCESS) { 
		total_time = benchmarkInserir(&sl, randomNumbers); 
		printf("Inserted %d values in %0.2f milliseconds = %0.2f seconds\n", BENCHMARKNUMBERS, total_time, total_time/1000.0); 
		
		total_time = benchmarkBuscar(&sl, randomNumbers); 
		printf("Searched %d random numbers in %0.2f milliseconds = %0.2f seconds\n", BENCHMARKNUMBERS, total_time, total_time/1000.0); 

		comparations = benchmarkCost_Buscar(&sl, randomNumbers); 
		printf("Search random number over %d values, cost %0.2f comparations/search\n", BENCHMARKNUMBERS, (float)comparations/(float)BENCHMARKNUMBERS);

		total_time = benchmarkEsborrar(&sl, randomNumbers);
		printf("Deleted %d numbers, in %0.2f miliseconds = %0.2f seconds\n", BENCHMARKNUMBERS, total_time, total_time/1000.0); 	


		// free memory 
		Destruir(&sl); 
	} else {
		printf("Could not load skip list\n"); 
	}	
}

float benchmarkInserir(skip_list *sl, int randomNumbers[]) {
	int i;	
	gettimeofday(&start, 0); 	
		
	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		Inserir(&(*sl), randomNumbers[i]);  	
	}

	gettimeofday(&end, 0); 
	return timedifference_msec(start, end);   
}

float benchmarkBuscar(skip_list *sl, int randomNumbers[]) {
	int i;
       	bool find; 
	gettimeofday(&start, 0);  

	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		// find will check if number has been found  
		Buscar(*sl, randomNumbers[i], &find); 
	}

	gettimeofday(&end, 0);  
	return timedifference_msec(start, end); 
}

int benchmarkCost_Buscar(skip_list *sl, int randomNumbers[]) {
	int total = 0, tmp, i; 
	
	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		// tmp will return number of comparations 
		Cost_Buscar(*sl, randomNumbers[i], &tmp); 
		total += tmp;  
	}

	return total; 
}

float benchmarkEsborrar(skip_list *sl, int randomNumbers[]) {
	int i; 
	gettimeofday(&start, 0); 

	for(i = 0; i < BENCHMARKNUMBERS; i++) {
		Esborrar(&(*sl), randomNumbers[i]); 
	}

	gettimeofday(&end, 0); 
	return timedifference_msec(start, end);  
}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
	    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

