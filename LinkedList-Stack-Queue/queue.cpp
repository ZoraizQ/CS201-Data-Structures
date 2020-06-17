#ifndef __QUEUE_CPP
#define __QUEUE_CPP
#include "queue.h"

template <class T>
Queue<T>::Queue()
{
}

template <class T>
Queue<T>::Queue(Queue<T>& otherQueue)
{
    ListItem<T> *otherHead = otherQueue.list.getHead();
    while(otherHead != NULL){
        list.insertAtTail(otherHead->value);
        otherHead = otherHead->next;
    }
}

template <class T>
Queue<T>::~Queue()
{
}

template <class T>
void Queue<T>::enqueue(T item)
{
    list.insertAtHead(item);
}

template <class T>
T Queue<T>::front()
{
    ListItem<T> *top_node = list.getTail();
    if (top_node != NULL){
    return top_node->value;
    }
}

template <class T>
T Queue<T>::dequeue()
{
    ListItem<T> *dequeued_node = list.getTail();
    if (dequeued_node != NULL){
    T dequeued_item = dequeued_node->value;
    list.deleteTail();
    return dequeued_item;
    }
}

template <class T>
int Queue<T>::length()
{
    return list.length();
}

template <class T>
bool Queue<T>::isEmpty()
{
    if (list.getHead() == NULL){
        return true;
    }
    else{
        return false;
    }
}


#endif
