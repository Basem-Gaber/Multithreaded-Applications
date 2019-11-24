#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <chrono>

using namespace std;


ofstream out;

int row1, row2, col1, col2;// m x n dimensions of teh input matrices
int ** matA;//1st input matrix
int ** matB;//2nd input matrix
int ** matC;//output matrix of procedure 1
int ** matD;//output matrix of procedure 2

void readFile(string Filename);
void procedure1();
void * computeElement(void * arg);
void procedure2();
void * computeRow(void * arg);
void displayMat(int ** mat,int r,int c);
void outputToFile(int num);
int main()
{
    out.open("output.txt");
    readFile("200.txt");
    cout << "Press any key to continue\n";
    getchar();
    auto start = chrono::high_resolution_clock::now();
    procedure1();
    auto stop = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
    cout << "Time elapsed by procedure 1 ( thread for each element ): ";
    cout << elapsed;
    cout << " nanoseconds \n";
    cout << "\nPress any key to continue\n";
    getchar();
    auto start2 = chrono::high_resolution_clock::now();
    procedure2();
    auto stop2 = chrono::high_resolution_clock::now();
    double elapsed2 = chrono::duration_cast<chrono::nanoseconds>(stop2 - start2).count();
    cout << "Time elapsed by procedure 2 ( thread for each row ): ";
    cout << elapsed2;
    cout << " nanoseconds \n";
    //displayMat(matC,row1,col2);
    out.close();
    return 0;
}

void outputToFile(int num, int ** mat){//function that prints the output to file
    out << row1;
    out << " ";
    out << col2;
    out << "\n";
    for(int i=0 ; i<row1 ; i++){
            for(int j=0; j<col2 ; j++){
                out << mat[i][j];
                out << " ";
            }
            out << "\n";
        }
    out << "END";
    out << num;
    out << "\n";
}

void displayMat(int ** mat,int r , int c){//function that prints a matrix on terminal
        for(int i=0 ; i<r ; i++){
            for(int j=0; j<c ; j++){
                cout << mat[i][j];
                cout << "\t";
            }
            cout << "\n";
        }
        cout << "\n";
}

void readFile(string Filename){//function that read teh input matrices from the file and allocates memory for the output matrix
    ifstream myfile (Filename);
    string line;
    int i,j;
    if (myfile.is_open()){
        myfile >> row1;
        myfile >> col1;
        matA= (int **)malloc(row1* sizeof(int *));//allocate memory for 1st matrix
        for(int k=0; k< row1 ; k++){
            matA[k]= (int *)malloc(col1*sizeof(int));
        }
        for( i=0; i<row1 ; i++){
            for( j=0; j<col1 ; j++){
                myfile >> matA[i][j];
            }
        }
        myfile >> row2;
        myfile >> col2;
        matB= (int **)malloc(row2 * sizeof(int *));//allocate memory for 2nd matrix
        for(int k=0; k< row2 ; k++){
            matB[k]= (int *)malloc(col2*sizeof(int));
        }
        for( i=0; i<row2 ; i++){
            for( j=0; j<col2 ; j++){
                if(myfile.eof()){
                    break;
                }
                myfile >> matB[i][j];
            };
        }
        matC= (int **)malloc(row1* sizeof(int *));//allocate memory for output of procedure 1
        for(int k=0; k< row1 ; k++){
            matC[k]= (int *)malloc(col2*sizeof(int));
        }
        matD= (int **)malloc(row1* sizeof(int *));//allocate memory for output of procedure 1
        for(int k=0; k< row1 ; k++){
            matD[k]= (int *)malloc(col2*sizeof(int));
        }
        /*
        if(!myfile.eof()){
            perror("Couldn't read file correclty.");
        }*/
        //displayMat(matA,row1,col1);
        //displayMat(matB,row2,col2);
    myfile.close();
    }

  else cout << "Unable to open file";
}

void procedure1(){//1st procedure that creates a thread to compute each elemnt of the output matrix
    int num_threads = row1 * col2; //thread for each element
            int ** dim= (int **)malloc(num_threads* sizeof(int *));
            for(int it=0; it<num_threads;it++){
                dim[it] = (int *)malloc( 2 * sizeof(int));
            }
//int dim[2];
    int ret; // will hold return value of pthread_create
    pthread_t * threads;//array of threads
    threads = (pthread_t *) malloc ( num_threads * sizeof(pthread_t));
    int k=0;
    for(int i =0 ; i< row1 ; ++i){
        for(int j=0; j< col2 ; ++j){
        //int * dim = NULL;//parameters that will be passed to compute element( row and col indices)

            dim[k][0] = i; // row index of thread
            dim[k][1] = j; // col index of thread
            //getchar();
            ret = pthread_create(&threads[k], NULL, computeElement, &dim[k] );

            if(ret){
                //perror("Error in creating threads.");}
                k++;
                }
    }

            for(int k =0; k< num_threads; k++){
                pthread_join(threads[k], NULL);
                }
    outputToFile(1, matC);
}
}

void * computeElement(void * arg){//function where the thread calculates an elemnt
    int ** dim = (int **)arg;
    int sum = 0;
    int inner = col1; // col of A , row of B
    int outer1 = dim[0][0]; // row of A
    int outer2 = dim[0][1]; // col of B
    for(int i=0; i<inner ; i++){
        sum+= matA[outer1][i] * matB[i][outer2];
    }
    matC[outer1][outer2] = sum;

    pthread_exit(NULL);
}

void procedure2(){//2nd procedure that creates a thread for each row of output matrix
    int num_threads = row1; // thread for each row
    int ret; // will hold return value of pthread_create
    //int dim[1];//parameters that will be passed to compute row( index of row)
    int ** dim= (int **)malloc(num_threads* sizeof(int *));
            for(int it=0; it<num_threads;it++){
                dim[it] = (int *)malloc( 1 * sizeof(int));
            }
    pthread_t threads[num_threads];//array of threads
    for(int i =0 ; i< row1 ; i++){
        dim[i][0] = i; // row index of thread
            ret = pthread_create(&threads[i], NULL, computeRow, &dim[i] );
            if(ret){
                perror("Error in creating threads.");}
        }
        for(int k =0; k< num_threads; k++){
            pthread_join(threads[k], NULL);
            }
    outputToFile(2, matD);
}

void * computeRow(void * arg){//function where thread calculates elemnts of a whole row
    int ** dim = (int **)arg;
    int sum = 0;
    int inner = col1; // col of A , row of B
    int outer1 = dim[0][0]; // row of A
    for(int j=0; j< col2 ; j++){
        sum = 0;
        for(int i=0; i<inner ; i++){
            sum+= matA[outer1][i] * matB[i][j];
        }
        matD[outer1][j] = sum;
    }

    pthread_exit(NULL);
}
