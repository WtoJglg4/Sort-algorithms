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
void PrintData(FILE* , int , int , int , int);

//copy
void CopyArray(int[], int[], int);

//sequence generation
const int MIN_INT = -1000, MAX_INT = 1000, SEQ_QUANT = 3;
void RandomSequenceInt(int[], int);
void AscendingSequenceInt(int[], int);
void DescendingSequenceInt(int[], int);
void(*Sequence[SEQ_QUANT])(int[], int) = {RandomSequenceInt, AscendingSequenceInt, DescendingSequenceInt};


int main(){
    //открытие файлов для записи
    FILE* quickFile = fopen("output/quick.txt", "w");
    FILE* selectionFile = fopen("output/selection.txt", "w"); 

    //длины сортируемых массивов
    int len[] = {15, 1000, 5000, 10000, 25000, 50000, 100000};
    bool lenFlag = true; //true только, когда длина = 15

    //для массива каждой длины
    for (int i = 0; i < size(len); i++){
        int* A = new int[len[i]];
        int* B = new int[len[i]];
        if (i != 0) lenFlag = false; //если длина > 15, не печатаем массивы и тд
        
        //генерация каждого вида последовательности
        for(int seqIndex = 0; seqIndex < SEQ_QUANT; seqIndex++){
            int comparations = 0, swaps = 0; //сравнения и свопы
            Sequence[seqIndex](A, len[i]); //сгенерили необходимую последовательность
            CopyArray(A, B, len[i]); //скопировали в другой массив, чтобы каждый сортировать разными функциями

            //печать в файл маленького массива
            if (lenFlag){
                fprintf(selectionFile, "Unsorted\n");
                fprintf(quickFile, "Unsorted\n");
                PrintArray(A, 0, len[i] - 1, selectionFile);
                PrintArray(B, 0, len[i] - 1, quickFile);
            }

            auto begin = steady_clock::now(); 
            SelectionSort(A, 0, len[i] - 1, selectionFile, lenFlag, comparations, swaps); //сортируем выбором
            auto end = steady_clock::now(); 
            auto duration = duration_cast<microseconds>(end - begin);
            //печать маленьуого отсортированного массива в файл
            if (lenFlag){
                fprintf(selectionFile, "Sorted\n");
                PrintArray(A, 0, len[i] - 1, selectionFile);
            }
            //печать данных в файл
            PrintData(selectionFile, len[i], comparations, swaps, duration.count());

            comparations = 0, swaps = 0;
            begin = steady_clock::now();
            QuickSort(B, 0, len[i] - 1, quickFile, lenFlag, comparations, swaps); //меняем алгоритм сортировки
            end = steady_clock::now(); 
            duration = duration_cast<microseconds>(end - begin);
            if (lenFlag){
                fprintf(quickFile, "Sorted\n");
                PrintArray(B, 0, len[i] - 1, quickFile);
            }
            PrintData(quickFile, len[i], comparations, swaps, duration.count());
        }
        //освобождаем память
        delete[] A;
        delete[] B;
    }
    //закрываем файлы
    fclose(quickFile);
    fclose(selectionFile);
}

//быстрая
void QuickSort(int Array[], int l, int r, FILE* file, bool flag, int& comparations, int& swaps){
    comparations++; //иф
    if (r - l < 1) return; //остался один элемент

    if (flag) PrintArray(Array, l, r, file); //здесь не инкрементируем comparations
    //потому что этот иф не имеет отношения к алгоритму, он просто для вывода нужен

    int Equals, Greater; //индексы на массиве
    //Equals указывает на первый элемент равный опорному(pivot)
    //Greater указывает на первый элемент строго больший опорному(pivot)
    
    //назначение функции Partition - разбить массив на 3 части: элементы строго меньшие pivot, 
    //равные(они уже заняли свои места в массиве) и строго большие pivot
    Partition(Array, l, r, Equals, Greater, comparations, swaps); //разбиение массива на 3 части относительно опорного
    QuickSort(Array, l, Equals - 1, file, flag, comparations, swaps); //сортируем элементы строго меньшие опорного
    QuickSort(Array, Greater, r, file, flag, comparations, swaps); //сортируем элементы строго большие опорного
}

//разбиение
void Partition(int Array[], int l,int r, int& Equals, int& Greater, int& comparations, int& swaps){
    //n - длина разбиваемой части массива, pivot - случайный опорный элемент из массива
    int n = r - l + 1, pivot = Array[rand() % n + l]; 
    int Now = l; //индекс текущего элемента
    Equals = l; //стартуем с l
    Greater = l;
    
    while(Now <= r){
        comparations +=3; //цикл + 2 условия
        if (Array[Now] == pivot){ //добавляем к элементам, равным опорному
            swap(Array[Now], Array[Greater]);
            Greater++;
            swaps++;
        }
        if (Array[Now] < pivot){ //добавляем к элементам, строго меньшим опорного
            swap(Array[Now], Array[Greater]);
            swap(Array[Greater], Array[Equals]);
            Greater++;
            Equals++;
            swaps+=2;
        }
        Now++;
    } 
    comparations++;//последнее сравнение, когда не входим в цикл
}

//выбором
void SelectionSort(int Array[], int l, int r, FILE* file, bool flag, int& comparations, int& swaps){
    int len = r - l + 1;
    for (int i = l; i < len - 1; i++){
        comparations++; //сравнение в цикле
        if (flag){ //игнорируем этот иф
        PrintArray(Array, l, r, file);    
        }
        
        int smallest = i; //наименьший
        for (int j = i + 1; j < len; j++){
            comparations+=2; //цикл + иф
            if (Array[j] <= Array[smallest]){
                smallest = j; //найти меньший либо равный iтому
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

//printing statistics
void PrintData(FILE* file, int len, int comparations, int swaps, int time){
    fprintf(file, "Len: %d\tComparations: %d\tSwaps:%d\tTime:%d\n\n", len, comparations, swaps, time);
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