#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 10000000;

// Set this to true if you wish the arrays to be printed.
const bool OUTPUT_DATA = false;


void ReadInput(string& sortAlg, int& size);

void GenerateSortedData(int data[], int size);
void GenerateNearlySortedData(int data[], int size);
void GenerateReverselySortedData(int data[], int size);
void GenerateRandomData(int data[], int size);

void Sort(int data[], int size, string sortAlg, char* dataType);

void InsertionSort(int data[], int size);
void MergeSort(int data[], int lo, int hi);
void QuickSort(int data[], int lo, int hi);
void LibSort(int data[], int size);

void Swap(int &x, int &y);

bool IsSorted(int data[], int size);
void printData(int data[], int size, string title);


int main(void)
{
	int size;
	string sortAlg;
	ReadInput(sortAlg, size);
	
	int * data = new int[size];
	if( data == NULL)
	{
		cout << "\n Memory allocation error." << endl;
		exit(1);
	}
	
	GenerateSortedData(data, size);
	Sort(data, size, sortAlg,"Sorted Data");

	GenerateNearlySortedData(data, size);
	Sort(data, size, sortAlg,"Nearly Sorted Data");
	
	GenerateReverselySortedData(data, size);
	Sort(data, size, sortAlg,"Reversely Sorted Data");
	
	GenerateRandomData(data, size);
	Sort(data, size, sortAlg,"Random Data");
		
	cout << "\nProgram Completed Successfully." << endl;;
	
	delete [] data;
	
	return 0;
}
/********************************************************************/




void ReadInput(string& sortAlg, int& size)
{
	cout << "  I:\tInsertion Sort" << endl;
	cout << "  M:\tMergeSort" << endl;
	cout << "  Q:\tQuickSort" << endl;
	cout << "  L:\tLibSort" << endl;
	cout << "Enter sorting algorithm: ";
	cin >> sortAlg;
	string sortAlgName;
	
	
	if(sortAlg == "I")
		sortAlgName = "Insertion Sort";
	else if(sortAlg == "M")
		sortAlgName = "MergeSort";
	else if(sortAlg == "Q")
		sortAlgName = "QuickSort";
	else if(sortAlg == "L")
		sortAlgName = "LibSort";
	else {
		cout << "\nUnrecognized sorting algorithm Code: " << sortAlg << endl;
		exit(1);
	}
	
	cout << "Enter input size: ";
	cin >> size;
	
	cout << "\nSorting Algorithm: " << sortAlgName;
	cout << "\nInput Size = " << size << endl;
	cout << endl;
	
}
/******************************************************************************/




void GenerateSortedData(int data[], int size)
{
	int i;
	
	for(i=0; i<size; i++)
		data[i] = i * 3 + 5;
}
/*****************************************************************************/



void GenerateNearlySortedData(int data[], int size)
{
	int i;
	
	GenerateSortedData(data, size);
	
	for(i=0; i<size; i++)
		if(i % 10 == 0)
			if(i+1 < size)
				data[i] = data[i+1] + 7;
}
/*****************************************************************************/



void GenerateReverselySortedData(int data[], int size)
{
	int i;
	
	for(i = 0; i < size; i++)
		data[i] = (size-i) * 2 + 3;
}
/*****************************************************************************/



void GenerateRandomData(int data[], int size)
{
	int i;
	
	for(i = 0; i < size; i++)
		data[i] = rand();
}
/*****************************************************************************/


