#include "clueq.h"
cluequeue_t * clue_initialize()
{
	cluequeue_t *cq = (cluequeue_t *) malloc(sizeof(cluequeue_t));
	if (cq == NULL) return NULL;

	cq -> head = NULL;
	cq -> tail = NULL;

	return cq;

}
bool clue_insert(cluequeue_t *cq, int val)
{
	clue_t * clue = (clue_t *) malloc (sizeof(clue_t));
	if (clue == NULL) return 0;
	//Insert at tail
	if(cq -> head == NULL && cq -> tail == NULL)
	{
		//Empty List
		clue -> value = val;
		clue -> next = NULL; 
		cq -> head = clue;
		cq -> tail = clue;

		return 1;
	}
	else if(cq -> head == NULL || cq -> tail == NULL)
	{
		//Improper insertions.
		return 0;
	}
	else
	{
		//Regular insertion at tail.
		clue_t * clue = (clue_t *) malloc (sizeof(clue_t));
		if (clue == NULL) return 0;
		clue -> value = val;
		clue -> next = NULL; 

		cq -> tail -> next = clue;
		cq -> tail = clue;
		return 1;
	}
}
void clue_delete(cluequeue_t *cq)
{
	if(cq == NULL) return;
	if(cq -> head == NULL)
	{
		free(cq);
		return;
	}
	clue_t *currClue = cq -> head;
	clue_t *nextClue;
	for(;currClue != NULL; currClue = nextClue)
	{
		nextClue = currClue -> next;
		free(currClue);
	}
	free(cq);
}
