#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>

#define SUCCESS 0
#define ERROR_CREAR 1 
#define ERROR_DESTRUIR 2
#define LLISTA_NO_CREADA 3
#define LLISTA_BUIDA 4
#define ELEMENT_NO_TROBAT 6
#define ERROR_RECORRER 7
#define ERROR_CREAR_CAPA_SUPERIOR 8

#define INFINIT_POSITIU INT_MAX
#define INFINIT_NEGATIU INT_MIN

// Necesari per mostrar elements test
#define INDETERMINAT (INT_MAX-1)

typedef struct skip_node_t {
	int key, level; 
	struct skip_node_t *prev, *next; 
	struct skip_node_t *down, *up; 
} skip_node; 

typedef struct skip_list_t {
	skip_node *head, *tail; 
	int nElems;
} skip_list;

int Crear(skip_list *sl);
int Destruir(skip_list *sl);
int Inserir(skip_list *sl, int elem);
int Esborrar(skip_list *sl, int elem);
int Longitud(skip_list sl, int *lon);
int Buscar(skip_list sl, int elem, bool *trobat);
int Cost_Buscar(skip_list sl, int elem, int *cost);

// Funcions auxiliars
bool Existeix(skip_list sl); 
int creaNovaCapaSuperior(skip_list *sl); 
int eliminaCapaSuperior(skip_list *sl); 
int eliminaUnElement(skip_list *sl, int elem); 


