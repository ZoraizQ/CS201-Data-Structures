#ifndef __STACK_CPP
#define __STACK_CPP
#include "stack.h"

template <class T>
Stack<T>::Stack()
{
}

template <class T>
Stack<T>::Stack(Stack<T>& otherStack)
{
    ListItem<T> *otherHead = otherStack.list.getHead();
    while(otherHead != NULL){
        list.insertAtTail(otherHead->value);
        otherHead = otherHead->next;
    }
}

template <class T>
Stack<T>::~Stack()
{
}

template <class T>
void Stack<T>::push(T item)
{
    list.insertAtHead(item);
}

template <class T>
T Stack<T>::top()
{
    ListItem<T> *top_node = list.getHead();
    if (top_node != NULL){
    return top_node->value;
    }
}

template <class T>
T Stack<T>::pop()
{
    ListItem<T> *popped_node = list.getHead();
    if (popped_node != NULL){
    T popped_item = popped_node->value;
    list.deleteHead();
    return popped_item;
    }
}

template <class T>
int Stack<T>::length()
{
    return list.length();
}

template <class T>
bool Stack<T>::isEmpty()
{
    if (list.getHead() == NULL){
        return true;
    }
    else{
        return false;
    }
}

#endif
