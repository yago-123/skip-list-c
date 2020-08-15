#include"skip_list.h"

int Crear(skip_list *sl) {
	srand(time(0)); 
	sl->nElems = 0; 

	sl->head = (skip_node*)malloc(sizeof(skip_node)); 
	sl->head->level = -1; 

	sl->tail = (skip_node*)malloc(sizeof(skip_node)); 
	sl->tail->level = -1; 

	// Crea primer nivell valid: 0 
	if(creaNovaCapaSuperior(&(*sl)) == SUCCESS) {
		return SUCCESS; 
	} else {
		return ERROR_CREAR; 
	}
}

int Destruir(skip_list *sl) {
	bool fi; 
	int i, j, maxLevel;
	skip_node *tmp, *tmp2, *tmp3; 

	if(Existeix(*sl)) {
		maxLevel = sl->head->level+1; 
		// Agafa head inferior
		tmp = sl->head; 
		while(tmp->level != 0) {
			tmp = tmp->down; 
		}

		// Recorre node 1 a 1 de baix a dalt 
		for(j = 0; j < (sl->nElems+2); j++) {
			tmp2 = tmp->next; 
			fi = false; 
			i = 0; 
			while(!fi && i < maxLevel) {
				if(tmp != NULL) {
					tmp3 = tmp; 
					tmp = tmp->up;
					// Allibera node 
					free(tmp3);  
				} else {
					fi = true; 
				} 
				i++; 
			} 

			tmp = tmp2; 
		}

		sl->nElems = -1; 

		return SUCCESS;
	} else {
		return ERROR_DESTRUIR;  
	}
}

int Inserir(skip_list *sl, int elem) {
	int i; 
	bool nouNivell; 
	skip_node *node, *tmp, *tmp2; 

	node = sl->head; 
	// Busca fins nivell 0 
	while(node->level != 0) {
		if(node->key <= elem && node->next->key <= elem) {
			node = node->next;
		} else {
			node = node->down; 	
		}
	} 

	// Busca posicio dins nivell 0 
	while(elem > node->key) {
		node = node->next; 
	}

	// Insereix element
	tmp = (skip_node*)malloc(sizeof(skip_node)); 
	tmp->key = elem; 
	tmp->level = 0;
	tmp->up = NULL;  
	tmp->down = NULL; 

	tmp->next = node; 
	tmp->prev = node->prev;
	node->prev->next = tmp;
	node->prev = tmp;

	// Posiciona node darrere tmp 
	node = tmp->prev;  

	// Insereix columna (coin flip) 
	i = 1;  
	while((rand()%2) == 0) {
		nouNivell = false; 
		// Busca primer previ amb nivell superior
		while(node->up == NULL && !nouNivell) {
			if((node->key == INFINIT_NEGATIU) && 
					(node->level == sl->head->level)) {
				nouNivell = true; 
			} else {
				node = node->prev; 
			}
		}

		// Crea nova capa 
		if(nouNivell) {
			creaNovaCapaSuperior(&(*sl)); 
			node = sl->head; 
		} else {
			// Mou node superior
			node = node->up;
		} 

		// Posiciona tmp a la capa superior
		tmp->up = (skip_node*)malloc(sizeof(skip_node)); 
		tmp->up->key = elem; 
		tmp->up->level = i;

		// Reestableix direccions
		tmp2 = tmp;  
		tmp = tmp->up;
		tmp->down = tmp2;  

		tmp->prev = node; 
		tmp->next = node->next;

		node->next->prev = tmp; 
		node->next = tmp; 

		i++;   
	}

	// Augmenta elements i posa fi a l'element
	tmp->up = NULL;  
	sl->nElems++; 

	return 0;
}

int Esborrar(skip_list *sl, int elem) {
	bool present; 

	if(Existeix(*sl)) {
		// Elimina tots els nodes amb aquest elem
		Buscar((*sl), elem, &present);
		while(present) { 
			// Elimina element amb valor elem
			eliminaUnElement(&(*sl), elem);
			Buscar((*sl), elem, &present); 
		} 

		return SUCCESS; 
	} else {
		return LLISTA_NO_CREADA; 
	}
}

int Longitud(skip_list sl, int *lon) {
	if(Existeix(sl)) {
		*lon = sl.nElems; 
		return SUCCESS; 
	} else {
		return LLISTA_NO_CREADA; 
	} 
}