void Sort(int data[], int size, string sortAlg, char* dataType)
{
	
	cout << endl << dataType << ":";
	
	
	if (OUTPUT_DATA)
		printData(data, size, "Data before sorting:");
	
	
	
	// Sorting is about to begin ... start the timer!
	clock_t start = clock();
	
	
	if(sortAlg == "I")
		InsertionSort(data, size);
	else if(sortAlg == "M")
		MergeSort(data, 0, size-1);
	else if(sortAlg == "Q")
		QuickSort(data, 0, size-1);
	else if(sortAlg == "L")
		LibSort(data, size);
	else
	{
		cout << "Invalid sorting algorithm!" << endl;
		exit(1);
	}
	
	// Sorting has finished ... stop the timer!
	clock_t end = clock();
	double elapsed = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
	
	
	
	if (OUTPUT_DATA)
		printData(data, size, "Data after sorting:");
	
	
	if (IsSorted(data, size))
    {
		cout << "\nCorrectly sorted " << size << " elements in " << elapsed << "ms";
    }
	else
		cout << "ERROR!: INCORRECT SORTING!" << endl;
	cout << "\n-------------------------------------------------------------\n";
}
/*****************************************************************************/




bool IsSorted(int data[], int size)
{
	int i;
	
	for(i=0; i<(size-1); i++)
	{
		if(data[i] > data[i+1])
			return false;
	}
	return true;
}
/*****************************************************************************/




void InsertionSort(int data[], int size)
{
	for (int i = 1; i < size; ++i)
    {
        int key = data[i];
        int j;

        for (j = i - 1; j >= 0 && data[j] > key; --j)
        {
            data[j + 1] = data[j];
        }

        data[j + 1] = key;
    }
}
/*****************************************************************************/




void _Combine(int data[], int start, int mid, int end)
{
    int n1 = mid - start + 1,
        n2 = end - mid;

    int *Left  = new int[n1 + 1],
        *Right = new int[n2 + 1];

    /// Copy A[start .. mid] into Left
    for (int i = 0; i < n1; ++i)
    {
        Left[i] = data[start + i];
    }

    /// Copy A[mid + 1 .. end] into Right
    for (int i = 0; i < n2; ++i)
    {
        Right[i] = data[mid + 1 + i];
    }

    int X = max(Left[n1 - 1], Right[n2 - 1]) + 1;
    
    /// Preferably, replace with `std::numeric_limits<int>::max()`
    /// Source: https://stackoverflow.com/questions/1855459/maximum-value-of-int
    Left[n1]  = X;
    Right[n2] = X;

    int i = 0,
        j = 0;

    for (int k = start; k <= end; ++k)
    {
        if (Left[i] < Right[j])
        {
            data[k] = Left[i];
            ++i;
        }
        else
        {
            data[k] = Right[j];
            ++j;
        }
    }

    delete[] Left;
    delete[] Right;
}


void MergeSort(int data[], int lo, int hi)
{
	if ( lo >= hi )
        return;

    int mid = ( lo + hi ) / 2;

    MergeSort(data,    lo,   mid);
    MergeSort(data, mid + 1,  hi);

    _Combine(data, lo, mid, hi);
}

/*****************************************************************************/


int _Partition(int data[], int start, int end)
{
    int value = data[end];

    /// i = Partition Index
    int i = start - 1;

    for (int j = start; j < end; ++j)
    {
        if ( data[j] <= value )
        {
            ++i;
            std::swap(data[i], data[j]);
        }
    }

    std::swap(data[i + 1], data[end]);

    return i + 1;
}


void QuickSort(int data[], int lo, int hi)
{
    if ( lo >= hi )
        return;

    int pivot = _Partition(data, lo, hi);

    QuickSort(data,    lo,     pivot - 1);
    QuickSort(data, pivot + 1,    hi    );
}
/*****************************************************************************/



void LibSort(int data[], int size)
{
	std::sort(data, data + size);
}
/*****************************************************************************/




void Swap(int &x, int &y)
{
	int temp = x;
	x = y;
	y = temp;
}
/*****************************************************************************/




void printData(int data[], int size, string title) {
	int i;

	cout << endl << title << endl;
	for(i=0; i<size; i++)
	{
		cout << data[i] << " ";
		if(i%10 == 9 && size > 10)
			cout << endl;
	}
}
