#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>

#define SUCCESS 0
#define ERROR_CREATION 1 
#define ERROR_DESTROY 2
#define NOT_DELETED_LIST 3
#define EMPTY_LIST 4
#define ELEMENT_NOT_FOUND 6
#define ERROR_WALK 7
#define ERROR_CREATION_CAPA_SUPERIOR 8

#define INFINIT_POSITIVE INT_MAX
#define INFINIT_NEGATIVE INT_MIN

// Used for showing elements in test
#define INDETERMINATE (INT_MAX-1)

typedef struct skip_node_t {
	int key, level; 
	struct skip_node_t *prev, *next; 
	struct skip_node_t *down, *up; 
} skip_node; 

typedef struct skip_list_t {
	skip_node *head, *tail; 
	int nElems;
} skip_list;

int Create(skip_list *sl);
int Destroy(skip_list *sl);
int Insert(skip_list *sl, int elem);
int Delete(skip_list *sl, int elem);
int Length(skip_list sl, int *length);
int Search(skip_list sl, int elem, bool *found);
int Cost_Search(skip_list sl, int elem, int *cost);

// Funcions auxiliars
bool Exist(skip_list sl); 
int createNewUpperLayer(skip_list *sl); 
int deleteUpperLayer(skip_list *sl); 
int deleteOneElement(skip_list *sl, int elem); 


