#include "algorithms.h"
#include <QDebug>
#include "sortingalgapp.h"
#include <chrono>

Algorithms::Algorithms()
{
    qDebug() << "Algorithms";
}

/*
 * GeeksforGeeks Explanation and Definition :
 *  - Quicksort is a divide and conquer algorithm
 *  - It picks an element as pivot and partitions the given array around pivot.
 *  1. Always pick first element as pivot
 *  2. Always pick last element as pivot
 *  3. Pick a random element as pivot
 *  4. Pick median as pivot
 *
 *  partitions -> given an array and element x as pivot, sort elements < left, > right.
 *  - Linear time
*/

/* a utility function to swap elements */
void Algorithms::swap(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}

int Algorithms::partition(int arr[], int low, int high){
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high -1; j++){
        if(arr[j] < pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return (i + 1);
}

void Algorithms::quickSort(int arr[], int low, int high){
    if(low < high){ // ensure that the partition array has two or more elements
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void Algorithms::quickSort_init(signalData* data, int low, int high){
    qDebug() << "quickSort_init";

    auto start = std::chrono::high_resolution_clock::now();
    quickSort(data->data, low, high-1);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    signalData *x = new signalData(data->data);
    emit this->resultReady(duration.count(), x, high, "QuickSort");
}


/*
 * GeeksforGeeks Explanation and Definition :
 *  - a divide and conquer algorithm
 *  - divide array into subarrays sort and then merge again
 *  1. Find the middle point to divide the array into two halves
 *  2. Call mergeSort for first half
 *  3. Call mergeSort for second half
 *  4. Merge the two halves sorted in step 2 and 3
*/


void Algorithms::merge(int arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1; //size of the left subarray
    int n2 = r - m; //size of the right

    /*create temp arrays*/
    int L[n1], R[n2];

    //Copy data to the temp arrays L and R
    for(i = 0 ; i < n1; i++){
        L[i] = arr[i + l];
    }
    for(j = 0; j < n2; j++){
        R[j] = arr[j+m+1];
    }

    i = 0; // init index L subarray
    j = 0; // init index R subarray
    k = l; // init index of merged subarray

    while(i < n1 && j < n2){
        if(L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining ele in L */
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    /* Copy the remaining ele in R*/
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void Algorithms::mergeSort(int arr[], int l, int r){
    if(l < r){ //continue calling mergesort until subarray has 2 or more elements
        int m = l + (r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void Algorithms::mergeSort_init(signalData *data, int l, int r){
    qDebug() << "mergeSort";
//    std::copy(data->data,data.data + r, b );
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(data->data, l , r);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    emit this->resultReady(duration.count(), data, r, "MergeSort");
}

/*
 * Bubble Sort
 * GeeksforGeeks Explanation and Definition :
 *  - the simplest sorting algorithm
 *  - repeatedly swapping adjacent elements if they are in the wrong order
 *  - keep passing through the array until their are no more swaps
*/

void Algorithms::bubbleSort(int arr[], int n){
    int i, j;
    for(i = 0 ; i < n-1; i++){ // number of passes
        for(j = 0 ; j < n - i ; j++){ // you can shrink the scope of the array
            if(arr[j] > arr[j + 1]){
                swap(&arr[j], &arr[j+1]); //largest elements are push to the right
            }
        }
    }
}

void Algorithms::bubbleSort_init(signalData *data, int size){
    qDebug() << "mergeSort";
    auto start = std::chrono::high_resolution_clock::now();
    bubbleSort(data->data, size);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    emit this->resultReady(duration.count(), data, size, "BubbleSort");
}


/*
 * HeapSort
 * GeeksforGeeks Explanation and Definition :
 *  - Binary Heap
 *      - (Complete Binary Tree) - Every level is filled except maybe the last, all nodes are far left as possible
 *      - the value of the parent is greater or smaller than the child nodes (max heap vs min heap).
 *      - easily represented by arrays
 *      - child nodes can be find 2 * i + 1 (left), 2 * i + 2 (right)
 *  - Heap Sort Algorithm
 *      1. Build max heap from the the input data
 *      2. Largest value is root, replace with last item in heap, and reduce size of heap. Heapify root of tree
 *      3. Repeat steps while heap size > 1
 *  Heapification is performed bottom up
*/

// Building the heap
void Algorithms::heapify(int arr[], int size, int index){
    // left child exists
    int leftIndex = index * 2 + 1;
    int rightIndex = index * 2 + 2;
    if(leftIndex < size){
        heapify(arr, size, leftIndex); // left
    }
    if(rightIndex < size){
        heapify(arr, size, rightIndex); // right
    }

    if(leftIndex < size){
        // Compare and swap values
        if(arr[leftIndex] > arr[index]){
            swap(&arr[leftIndex], &arr[index]);
        }
    }
    // right child exists
    if(rightIndex < size){
        // Compare and swap values
        if(arr[rightIndex] > arr[index]){
            swap(&arr[rightIndex], &arr[index]);
        }
    }

    return;
}

void Algorithms::heapSort(int arr[], int size, int index){
    //build heap
    for(index = 0; index < size/2; index++){
        heapify(arr, size, index);
    }

    //swap last and root
    for(int j = size-1; j > 1; j--){
        swap(&arr[0], &arr[j]);
        heapify(arr, j, 0);
    }

}

void Algorithms::heapSort_init(signalData *data, int size, int index){
    qDebug() << "heapSort";
    auto start = std::chrono::high_resolution_clock::now();
    heapSort(data->data, size, index);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    emit this->resultReady(duration.count(), data, size, "HeapSort");
}

/* Linear Search (Sequential Search)
 * GeeksforGeeks Explanation and Definition :
 * A simple approach is to do linear search, i.e
    Start from the leftmost element of arr[] and one by one compare x with each element of arr[]
    If x matches with an element, return the index.
    If x doesn’t match with any of elements, return -1
*/
int Algorithms::linearSearch(int arr[], int size, int num){
    int i;
    for (i = 0 ; i < size; i++){
        if(arr[i] == num){
            return i;
        }
    }
    return -1;
}
void Algorithms::linearSearch_init(signalData *data, int size, int num){
    qDebug() << "linearSearch";
    auto start = std::chrono::high_resolution_clock::now();
    linearSearch(data->data, size, num);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    emit this->resultReady(duration.count(), data, size, "LinearSearch");
    return;
}

/*
 * Binary Search (Interval Search)
 * GeeksforGeeks Explanation and Definition :
 *  - Search through a sorted array by dividing the search interval in half
*/


int Algorithms::binarySearch(int arr[], int size, int num, int low, int high){
    if(high >= low){
        int mid = low + (high - 1)/2;
        if(arr[mid] == num){ // if equal then found
            return mid;
        }
        if(arr[mid] > num){ //if num smaller check left
            return binarySearch(arr, size, num, low, mid - 1);
        } // if num larger check right side
        return binarySearch(arr, size, num, mid + 1, high);
    }
    return -1;
}
void Algorithms::binarySearch_init(signalData * data, int size, int num){
    qDebug() << "binarySearch";
    auto start = std::chrono::high_resolution_clock::now();
    binarySearch(data->data, size, num, 0 , size);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    emit this->resultReady(duration.count(), data, size, "BinarySearch");
    return;
}
