#include <stdlib.h>
#include <assert.h>
#include <world.h>
#include <string.h>

ekBody* ekBodies = NULL;
int ekBodyCount = 0;

ekBody* CreateBody()
{
	//Allocate memory for new ekBody
	ekBody* newBody = (ekBody*)malloc(sizeof(ekBody));
	//Check if allocation is successful
	assert(newBody);

	memset(newBody, 0, sizeof(ekBody));
	//Initialize 'prev' to NULL and 'next' to the head of the list
	newBody->prev = NULL;
	newBody->next = ekBodies;
	//If list is not empty, update 'prev' of existing head
	if (ekBodies != NULL)
	{
		ekBodies->prev = newBody;
	}
	//Update head of the list to new ekBody
	ekBodies = newBody;
	//Increment body count
	ekBodyCount++;
	//Return new ekBody
	return newBody;
}

void DestroyBody(ekBody* body)
{
	//Assert if provided ekBody is not NULL
	assert(body != NULL);
	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (body->prev != NULL)
	{
		body->prev->next = body->next;
	}
	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (body->next != NULL)
	{
		body->next->prev = body->prev;
	}
	//If body is the head, update head to 'body->next'
	if (body == ekBodies)
	{
		ekBodies = body->next;
	}
	//Decrement body count
	ekBodyCount--;
	//Free the body
	free(body);
}

void DestroyAllBodies()
{

}