#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>

using namespace std;

ofstream out;

int n; // size of array
int * arr; // array

void readFile(string Filename);
void displayArr();
void outputToFile();
void * mergeSort(void * arg);

int main()
{
    out.open("output.txt");
    readFile("merge.txt");
    int dim[2]; // paramters that will be passed to mergesort(indices of 1st and last elements)
    dim[0] = 0;
    dim[1] = n - 1;
    mergeSort(&dim);
    displayArr();
    outputToFile();
    out.close();
    return 0;
}

void displayArr(){// function to print the array to terminal
    for(int i =0 ;i < n; i++){
        cout << arr[i];
        cout << " ";
    }
    cout << "\n";
}

void outputToFile(){//function to print the array to output file
    for(int i=0 ; i<n ; i++){
            out << arr[i];
            out << " ";
        }
    out << "\nEND\n";
}

void readFile(string Filename){//function that read teh array from input file
    ifstream myfile (Filename);
    string line;
    int i;
    if (myfile.is_open()){
        myfile >> n;
        arr= (int *)malloc(n* sizeof(int ));//allocate memory for the array
        for( i=0; i< n ; i++){//read array elemnt by elemnt
            myfile >> arr[i];

        }
    myfile.close();
    }

  else cout << "Unable to open file";
}

void subarrMerge(int l, int m, int r){//merge the sorted subbarrays
    int n1 = m - l + 1; // number of elements of left subarray
    int n2 =  r - m; // number of elemnts of second subarray

    // create temp arrays
    int left[n1];
    int right[n2];

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++){ // First subarray is arr[l..m]
        left[i] = arr[l + i];
        }
    for (int j = 0; j < n2; j++){ // Second subarray is arr[m+1..r]
        right[j] = arr[m + 1+ j];
        }

    // Merge the temp arrays back into arr[l..r]
    int i = 0;
    int j = 0;
    int k = l; // Initial index of merged subarray

    //loop over the 2 subarray and compare their elemnts and merge in a sorted fashion
    while (i < n1 && j < n2){//while both temp arrays still got elements
        if (left[i] <= right[j]){
            arr[k] = left[i++];
        }
        else{
            arr[k] = right[j++];
        }
        k++;
    }

    // Copy the remaining elements of left
    while (i < n1) {
        arr[k++] = left[i++];
    }

    // Copy the remaining elements of right
    while (j < n2){
        arr[k++] = right[j++];
    }
}

// l is for left index and r is right index of the
// sub-array of arr to be sorted
void * mergeSort(void * arg){
    int * dim = (int *)arg;
    int l = dim[0];//left
    int r = dim[1];//right
    if (l < r){
        int m = l+(r-l)/2; // middle
        int leftDim[2];//paramters of 1st half
        leftDim[0] = l;
        leftDim[1] = m;
        int rightDim[2];//paramters of 2nd half
        rightDim[0] = m + 1;
        rightDim[1] = r;

        pthread_t threadL;//left thread
        pthread_t threadR;//right thread

        // Sort first half
        if(pthread_create(&threadL, NULL, mergeSort, &leftDim )){
            perror("Error in creating threads.");
            }

        // Sort second half
        if(pthread_create(&threadR, NULL, mergeSort, &rightDim )){
            perror("Error in creating threads.");
            }

        pthread_join(threadL, NULL);
        pthread_join(threadR, NULL);

        subarrMerge(l, m, r);
        //pthread_exit(NULL);
    }
}
