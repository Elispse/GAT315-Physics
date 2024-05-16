#include "collision.h"
#include "contact.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(ekBody* body1, ekBody* body2)
{
	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;

	return (distance <= radius);
}

void CreateContacts(ekBody* bodies, ekContact_t** contacts)
{
	for (ekBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ekBody* body2 = body1->next; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

			if (Intersects(body1, body2))
			{
				ekContact_t* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}

ekContact_t* GenerateContact(ekBody* body1, ekBody* body2)
{
	ekContact_t* contact = (ekContact_t*)malloc(sizeof(ekContact_t));
	assert(contact);

	memset(contact, 0, sizeof(ekContact_t));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}

void SeparateContacts(ekContact_t* contacts)
{
	// Iterate through each contact in the array
	for (ekContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the total inverse mass of the bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the separation vector using the contact's normal and depth,
		// scaled by the total inverse mass
		Vector2 separation = Vector2Scale(contact->normal, contact->depth / totalInverseMass);

		// Update the position of the first body by adding the scaled separation vector,
		// scaled by the first body's inverse mass
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));

		// Update the position of the second body by adding the scaled separation vector,
		// scaled by the negative of the second body's inverse mass
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(separation, -contact->body2->inverseMass));
	}
}

void ResolveContacts(ekContact_t* contacts)
{
	// Iterate through each contact in the array
	for (ekContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the relative velocity between the bodies involved in the contact
		Vector2 relativeVelocity = Vector2Subtract(contact->body1->velocity, contact->body2->velocity);

		// Calculate the velocity along the normal direction
		float normalVelocity = Vector2DotProduct(relativeVelocity, contact->normal);

		// If the normal velocity is positive (bodies are moving apart), skip this contact
		if (normalVelocity > 0) continue;

		// Calculate the total inverse mass of the bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the magnitude of the impulse
		float impulseMagnitude = (-(1 + contact->restitution) * normalVelocity / totalInverseMass);

		// Calculate the impulse vector along the normal direction
		Vector2 impulseVector = Vector2Scale(contact->normal, impulseMagnitude);

		// Apply the impulse to the first body
		ApplyForce(contact->body1, impulseVector, FM_IMPULSE);

		// Apply the opposite impulse to the second body
		ApplyForce(contact->body2, Vector2Negate(impulseVector), FM_IMPULSE);
	}
}