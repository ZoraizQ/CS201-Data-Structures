#ifndef __SORTS_CPP
#define __SORTS_CPP
#include "sorts.h"
#include "heap.cpp"
#include <algorithm>
#include "list.cpp"

//=====================================================================================
vector<long> InsertionSort(vector<long> nums) //in place, O(n^2), worst-case when elements are in reverse order, and O(n) when sorted vector given
{
    int n = nums.size();
    long nums_arr[n]; //array created of the size of the given vector
    copy(nums.begin(), nums.end(), nums_arr);

    int curr_pos;
    for (int x = 1; x < n; x++) {
        curr_pos = x-1;
        int curr_element = nums_arr[x];
        while (nums_arr[curr_pos] > curr_element) { //shift all elements greater than key one index ahead in the array
            if (curr_pos < 0)
                break;
            nums_arr[curr_pos+1] = nums_arr[curr_pos];
            curr_pos--;
        }
        nums_arr[curr_pos+1] = curr_element;
    }

    //nums remain the same, so same length of vector as array still, just copy the entire array
    copy(nums_arr, nums_arr+n, nums.begin());
    return nums;
}

//=====================================================================================
/*
List<long> merger(ListItem<long>* a_head, ListItem<long>* b_head){
    List<long> merged;
    if (a_head == NULL){
        merged.insertAtHead(b_head->value);
        return merged;
    }
    else if (b_head == NULL){
        merged.insertAtHead(a_head->value);
        return merged;
    }

    if (a_head->value <= b_head->value){
        merged.insertAtHead(a_head->value);
        a_head = a_head->next;
    }
    else{
        merged.insertAtHead(b_head->value);
        b_head = b_head->next;
    }

    ListItem<long>* h = merged.getHead();
    while(b_head!= NULL && a_head!= NULL){
        if (a_head->value <= b_head->value){
            ListItem<long> *temp = new ListItem<long>(a_head->value);
            temp->next = NULL;
            temp->prev = h;
            h->next = temp;
            h = h->next;
            a_head = a_head->next;
        }
        else{
            ListItem<long> *temp = new ListItem<long>(b_head->value);
            temp->next = NULL;
            temp->prev = h;
            h->next = temp;
            h = h->next;
            b_head = b_head->next;
        }
    }

    //insert remaining elements of b
    while (b_head != NULL){
        ListItem<long> *temp = new ListItem<long>(b_head->value);
        temp->next = NULL;
        temp->prev = h;
        h->next = temp;
        h = h->next;
        b_head = b_head->next; //reverse insertion
    }
    //insert remaining elements of a
    while (a_head != NULL){
        ListItem<long> *temp = new ListItem<long>(a_head->value);
        temp->next = NULL;
        temp->prev = h;
        h->next = temp;
        h = h->next;
        a_head = a_head->next;
    }
    return merged;
}

List<long> recursiveMSHelper(List<long> nums_list){

    int n=nums_list.length();
    //length of linked list
    if (n == 1 || n == 0){
        return nums_list;
    }
    int mid = n/2; //floor by default
    int len2 = n-mid;

    ListItem<long>* a_head = nums_list.getHead();
    ListItem<long>* b_head;
    b_head = a_head;
    //a     b
    //1,2 | 3,4
    for (int i = 0; i < mid; i++)
        b_head = b_head->next;

    b_head->prev->next = NULL;
    b_head->prev = NULL;

    if (mid <= 1 && len2 <= 1){
        return merger(a_head, b_head);
    }
    else{
    List<long> nums1, nums2;
    nums1.insertAtTail(a_head->value);
    a_head = a_head->next;
    nums2.insertAtTail(b_head->value);
    b_head = b_head->next;
    ListItem<long>* h1 = nums1.getHead();
    ListItem<long>* h2 = nums2.getHead();

    for (int i = 1; i < mid; i++){
    ListItem<long> *temp = new ListItem<long>(a_head->value);
    temp->next = NULL;
    temp->prev = h1;
    h1->next = temp;
    h1 = h1->next;
    a_head = a_head->next;
    }
    for (int i = 1; i < len2; i++){
    ListItem<long> *temp = new ListItem<long>(b_head->value);
    temp->next = NULL;
    temp->prev = h2;
    h2->next = temp;
    h2 = h2->next;
    b_head = b_head->next;
    }
    return merger(recursiveMSHelper(nums1).getHead(),recursiveMSHelper(nums2).getHead());
    }
}

vector<long> MergeSort(vector<long> nums)
{
     List<long> nums_list;
     for (int i = nums.size()-1; i >= 0; i--)
        nums_list.insertAtHead(nums[i]);

     List<long> out_list = recursiveMSHelper(nums_list);
     for (int i = 0; i < nums.size(); i++){
        nums[i] = out_list.getHead()->value;
        out_list.deleteHead();
     }

     return nums;
}
*/
List<long> merger(ListItem<long>* a_head, ListItem<long>* b_head){
    List<long> merged;
    if (a_head == NULL){
        merged.insertAtHead(b_head->value);
        return merged;
    }
    else if (b_head == NULL){
        merged.insertAtHead(a_head->value);
        return merged;
    }

    if (a_head->value <= b_head->value){
        merged.insertAtHead(a_head->value);
        a_head = a_head->next;
    }
    else{
        merged.insertAtHead(b_head->value);
        b_head = b_head->next;
    }

    ListItem<long>* h = merged.getHead();
    while(b_head!= NULL && a_head!= NULL){
        if (a_head->value <= b_head->value){
            ListItem<long> *temp = new ListItem<long>(a_head->value);
            temp->next = NULL;
            temp->prev = h;
            h->next = temp;
            h = h->next;
            a_head = a_head->next;
        }
        else{
            ListItem<long> *temp = new ListItem<long>(b_head->value);
            temp->next = NULL;
            temp->prev = h;
            h->next = temp;
            h = h->next;
            b_head = b_head->next;
        }
    }

    //insert remaining elements of b
    while (b_head != NULL){
        ListItem<long> *temp = new ListItem<long>(b_head->value);
        temp->next = NULL;
        temp->prev = h;
        h->next = temp;
        h = h->next;
        b_head = b_head->next; //reverse insertion
    }
    //insert remaining elements of a
    while (a_head != NULL){
        ListItem<long> *temp = new ListItem<long>(a_head->value);
        temp->next = NULL;
        temp->prev = h;
        h->next = temp;
        h = h->next;
        a_head = a_head->next;
    }
    return merged;
}

