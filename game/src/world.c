#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "world.h"
#include "body.h"

ekBody* ekBodies = NULL;
int ekBodyCount = 0;
Vector2 ekGravity;

ekBody* CreateBody(Vector2 position, float mass, ekBodyType bodyType) {
    ekBody* body = (ekBody*)malloc(sizeof(ekBody));
    assert(body != NULL);

    memset(body, 0, sizeof(ekBody));
    body->position = position;
    body->mass = mass;
    body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
    body->type = bodyType;

    return body;
}

void AddBody(ekBody* body)
{
    assert(body);
    // add element to linked list
    body->prev = NULL;
    body->next = ekBodies;

    // If the list isn't empty, change previous pointer to current head
    if (ekBodies != NULL) {
        ekBodies->prev = body;
    }

    // Update head of elements to new element
    ekBodies = body;

    ekBodyCount++;
}

void DestroyBody(ekBody* body) {
    assert(body != NULL); // check if pointer is not NULL

    // if next pointer exists 
    if (body->prev != NULL) {
        body->prev->next = body->next;//changes to next pointer of previous body
    }

    // if previous pointer exists 
    if (body->next != NULL) {
        body->next->prev = body->prev;//changes to previous pointer of next body
    }

    // If body is the head, update
    if (body == ekBodies)
    {
        ekBodies = body->next;
    }

    ekBodyCount--;

    // free memory
    free(body);
}

void DestroyAllBodies()
{
    ekBody* currentBody = ekBodies;
    ekBody* nextBody = NULL;

    while (currentBody != NULL)
    {
        nextBody = currentBody->next;
        free(currentBody);
        currentBody = nextBody;
    }

    ekBodies = NULL;
    ekBodyCount = 0;
}
