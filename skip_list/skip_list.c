#include"skip_list.h"

int Create(skip_list *sl) {
	srand(time(0)); 
	sl->nElems = 0; 

	sl->head = (skip_node*)malloc(sizeof(skip_node)); 
	sl->head->level = -1; 

	sl->tail = (skip_node*)malloc(sizeof(skip_node)); 
	sl->tail->level = -1; 

	// Create endrst valid level: 0 
	if(createNewUpperLayer(&(*sl)) == SUCCESS) {
		return SUCCESS; 
	} else {
		return ERROR_CREATION; 
	}
}

int Destroy(skip_list *sl) {
	bool end; 
	int i, j, maxLevel;
	skip_node *tmp, *tmp2, *tmp3; 

	if(Exist(*sl)) {
		maxLevel = sl->head->level+1; 
		// Take low level head 
		tmp = sl->head; 
		while(tmp->level != 0) {
			tmp = tmp->down; 
		}

		// Walks node 1 to 1, from down to up
		for(j = 0; j < (sl->nElems+2); j++) {
			tmp2 = tmp->next; 
			end = false; 
			i = 0; 
			while(!end && i < maxLevel) {
				if(tmp != NULL) {
					tmp3 = tmp; 
					tmp = tmp->up;
					// Allibera node 
					free(tmp3);  
				} else {
					end = true; 
				} 
				i++; 
			} 

			tmp = tmp2; 
		}

		sl->nElems = -1; 

		return SUCCESS;
	} else {
		return ERROR_DESTROY;  
	}
}

int Insert(skip_list *sl, int elem) {
	int i; 
	bool newLevel; 
	skip_node *node, *tmp, *tmp2; 

	node = sl->head; 
	// Search until level 0 
	while(node->level != 0) {
		if(node->key <= elem && node->next->key <= elem) {
			node = node->next;
		} else {
			node = node->down; 	
		}
	} 

	// Search positions inside level 0
	while(elem > node->key) {
		node = node->next; 
	}

	// Insert element
	tmp = (skip_node*)malloc(sizeof(skip_node)); 
	tmp->key = elem; 
	tmp->level = 0;
	tmp->up = NULL;  
	tmp->down = NULL; 

	tmp->next = node; 
	tmp->prev = node->prev;
	node->prev->next = tmp;
	node->prev = tmp;

	// Positions node behind tmp 
	node = tmp->prev;  

	// Insert column (coin flip) 
	i = 1;  
	while((rand()%2) == 0) {
		newLevel = false; 
		// Search endrst level with superior level
		while(node->up == NULL && !newLevel) {
			if((node->key == INFINITE_NEGATIVE) && 
					(node->level == sl->head->level)) {
				newLevel = true; 
			} else {
				node = node->prev; 
			}
		}

		// Create new layer
		if(newLevel) {
			createNewUpperLayer(&(*sl)); 
			node = sl->head; 
		} else {
			// Move node higher
			node = node->up;
		} 

		// Position tmp in upper layer 
		tmp->up = (skip_node*)malloc(sizeof(skip_node)); 
		tmp->up->key = elem; 
		tmp->up->level = i;

		// Reestablish direction
		tmp2 = tmp;  
		tmp = tmp->up;
		tmp->down = tmp2;  

		tmp->prev = node; 
		tmp->next = node->next;

		node->next->prev = tmp; 
		node->next = tmp; 

		i++;   
	}

	// Increment elements and put end 
	tmp->up = NULL;  
	sl->nElems++; 

	return 0;
}

int Delete(skip_list *sl, int elem) {
	bool present; 

	if(Exist(*sl)) {
		// Delete all the nodes with that element
		Search((*sl), elem, &present);
		while(present) { 
			// Delete node with elem value
			deleteOneElement(&(*sl), elem);
			Search((*sl), elem, &present); 
		} 

		return SUCCESS; 
	} else {
		return NOT_DELETED_LIST; 
	}
}

int Length(skip_list sl, int *length) {
	if(Exist(sl)) {
		*length = sl.nElems; 
		return SUCCESS; 
	} else {
		return NOT_DELETED_LIST; 
	} 
}