List<long> recursiveHelper(ListItem<long>* nums_head){

    ListItem<long> *temp = nums_head;
    int n=0;
    while (temp != NULL){
        temp = temp->next;
        n++;
    }
    //length of linked list
    if (n == 1 || n == 0){
        List<long> ret_list;
        while(nums_head != NULL){
        ret_list.insertAtTail(nums_head->value);
        nums_head = nums_head->next;
        }
        return ret_list;
    }
    int mid = n/2; //floor by default
    int len2 = n-mid;

    ListItem<long>* a_head = nums_head;
    ListItem<long>* b_head;
    b_head = a_head;
    //a     b
    //1,2 | 3,4
    for (int i = 0; i < mid; i++)
        b_head = b_head->next;

    b_head->prev->next = NULL;
    b_head->prev = NULL;

    if (mid <= 1 && len2 <= 1){
        return merger(a_head, b_head);
    }
    else{
    return merger(recursiveHelper(a_head).getHead(),recursiveHelper(b_head).getHead());
    }
}

vector<long> MergeSort(vector<long> nums)
{
     List<long> nums_list;
     for (int i = nums.size()-1; i >= 0; i--)
        nums_list.insertAtHead(nums[i]);

     List<long> out_list = recursiveHelper(nums_list.getHead());
     for (int i = 0; i < nums.size(); i++){
        nums[i] = out_list.getHead()->value;
        out_list.deleteHead();
     }

     return nums;
}
//=====================================================================================
void swap_p(long *x, long *y){
    long temp = *x;
    *x = *y;
    *y = temp;
}

int first_element_partition(long nums_arr[], int l, int h){
    long p = nums_arr[l]; //first element is pivot
    int lmark = l+1, rmark = h; //two markers for leftmost side (after p which is first element), and rightmost side

    while (true){
        while(lmark <= rmark && nums_arr[rmark] >= p){ //same conditions, but rmark values must stay right of the pivot
            rmark--; //if that is the case, move on to the next element
        }
        while (lmark <= rmark && nums_arr[lmark] <= p){ //while there is still some gap between the two marks and lmark <= pivot's value, stays to the left of pivot
            lmark++; //move on to the next, this element is correctly positioned
        }

        if (rmark < lmark) //if the 2nd mark has crossed the 1st
            break; //end loop
        else{
            //swap elements at the two marker indices
            swap_p(nums_arr+rmark, nums_arr+lmark);
        }
    }

    //swap pivot value (first index l) with the rmark value finally to put it back into position where it belongs
    swap_p(nums_arr+l, nums_arr+rmark);

    return rmark;
}

