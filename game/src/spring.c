#include "spring.h"
#include "body.h"
//#include "force.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ekSpring_t* ekSprings = NULL;

ekSpring_t* CreateSpring(struct ekBody* body1, struct ekBody* body2, float restLength, float k)
{
	ekSpring_t* spring = (ekSpring_t*)malloc(sizeof(ekSpring_t));
	assert(spring);


	memset(spring, 0, sizeof(ekSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	//Return new Body
	return spring;
}

void AddSpring(ekSpring_t* spring)
{
	assert(spring);

	// add element to linked list
	spring->prev = NULL;
	spring->next = ekSprings;

	if (ekSprings)
	{
		ekSprings->prev = spring;
	}
	//Update head of elements to new element
	ekSprings = spring;
}

void DestroySpring(ekSpring_t* spring)
{
	//Assert if provided Body is not NULL
	assert(spring);
	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (spring->prev) spring->prev->next = spring->next;
	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (spring->next) spring->next->prev = spring->prev;
	//If body is the head, update head to 'body->next'
	if (ekSprings == spring)
	{
		ekSprings = spring->next;
	}
	//Free the body
	free(spring);
}

void ApplySpringForce(ekSpring_t* springs)
{
	for (ekSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position); //<get direction vector from body2 to body1>
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength; //<compute displacement from current length to resting length>;
		float force = -spring->k * x; //<compute force using product of displacement and stiffness(k)>;

		Vector2 ndirection = Vector2Normalize(direction); //<get direction normal>

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE);
	}
}