int Search(skip_list sl, int elem, bool *found) {
	skip_node *tmp; 

	tmp = sl.head; 
	*found = false; 

	if(Exist(sl)) {
		while(!(*found) && elem >= tmp->key) {
			// If is equal 
			if(elem == tmp->key) {
				*found = true; 
			} else {
				// If element is smaller and not equals level 0
				if(tmp->next->key > elem && tmp->level != 0) {
					tmp = tmp->down; 	
				} else {
					// If elem is bigger 
					tmp = tmp->next; 
				} 
			}
		}

		return SUCCESS; 
	} else {
		return NOT_DELETED_LIST; 
	}
}

int Cost_Search(skip_list sl, int elem, int *cost) {
	skip_node *tmp; 
	bool found; 

	tmp = sl.head; 
	found = false; 

	*cost = 0; 
	if(Exist(sl)) {
		// If is equal
		while(!found && elem >= tmp->key) {
			if(elem == tmp->key) {
				found = true; 
			} else {
				// If elem is smaller and different from level 0 
				if(tmp->next->key > elem && tmp->level != 0) {
					tmp = tmp->down;
				} else {
					// If element is bigger 
					tmp = tmp->next; 
				} 

				(*cost)++; 
			}
		}

		if(found) {
			return SUCCESS;
		} else {
			return ELEMENT_NOT_FOUND; 
		} 
	} else {
		return NOT_DELETED_LIST; 
	}
}

// AUXILIARIES
bool Exist(skip_list sl) {
	if(sl.nElems != -1) {
		return true; 
	} else {
		return false; 
	}
} 

// Create value head, tail and position directions
int createNewUpperLayer(skip_list *sl) {
	skip_node *tmp, *tmp2; 

	tmp = sl->head; 
	tmp2 = sl->tail; 

	// Create new head and tail 
	sl->head->up = (skip_node*)malloc(sizeof(skip_node)); 
	sl->tail->up = (skip_node*)malloc(sizeof(skip_node)); 

	if((sl->head->up != NULL) && (sl->tail->up != NULL)) {	
		// Position head and tail in correct directions  
		sl->head->up->level = sl->head->level + 1; 
		sl->tail->up->level = sl->tail->level + 1; 

		sl->head = sl->head->up;  
		sl->tail = sl->tail->up; 

		sl->head->key = INFINITE_NEGATIVE; 
		sl->tail->key = INFINITE_POSITIVE;

		sl->head->next = sl->tail; 
		sl->tail->prev = sl->head; 

		sl->head->down = tmp; 
		sl->tail->down = tmp2; 

		sl->head->up = NULL; 
		sl->tail->up = NULL; 

		return SUCCESS; 
	} else {
		return ERROR_CREATION_CAPA_SUPERIOR; 
	}
}

// Delete head and tail not required 
int deleteUpperLayer(skip_list *sl) {  
	sl->head = sl->head->down;  
	sl->tail = sl->tail->down; 

	free(sl->head->up); 
	free(sl->tail->up); 

	sl->head->up = NULL; 
	sl->tail->up = NULL;

	return SUCCESS; 
}
 
// Delete one element from list 
int deleteOneElement(skip_list *sl, int elem) {
	skip_node *tmp, *tmp2; 
	bool found, end; 

	tmp = sl->head; 
	found = false; 

	// If is equal 
	while(!found && elem >= tmp->key) {
		if(elem == tmp->key) {
			found = true; 
		} else {
			// Si elem is smaller and different from level 0 
			if(tmp->next->key > elem && tmp->level != 0) {
				tmp = tmp->down;
			} else { 
				// If elem is bigger 
				tmp = tmp->next; 
			} 
		}
	}

	if(found) {
		// Deletes whole column 
		end = false; 
		do {
			if((tmp->prev->key == INFINITE_NEGATIVE) &&
					(tmp->next->key == INFINITE_POSITIVE) && 
					(tmp->level > 0)) {

				deleteUpperLayer(&(*sl)); 
			} else {
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;   
			}

			tmp2 = tmp; 
			if(tmp->level == 0) {
				end = true; 
			} else {
				tmp = tmp->down; 
			}

			// Free node 
			free(tmp2);   
		} while(!end && tmp != NULL); 

		sl->nElems--;

		return SUCCESS;
	} else {
		return ELEMENT_NOT_FOUND; 
	} 	
}