int Buscar(skip_list sl, int elem, bool *trobat) {
	skip_node *tmp; 

	tmp = sl.head; 
	*trobat = false; 

	if(Existeix(sl)) {
		while(!(*trobat) && elem >= tmp->key) {
			// Si es igual 
			if(elem == tmp->key) {
				*trobat = true; 
			} else {
				// Si elem es mes petit i diferent de nivell 0
				if(tmp->next->key > elem && tmp->level != 0) {
					tmp = tmp->down; 	
				} else {
					// Si elem es mes gran 
					tmp = tmp->next; 
				} 
			}
		}

		return SUCCESS; 
	} else {
		return LLISTA_NO_CREADA; 
	}
}

int Cost_Buscar(skip_list sl, int elem, int *cost) {
	skip_node *tmp; 
	bool trobat; 

	tmp = sl.head; 
	trobat = false; 

	*cost = 0; 
	if(Existeix(sl)) {
		// Si es igual 
		while(!trobat && elem >= tmp->key) {
			if(elem == tmp->key) {
				trobat = true; 
			} else {
				// Si elem es mes petit i diferent de nivell 0 
				if(tmp->next->key > elem && tmp->level != 0) {
					tmp = tmp->down;
				} else {
					// Si elem es mes gran 
					tmp = tmp->next; 
				} 

				(*cost)++; 
			}
		}

		if(trobat) {
			return SUCCESS;
		} else {
			return ELEMENT_NO_TROBAT; 
		} 
	} else {
		return LLISTA_NO_CREADA; 
	}
}

// AUXILIARS
bool Existeix(skip_list sl) {
	if(sl.nElems != -1) {
		return true; 
	} else {
		return false; 
	}
} 

// Crea valors head i tail i posiciona direccions 
int creaNovaCapaSuperior(skip_list *sl) {
	skip_node *tmp, *tmp2; 

	tmp = sl->head; 
	tmp2 = sl->tail; 

	// Crea nou head i tail 
	sl->head->up = (skip_node*)malloc(sizeof(skip_node)); 
	sl->tail->up = (skip_node*)malloc(sizeof(skip_node)); 

	if((sl->head->up != NULL) && (sl->tail->up != NULL)) {	
		// Posiciona head i tail a les posicions correctes 
		sl->head->up->level = sl->head->level + 1; 
		sl->tail->up->level = sl->tail->level + 1; 

		sl->head = sl->head->up;  
		sl->tail = sl->tail->up; 

		sl->head->key = INFINIT_NEGATIU; 
		sl->tail->key = INFINIT_POSITIU;

		sl->head->next = sl->tail; 
		sl->tail->prev = sl->head; 

		sl->head->down = tmp; 
		sl->tail->down = tmp2; 

		sl->head->up = NULL; 
		sl->tail->up = NULL; 

		return SUCCESS; 
	} else {
		return ERROR_CREAR_CAPA_SUPERIOR; 
	}
}

// Elimina head i tail no necesaris
int eliminaCapaSuperior(skip_list *sl) {  
	// Elimina head i tail sobrant 
	sl->head = sl->head->down;  
	sl->tail = sl->tail->down; 

	free(sl->head->up); 
	free(sl->tail->up); 

	sl->head->up = NULL; 
	sl->tail->up = NULL;

	return SUCCESS; 
}

// Elimina un element de la llista 
int eliminaUnElement(skip_list *sl, int elem) {
	skip_node *tmp, *tmp2; 
	bool trobat, fi; 

	tmp = sl->head; 
	trobat = false; 

	// Si es igual 
	while(!trobat && elem >= tmp->key) {
		if(elem == tmp->key) {
			trobat = true; 
		} else {
			// Si elem es mes petit i diferent de nivell 0 
			if(tmp->next->key > elem && tmp->level != 0) {
				tmp = tmp->down;
			} else {
				// Si elem es mes gran 
				tmp = tmp->next; 
			} 
		}
	}

	if(trobat) {
		// Procedeix a eliminar columna sencera 
		fi = false; 
		do {
			if((tmp->prev->key == INFINIT_NEGATIU) &&
					(tmp->next->key == INFINIT_POSITIU) && 
					(tmp->level > 0)) {

				eliminaCapaSuperior(&(*sl)); 
			} else {
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;   
			}

			tmp2 = tmp; 
			if(tmp->level == 0) {
				fi = true; 
			} else {
				tmp = tmp->down; 
			}

			// Allibera node 
			free(tmp2);   
		} while(!fi && tmp != NULL); 

		sl->nElems--;

		return SUCCESS;
	} else {
		return ELEMENT_NO_TROBAT; 
	} 	
}
