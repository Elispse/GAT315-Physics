#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void AddContact(ekContact_t* contact, ekContact_t** contacts)
{
	assert(contact);

	// add element to linked list
	contact->next = *contacts;

	// set head of elements to new element
	*contacts = contact;
}

void DestroyAllContacts(ekContact_t** contacts)
{
	if (!*contacts) return;

	ekContact_t* contact = *contacts;
	while (contact)
	{
		ekContact_t* next = contact->next;
		free(contact);
		contact = next;
	}

	*contacts = NULL;
}