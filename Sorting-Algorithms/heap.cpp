#ifndef __HEAP_CPP
#define __HEAP_CPP
#include "heap.h"

MinHeap::MinHeap(int cap)
{ //O(nlogn) time for heap sort
    capacity = cap; //assign given capacity
    heap_size = 0; //initially empty heap
    harr = new int[capacity]; //initialize heap array with new max capacity given
}

void MinHeap::MinHeapify(int i) //O(logn)
{
    //recursive code
    int right_child = right(i);
    int left_child = left(i);
    int least_index = i;

    if (right_child < heap_size){ //if right child exists within bounds of heap
        if (harr[right_child] < harr[i]) //if its key's value is less than current node's
            least_index = right_child;
    }

    if (left_child < heap_size){ //if it exists
        if (harr[left_child] < harr[least_index]) //if its value is less than the current least value
            least_index = left_child;
    }

    if (least_index == i) //if the least index remains the same (current node i index), that is the current node is the minimum of the three still (i, 2i+1, 2i+2)
        return; //then like a continue, return control, prevent another recursive call, no need for swapping since heap order maintained

    //heap property disturbed here so
    //swap the least index value with the old current index i's value since a lesser value was found than i
    int temp = harr[i];
    harr[i] = harr[least_index];
    harr[least_index] = temp;

    //check the nodes next onwards from the least_index for the heap property fixes
    MinHeapify(least_index);
}

int MinHeap::parent(int i)
{
    return (i-1)/2; //parent index
}

int MinHeap::left(int i)
{
    return 2*i+1; //left child index
}

int MinHeap::right(int i)
{
    return 2*i+2; //right child index
}

int MinHeap::extractMin() //O(logn), also returns min after deleting it
{
    //since the min (root) will be removed, heap property disturbed, so to fix that O(logn) time needed
    int old_minVal = getMin(); //minimum value == root
    harr[0] = harr[heap_size-1]; //when the root is deleted it is swapped with last node
    heap_size--; //one node deleted (it remains in the array, but ignored)
    MinHeapify(0); //fix the heap property from the min node (root) onwards

    return old_minVal;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    int curr_index = i;
    harr[curr_index] = new_val; //key's value at index i decreased to new_val (being lesser)

    while(curr_index != 0){ //until the root index (0) has been reached
        if (harr[curr_index] > harr[parent(curr_index)]) //if the heap property is obeyed (parent's key is less than current child's), break and do not check further
            break;

        //swap parent's key with current child node's key, to fix the heap property
        int temp = harr[curr_index];
        harr[curr_index] = harr[parent(curr_index)];
        harr[parent(curr_index)] = temp;

        //update curr_index to the parent's so we can traverse the described route upwards
        curr_index = parent(curr_index);
    }
}

int MinHeap::getMin() //O(1)
{
    //root element will be at harr[0], the key at the root is the minimum
    return harr[0];
}

void MinHeap::deleteKey(int i) //O(logn)
{
    if (i < heap_size){ //error handling in case some one gives index outside heap
    decreaseKey(i, INT_MIN);
    extractMin();
    }
}

void MinHeap::insertKey(int k) //O(logn)
{
    //new key inserted after last node in heap (end), may disturb heap property
    if (heap_size == capacity){ //if the array is already full, max cap
        cout << "Max size reached.\n";
        return;
    }
    //to insert, simply place the key at the last index which is equal to the old heap_size
    int curr_index = heap_size;
    harr[curr_index] = k;
    heap_size++; //increase in heap size

    //tracing the route upwards from the current key index to the root node (curr_index = 0) using parent, fix the heap property
    //starts from curr_index being the index of the key that was just inserted
    while(curr_index != 0){ //until the root index (0) has been reached
        if (harr[curr_index] > harr[parent(curr_index)]) //if the heap property is obeyed (parent's key is less than current child's), break and do not check further
            break;

        //swap parent's key with current child node's key, to fix the heap property
        int temp = harr[curr_index];
        harr[curr_index] = harr[parent(curr_index)];
        harr[parent(curr_index)] = temp;

        //update curr_index to the parent's so we can traverse the described route upwards
        curr_index = parent(curr_index);
    }
}

int* MinHeap::getHeap()
{
	return harr;
}

#endif