int last_element_partition(long nums_arr[], int l, int h){
    long p = nums_arr[h]; //last element is pivot
    int lmark = l, rmark = h-1; //two markers for leftmost side, and rightmost side (before last element pivot)

    while (true){
        while(lmark <= rmark && nums_arr[rmark] >= p){ //same conditions, but rmark values must stay right of the pivot
            rmark--; //if that is the case, move on to the next element
        }
        while (lmark <= rmark && nums_arr[lmark] <= p){ //while there is still some gap between the two marks and lmark <= pivot's value, stays to the left of pivot
            lmark++; //move on to the next, this element is correctly positioned
        }

        if (rmark < lmark) //if the 2nd mark has crossed the 1st
            break; //end loop
        else{
            //swap elements at the two marker indices
            swap_p(nums_arr+rmark, nums_arr+lmark);
        }
    }

    //swap pivot value (last index h) with the lmark value finally to put it back into position where it belongs
    swap_p(nums_arr+h, nums_arr+lmark);
    
    return lmark; //rmark has crossed the lmark
}

int median_of_three_partition(long nums_arr[], int l, int h){
    int m = (l+h)/2; //if 0, 4 then, middle is 2, if 3,10 then middle is 6
    //median of three is pivot, which is moved to the last after all 3 are sorted

    //move selected median value from m to h (where h will be selected as the pivot)
    swap_p(nums_arr+m, nums_arr+h);

    return last_element_partition(nums_arr, l, h); 
}

void recursiveQSAHelper(long nums_arr[], long l, long h){
    if (l >= h)//low index is greater than high index, means array size == 0
        return;

    // if p is pivot, then put p where it was back in the array, with elements less than p before it and greater than p after it O(n)
    int partition_point = median_of_three_partition(nums_arr, l, h);
    recursiveQSAHelper(nums_arr, l, partition_point-1); //recurse with the left half to the side of the pivot
    recursiveQSAHelper(nums_arr, partition_point+1, h); //recurse with the right half to the side of the pivot
}

vector<long> QuickSortArray(vector<long> nums){
    //pick pivot as first element
    //as median of three
    //as last element
    int n = nums.size();
    long nums_arr[n]; //array created of the size of the given vector
    copy(nums.begin(), nums.end(), nums_arr);

    recursiveQSAHelper(nums_arr, 0, n-1); //array passed with low, high, the array points to memory so it will get sorted itself

    //nums remain the same, so same length of vector as array still, just copy the entire array
    copy(nums_arr, nums_arr+n, nums.begin());
    return nums;
}

//=====================================================================================
struct partitionBox{
    ListItem<long>* node;
    int point;
};

partitionBox last_element_partition2(ListItem<long>* leftptr, ListItem<long>* rightptr, int l, int r){
    ListItem<long>* partition_tail = rightptr;
    long p = partition_tail->value; //last element is pivot
    int lmark = l, rmark = r-1; //two markers for leftmost side, and rightmost side
    rightptr = rightptr->prev; //since r-1 init

    while (true){
        while(lmark <= rmark && rightptr->value >= p){ //same conditions, but rmark values must stay right of the pivot
            rmark--; //if that is the case, move on to the next element
            rightptr = rightptr->prev;
        }
        while (lmark <= rmark && leftptr->value <= p){ //while there is still some gap between the two marks and lmark <= pivot's value, stays to the left of pivot
            lmark++; //move on to the next, this element is correctly positioned
            leftptr = leftptr->next;
        }

        if (rmark < lmark) //if the 2nd mark has crossed the 1st
            break; //end loop
        else{
            //swap elements at the two marker indices
            swap_p(&leftptr->value, &rightptr->value);
        }
    }

    //swap pivot value (last index h) with the lmark value finally to put it back into position where it belongs
    swap_p(&partition_tail->value, &leftptr->value);
    partitionBox pb;
    pb.node = leftptr;
    pb.point = lmark;
    return pb; 
}

