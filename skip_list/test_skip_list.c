#include"skip_list.h"

#define INITIALNUMBERS 20

void mostraSkipList(skip_list sl); 
int contaLongitud(int num);  
int mostraMenu(); 

int main() {
	skip_list sl; 
	bool trobat; 
	int i, opcio, elem, cost; 

	Crear(&sl); 
	for(i = 0; i < INITIALNUMBERS; i++) {
		Inserir(&sl, (rand()%100)); 
	}

	mostraSkipList(sl); 

	opcio = mostraMenu(); 
	while(opcio != 0) {
		switch(opcio) {
			case 1: 
				printf("Insert element: "); 
				scanf("%d", &elem); 

				Inserir(&sl, elem); 
				break;  
			case 2:
				printf("Delete element: "); 
				scanf("%d", &elem);

				Esborrar(&sl, elem);  
				break; 
			case 3:
				printf("Search element: "); 
				scanf("%d", &elem); 

				Buscar(sl, elem, &trobat); 
				if(trobat) {
					printf("Value PRESENT in the list!\n\n"); 
				} else {
					printf("Value NOT PRESENT in the list!\n\n"); 
				}
				break;  
			case 4:
				printf("Number of comparations searching element: ");
				scanf("%d", &elem); 

				if(Cost_Buscar(sl, elem, &cost) != ELEMENT_NO_TROBAT) {
					printf("Value found, COST = %d comparations\n\n", cost); 
				} else {
					printf("Value not found, COST = %d comparations\n\n", cost); 
				} 
				break; 
			default: 
				printf("Unexpected error\n"); 
		}

		if(opcio < 3) {
			mostraSkipList(sl);
		} 
		opcio = mostraMenu(); 
	}

	Destruir(&sl); 
	return 0;
}

void mostraSkipList(skip_list sl) {
	bool fi; 
	skip_node *node, *tmp; 
	int i, j, k, maxLevels, lon;
	int **arr; 

	printf("Interactive Skip List: \n"); 

	// Inicialitza array
	maxLevels = sl.head->level+1; 
	arr = (int **)malloc(maxLevels*sizeof(int *)); 

	node = sl.head; 
	// Inicialitza punters dels arrays  
	for(i = 0; i < maxLevels; i++) {
		arr[i] = (int*)malloc((sl.nElems+2)*sizeof(int));	
		for(j = 0; j < (sl.nElems+2); j++) {
			arr[i][j] = INDETERMINAT; 
		}

		if(node->level != 0) { 
			node = node->down; 
		} 
	}

	// Guarda a matriu els elements 
	for(j = 0; j < (sl.nElems+2); j++) {
		fi = false; 
		i = 0; 
		tmp = node; 
		while(i < maxLevels && !fi) {
			if(node != NULL) {
				arr[i][j] = node->key; 
				node = node->up; 
			} else {
				fi = true; 
			}

			i++; 
		}
		node = tmp->next; 		
	} 

	// Mostra matriu; 
	for(i = maxLevels-1; i > -1; i--) {
		for(j = 0; j < (sl.nElems+2); j++) {
			if(arr[i][j] == INFINIT_NEGATIU) {
				printf("-inf "); 
			} else if(arr[i][j] == INFINIT_POSITIU) {
				printf("inf "); 
			} else if(arr[i][j] == INDETERMINAT) {
				for(k = 0; k < contaLongitud(arr[0][j]); k++) {
					printf("-"); 
				}  

				printf(" "); 
			} else {
				printf("%d ", arr[i][j]); 
			}
		}

		printf("\n"); 
	}

	// Allibera matriu 
	for(i = 0; i < maxLevels; i++) {
		free(arr[i]); 
	}

	Longitud(sl, &lon); 
	printf("List len: %d\n", lon); 
	printf("---------------------------\n"); 
}

// Determina longitud nombres 
int contaLongitud(int num) {
	int count = 0; 

	do {
		count++; 
		num /= 10; 
	} while(num != 0); 

	return count; 
}

int mostraMenu() {
	int opcio; 

	printf("\t1. Insert\n");  
	printf("\t2. Delete\n");  
	printf("\t3. Search\n");  
	printf("\t4. Cost of searching\n");  

	printf("\n\t0. Destroy and Exit\n");  

	do {
		printf("\tInsert option: "); 
		scanf("%d", &opcio); 
	} while(opcio < 0 || opcio > 5); 

	return opcio; 
}
