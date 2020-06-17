#ifndef __LIST_CPP
#define __LIST_CPP

#include <cstdlib>
#include "LinkedList.h"

template <class T>
LinkedList<T>::LinkedList()
{
    head = NULL;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& otherList)
{
    head = NULL;
    ListItem<T> *otherHead = otherList.head;
    while (otherHead != NULL){
        insertAtTail(otherHead->value);
        otherHead=otherHead->next;
    }
}

template <class T>
LinkedList<T>::~LinkedList()
{
// Destructor, delete all nodes here
while(head != NULL){
	deleteHead();
}
}

template <class T>
void LinkedList<T>::insertAtHead(T item)
{
ListItem<T> *new_node = new ListItem<T>(item);
	if (head == NULL){ // empty linked list
		head = new_node;
		new_node->next = NULL; // also by default
		new_node->prev = NULL;
	}
	else{ // atleast 1 node present
		head->prev = new_node;
		new_node->next = head;
		new_node->prev = NULL;
		head = new_node;
	}
}

template <class T>
void LinkedList<T>::insertAtTail(T item)
{
ListItem<T> *new_node = new ListItem<T>(item);
	if (head == NULL){ // empty linked list
		head = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	}
	else{
		ListItem<T> *tail_node = getTail(); 	// tail_node will now point to the tail node
		tail_node->next = new_node;
		new_node->prev = tail_node;
		new_node->next = NULL;
	}
}

template <class T>
void LinkedList<T>::insertAfter(T toInsert, T afterWhat)
{
    ListItem<T> *what = searchFor(afterWhat);
	if (what == NULL){ // did not find or empty list
		return;
	}
	else{
        ListItem<T> *new_node = new ListItem<T>(toInsert);
        if (what->next == NULL){ // tail node
            new_node->next = NULL;
        }
        else{
            new_node->next = what->next;
            what->next->prev = new_node;
        }
        new_node->prev = what;
        what->next = new_node;
	}


}


template <class T>
ListItem<T>* LinkedList<T>::getHead()
{
	return head;
}

template <class T>
ListItem<T>* LinkedList<T>::getTail()
{
	ListItem<T> * temp = head;

	if (head == NULL){
		return NULL;
	}

	while (temp->next != NULL){ //traverse to end of the linked list (last node)
		temp = temp->next;
	}
	return temp;
}

template <class T>
ListItem<T> *LinkedList<T>::searchFor(T item)
{
	ListItem<T> *temp = head;

	if (head == NULL){ // empty list
		return NULL;
	}

	while(temp != NULL){
		if(temp->value == item){
			break;
		}
		temp = temp->next;
	}

	return temp; // returns NULL if not found
}

template <class T>
void LinkedList<T>::deleteElement(T item)
{
	ListItem<T> *del_node = searchFor(item);
	if (del_node == NULL){
		return;
	}
	else {
    if (del_node == head){ //del_node is the head
        if (del_node->next == NULL){ // only node present
            head = NULL;
        }
        else{
            del_node->next->prev = NULL;
            head = head->next;
        }
    }
    else if (del_node->next == NULL){ //del_node is the tail
        del_node->prev->next = NULL;
	}
	else{ //any internal node
        del_node->next->prev = del_node->prev;
        del_node->prev->next = del_node->next;
	}
	delete del_node;
	}
}

template <class T>
void LinkedList<T>::deleteHead()
{
	if (head == NULL){
		//std::cout << "List is empty already.\n";
		return;
	}
	else{
		ListItem<T> *old_head = head;
		if (head->next == NULL){ //only node present
            head = NULL;
		}
		else{
            head->next->prev = NULL;
            head = head->next;
		}
		delete old_head;
	}
}

template <class T>
void LinkedList<T>::deleteTail()
{
    ListItem<T> *tail_node = getTail();
	if (tail_node == NULL){
		return;
	}
	else{
        if(tail_node->prev == NULL){ // tail_node is the head (only node present)
            head = NULL;
        }
        else{
         tail_node->prev->next = NULL;
        }
	delete tail_node;
	}
}

template <class T>
int LinkedList<T>::length()
{
	if (head == NULL){
		return 0;
	}
	else{
		int len = 0;
		ListItem<T> * temp = head;
		while (temp != NULL){ //traverse to end of the linked list (last node)
			temp = temp->next;
			len++;
		}
		return len;
	}
}

#endif