partitionBox random_element_partition(ListItem<long>* l_node, ListItem<long>* h_node, int l, int h){
    ListItem<long>* random_node = l_node;
    int rnd = rand() % (h-l) + l; //from l to h
//    cout << "l: " << l << " h: "<< h << " rnd: " << rnd << endl;
    for (int i = l; i < rnd; i++)
        random_node = random_node->next;

    //e.g. 2,3,5,4
    //move selected random value from rnd to h (where h will be selected as the pivot)
    swap_p(&random_node->value, &h_node->value);
    //2,4,5,3, where 3 is the random element, now at the last element

    return last_element_partition2(l_node, h_node, l, h); 
}

void recursiveQSLHelper(ListItem<long>* l_node, ListItem<long>* h_node, int l, int h){
    if (l >= h)//low index is greater than high index, means array size == 0
        return;

    //node to partition between
    partitionBox pb = random_element_partition(l_node, h_node, l, h);
    
    recursiveQSLHelper(l_node, pb.node->prev, l, pb.point-1); //recurse with the left half to the side of the pivot
    recursiveQSLHelper(pb.node->next, h_node, pb.point+1, h); //recurse with the right half to the side of the pivot
}

vector<long> QuickSortList(vector<long> nums)
{
    List<long> nums_list;
    for (int i = nums.size()-1; i >= 0; i--)
        nums_list.insertAtHead(nums[i]);

    recursiveQSLHelper(nums_list.getHead(), nums_list.getTail(), 0, nums.size()-1);
    
    for (int i = 0; i < nums.size(); i++){
        nums[i] = nums_list.getHead()->value;
        nums_list.deleteHead();
    }

    return nums;
}

//=====================================================================================
vector<long> HeapSort(vector<long> nums)
{
    int n = nums.size();
    MinHeap* h = new MinHeap(n);
    for (int i = 0; i < n; i++)
        h->insertKey(nums[i]);

    nums.clear();
     for (int i = 0; i < n; i++)
        nums.push_back(h->extractMin());

    delete h;
    return nums;
}

//=====================================================================================

long* expandArray(long* old_array, int old_size, int new_size)
{
    long* new_array = new long[new_size];
    for (int i = 0; i < old_size; i++)
        new_array[i] = old_array[i];

    for (int i = old_size; i < new_size; i++)
        new_array[i] = 0;
    
    delete[] old_array;
    return new_array;
}

vector<long> QueenOfAllSorts(vector<long> nums)
{
    // nums.clear();
    // for (int x = -10; x <= 10; x++){
    //     nums.push_back(x);
    // }
    // nums.push_back(30);
    // nums.push_back(-99);
    // nums.push_back(-100);
    // nums.push_back(3000);

    int n = nums.size();
    int k = n; //initially
    int kn = k;
    int mostPositive = 0;
    int mostNegative = 0;
    //long compfactor = 0;
    //both arrays store frequency of the number now, to handle duplicates
    long* arr = new long[k]; //array created of the size of the given vector
    long* neg_arr = new long[kn]; //for storing negative values
    
    //iniitalization
    for (int i = 0; i < k; i++){
        arr[i] = 0;
        neg_arr[i] = 0;
    }
    
    for (int i = 0; i < n; i++){
        long p = nums[i];
        //cout << p << endl;
        long abs_p = abs(p);
        //int new_neg_size = abs_p + (abs_p+1-mostNegative);
        //int new_pos_size = p + (p+1-mostNegative);
        int new_neg_size = 2*kn;
        int new_pos_size = 2*k;
 
        if (p < 0){ //negative
            if (abs_p >= kn){ //resize needed, k-1 is the max index allowed (value of element)
            neg_arr = expandArray(neg_arr, kn, new_neg_size);
            kn = new_neg_size;
            }
            neg_arr[abs_p]++;
            if (p < mostNegative){
                mostNegative = -p;
            }
        }
        else{ // positive
            if (p >= k){ //resize needed, k-1 is the max index allowed (value of element)
            arr = expandArray(arr, k, new_pos_size);
            k = new_pos_size;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             ;
            }
            arr[p]++; //frequency of this number increases
            if (p > mostPositive){
                mostPositive = p;    
            }
        }
    }

    nums.clear();
    // adding the final array values to the nums vector along with duplications
    for (int i = kn-1; i >= 0; i--){
        for (int times = 0; times < neg_arr[i]; times++){
            //cout << -i << endl;
            nums.push_back(-i);
        }
        if (-i == mostNegative){
            break;
        }
    }
    for (int i = 0; i < k; i++){
        for (int times = 0; times < arr[i]; times++){
            //cout << i << endl;
            nums.push_back(i);
        }
        if (i == mostPositive){
            break;
        }
    }

    return nums;
}

#endif
