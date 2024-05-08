#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "world.h"
#include "body.h"

ekBody* ekBodies = NULL;
int ekBodyCount = 0;
Vector2 ekGravity;

ekBody* CreateBody() {
    // make space for body
    ekBody* body = (ekBody*)malloc(sizeof(ekBody));
    assert(body != NULL);

    memset(body, 0, sizeof(ekBody));

    body->prev = NULL;
    body->next = ekBodies;

    // If the list isn't empty, change previous pointer to current head
    if (ekBodies != NULL) {
        ekBodies->prev = body;
    }

    // Update head to the new body
    ekBodies = body;

    ekBodyCount++;

    return body;
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