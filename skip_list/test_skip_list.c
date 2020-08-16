#include"skip_list.h"

#define INITIALNUMBERS 20

void showSkipList(skip_list sl); 
int countLength(int num);  
int showMenu(); 

int main() {
	skip_list sl; 
	bool found; 
	int i, option, elem, cost; 

	Create(&sl); 
	for(i = 0; i < INITIALNUMBERS; i++) {
		Insert(&sl, (rand()%100)); 
	}

	showSkipList(sl); 

	option = showMenu(); 
	while(option != 0) {
		switch(option) {
			case 1: 
				printf("Insert element: "); 
				scanf("%d", &elem); 

				Insert(&sl, elem); 
				break;  
			case 2:
				printf("Delete element: "); 
				scanf("%d", &elem);

				Delete(&sl, elem);  
				break; 
			case 3:
				printf("Search element: "); 
				scanf("%d", &elem); 

				Search(sl, elem, &found); 
				if(found) {
					printf("Value PRESENT in the list!\n\n"); 
				} else {
					printf("Value NOT PRESENT in the list!\n\n"); 
				}
				break;  
			case 4:
				printf("Number of comparations searching element: ");
				scanf("%d", &elem); 

				if(Cost_Search(sl, elem, &cost) != ELEMENT_NOT_FOUND) {
					printf("Value found, COST = %d comparations\n\n", cost); 
				} else {
					printf("Value not found, COST = %d comparations\n\n", cost); 
				} 
				break; 
			default: 
				printf("Unexpected error\n"); 
		}

		if(option < 3) {
			showSkipList(sl);
		} 
		option = showMenu(); 
	}

	Destroy(&sl); 
	return 0;
}

void showSkipList(skip_list sl) {
	bool end; 
	skip_node *node, *tmp; 
	int i, j, k, maxLevels, length;
	int **arr; 

	printf("Interactive Skip List: \n"); 

	// Initialize array
	maxLevels = sl.head->level+1; 
	arr = (int **)malloc(maxLevels*sizeof(int *)); 

	node = sl.head; 
	// Initialize pointers from arrays  
	for(i = 0; i < maxLevels; i++) {
		arr[i] = (int*)malloc((sl.nElems+2)*sizeof(int));	
		for(j = 0; j < (sl.nElems+2); j++) {
			arr[i][j] = INDETERMINATE; 
		}

		if(node->level != 0) { 
			node = node->down; 
		} 
	}

	// Save element matrix 
	for(j = 0; j < (sl.nElems+2); j++) {
		end = false; 
		i = 0; 
		tmp = node; 
		while(i < maxLevels && !end) {
			if(node != NULL) {
				arr[i][j] = node->key; 
				node = node->up; 
			} else {
				end = true; 
			}

			i++; 
		}
		node = tmp->next; 		
	} 

	// Show matrix 
	for(i = maxLevels-1; i > -1; i--) {
		for(j = 0; j < (sl.nElems+2); j++) {
			if(arr[i][j] == INFINITE_NEGATIVE) {
				printf("-inf "); 
			} else if(arr[i][j] == INFINITE_POSITIVE) {
				printf("inf "); 
			} else if(arr[i][j] == INDETERMINATE) {
				for(k = 0; k < countLength(arr[0][j]); k++) {
					printf("-"); 
				}  

				printf(" "); 
			} else {
				printf("%d ", arr[i][j]); 
			}
		}

		printf("\n"); 
	}

	// Free matrix  
	for(i = 0; i < maxLevels; i++) {
		free(arr[i]); 
	}

	Length(sl, &length); 
	printf("List len: %d\n", length); 
	printf("---------------------------\n"); 
}
 
int countLength(int num) {
	int count = 0; 

	do {
		count++; 
		num /= 10; 
	} while(num != 0); 

	return count; 
}

int showMenu() {
	int option; 

	printf("\t1. Insert\n");  
	printf("\t2. Delete\n");  
	printf("\t3. Search\n");  
	printf("\t4. Cost of searching\n");  

	printf("\n\t0. Destroy and Exit\n");  

	do {
		printf("\tInsert option: "); 
		scanf("%d", &option); 
	} while(option < 0 || option > 5); 

	return option; 
}
