#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

//sort functions
void QuickSort(int[], int, int, FILE*, bool, int&, int&);
void Partition(int[] , int , int, int&, int&, int&, int&);
void SelectionSort(int[], int, int, FILE*, bool, int&, int&);

//print
void PrintArray(int[], int, int, FILE*);

//copy
void CopyArray(int[], int[], int);

//sequence generation
const int MIN_INT = -1000, MAX_INT = 1000, SEQ_QUANT = 3;
void RandomSequenceInt(int[], int);
void AscendingSequenceInt(int[], int);
void DescendingSequenceInt(int[], int);
void(*Sequence[SEQ_QUANT])(int[], int) = {RandomSequenceInt, AscendingSequenceInt, DescendingSequenceInt};


int main(){
    FILE* quickFile = fopen("output/quick.txt", "w"); 
    FILE* selectionFile = fopen("output/selection.txt", "w"); 

    int len[] = {15, 1000, 5000, 10000, 25000, 50000, 100000};
    bool lenFlag = true; //true только, когда длина = 15

    for (int i = 0; i < size(len); i++){
        int* A = new int[len[i]];
        int* B = new int[len[i]];
        if (i != 0) lenFlag = false;
        
        //генерация каждого вида последовательности
        for(int seqIndex = 0; seqIndex < SEQ_QUANT; seqIndex++){
            int comparations = 0, swaps = 0;
            Sequence[seqIndex](A, len[i]);
            CopyArray(A, B, len[i]);

            if (lenFlag){
                fprintf(selectionFile, "Unsorted\n");
                fprintf(quickFile, "Unsorted\n");
                PrintArray(A, 0, len[i] - 1, selectionFile);
                PrintArray(B, 0, len[i] - 1, quickFile);
            }

            auto begin = steady_clock::now(); 
            SelectionSort(A, 0, len[i] - 1, selectionFile, lenFlag, comparations, swaps);
            auto end = steady_clock::now(); 
            auto duration = duration_cast<microseconds>(end - begin);
            if (lenFlag){
                fprintf(selectionFile, "Sorted\n");
                PrintArray(A, 0, len[i] - 1, selectionFile);
                fprintf(selectionFile, "Comparations: %d\tSwaps:%d\tTime:%d\n", comparations, swaps, duration.count());
            }

            comparations = 0, swaps = 0;
            begin = steady_clock::now();
            QuickSort(B, 0, len[i] - 1, quickFile, lenFlag, comparations, swaps);
            end = steady_clock::now(); 
            duration = duration_cast<microseconds>(end - begin);
            if (lenFlag){
                fprintf(quickFile, "Sorted\n");
                PrintArray(B, 0, len[i] - 1, quickFile);
                fprintf(quickFile, "Comparations: %d\tSwaps:%d\tTime:%d\n", comparations, swaps, duration.count());
            }
        }
        delete[] A;
        delete[] B;
    }
    fclose(quickFile);
    fclose(selectionFile);
}

void QuickSort(int Array[], int l, int r, FILE* file, bool flag, int& comparations, int& swaps){
    comparations++;
    if (r - l < 1) return;

    if (flag) PrintArray(Array, l, r, file);    

    int E, G;
    Partition(Array, l, r, E, G, comparations, swaps);
    QuickSort(Array, l, E - 1, file, flag, comparations, swaps);
    QuickSort(Array, G, r, file, flag, comparations, swaps);
}

void Partition(int Array[], int l,int r, int& E, int& G, int& comparations, int& swaps){
    int n = r - l + 1, pivot = Array[rand() % n + l];
    int N = l, temp;
    E = l;
    G = l;
    
    while(N <= r){
        comparations +=3; //цикл + 2 условия
        if (Array[N] == pivot){
            swap(Array[N], Array[G]);
            G++;
            swaps++;
        }
        if (Array[N] < pivot){
            swap(Array[N], Array[G]);
            swap(Array[G], Array[E]);
            G++;
            E++;
            swaps+=2;
        }
        N++;
    } 
    comparations++;//последнее сравнение, когда не входим в цикл
}

void SelectionSort(int Array[], int l, int r, FILE* file, bool flag, int& comparations, int& swaps){
    int len = r - l + 1;
    for (int i = l; i < len - 1; i++){
        comparations++;
        if (flag){
        PrintArray(Array, l, r, file);    
        }
        
        int smallest = i;
        for (int j = i + 1; j < len; j++){
            comparations+=2;
            if (Array[j] <= Array[smallest]){
                smallest = j;
            }
        }
        swap(Array[i], Array[smallest]);
        comparations++;//последнее сравнение в цикле
        swaps++;
    }
    comparations++;//последнее сравнение в цикле
}

//printing array
void PrintArray(int Array[], int l, int r, FILE* file){
    fprintf(file, "[");
    for (int i = l; i <= r; i++){
        fprintf(file ,"%d", Array[i]);
        if (i != r){
            fprintf(file, ", ");
        }
    }
    fprintf(file, "]\n");
}

//copying from source array to destination array
void CopyArray(int srcArr[], int dstArr[], int len){
    for (int i = 0; i < len; i++){
        dstArr[i] = srcArr[i];
    }
}

//generates a random integer sequence
void RandomSequenceInt(int Array[], int len){
    srand(time(NULL));
    for (int i = 0; i < len; i++){
        Array[i] = MIN_INT + rand() % (MAX_INT - MIN_INT + 1);
    }
}

//generates a ascending integer sequence
void AscendingSequenceInt(int Array[], int len){
    double k = ((MAX_INT - MIN_INT) * 1.0) / (len * 1.0), b = MIN_INT;
    for (int i = 0; i < len; i++){
        Array[i] = k * i + b;
    }
}

//generates a descending integer sequence
void DescendingSequenceInt(int Array[], int len){
    double k = ((MAX_INT - MIN_INT) * -1.0) / (len * 1.0), b = MAX_INT;
    for (int i = 0; i < len; i++){
        Array[i] = k * i + b;
    }
}