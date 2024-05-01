#include <stdlib.h>
#include <assert.h>
#include <world.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	//Allocate memory for new Body
	Body* newBody = (Body*)malloc(sizeof(Body));
	//Check if allocation is successful
	assert(newBody);
	//Initialize 'prev' to NULL and 'next' to the head of the list
	newBody->prev = NULL;
	newBody->next = bodies;
	//If list is not empty, update 'prev' of existing head
	if (bodies != NULL)
	{
		bodies->prev = newBody;
	}
	//Update head of the list to new Body
	bodies = newBody;
	//Increment body count
	bodyCount++;
	//Return new Body
	return newBody;
}

void DestroyBody(Body* body)
{
	//Assert if provided Body is not NULL
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
	if (body == bodies)
	{
		bodies = body->next;
	}
	//Decrement body count
	bodyCount--;
	//Free the body
	free(body);
}