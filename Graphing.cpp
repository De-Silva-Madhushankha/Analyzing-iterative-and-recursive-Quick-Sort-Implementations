//
// Created by Madhshankha De Silva on 2/16/2024.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <random>
#include <cstdio>

#include <iostream>
#include <vector>
#include <cmath>
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"
#include <stack>

using namespace std;

// Functions To Test//

//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//

int Partition(vector<int>& arr, int start, int end) {
    int i = start - 1; // Ultimately "i" is the suitable index of pivot
    int pivot = arr[end - 1]; //Initial Guess

    for (int j = start; j < end - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    i++;
    int temp = arr[i];
    arr[i] = arr[end - 1];
    arr[end - 1] = temp;

    return i;
}

void QuickSort(vector<int>& arr, int start, int end) {

    if (end <= start) {
        return;
    }

    int pivot = Partition(arr, start, end);
    QuickSort(arr, start, pivot);
    QuickSort(arr, pivot + 1, end);

}


void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(vector<int>& arr, int l, int h) {
    int x = arr[h];
    int i = l - 1;
    for (int j = l; j <= h - 1; j++) {
        if (arr[j] <= x) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[h]);
    return i + 1;
}

void quickSortIterative(vector<int>& arr, int l, int h) {
    stack<int> st;
    st.push(l);
    st.push(h);
    while (!st.empty()) {
        h = st.top();
        st.pop();
        l = st.top();
        st.pop();
        int p = partition(arr, l, h);
        if (p - 1 > l) {
            st.push(l);
            st.push(p - 1);
        }
        if (p + 1 < h) {
            st.push(p + 1);
            st.push(h);
        }
    }
}



//---------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------//


vector<int> getUserArray() {

    /*
        Function read a line from stdin and produce an array

    */
    string inputLine;
    getline(cin, inputLine);
    istringstream stream(inputLine);
    vector<int> array;
    int element;
    while (stream >> element) {
        array.push_back(element);
    }

    return array;
}


vector<int> GenerateArray(int len) {

    /*
    
        this function returns a array of random values for a given size
    */

    std::random_device dev;
    std::mt19937 range(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist20(1, 20); 

    vector<int> testArray(len);

    for (int i = 0; i < (len + 1); i++) {
        testArray.push_back(dist20(range));
    }

    return testArray;

}

vector<vector<int>> GetExecutionTimes() {

    /*
        This function returns  execution times of sorting algorithms for various array lengths for a given function name function   
    
    */

    
        vector<vector<int>> result;

        typedef chrono::high_resolution_clock Clock;

        vector<int> RecurExeTime;
        vector<int> arrayLengths;
        vector<int> ItrExeTime;

        for (int len = 3; len < 52; len += 2) {
            vector<int> array = GenerateArray(len);
            auto t1 = Clock::now();
            QuickSort(array, 0, len);
            auto t2 = Clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
            RecurExeTime.push_back(duration);
            arrayLengths.push_back(len);

        }
        printf("Quick Sort Recursive Approach\n\n");
        printf("Array Length\t\tExecution Time(nanoSeconds)\n");
        for (int i = 0; i < RecurExeTime.size(); i++) {
            printf("%d\t\t\t%d\n", arrayLengths[i], RecurExeTime[i]);
        }


        result.push_back(arrayLengths);
        result.push_back(RecurExeTime);

        cout << "\n\n";


        for (int len = 3; len < 52; len += 2) {
            vector<int> array = GenerateArray(len);
            auto t1 = Clock::now();
            quickSortIterative(array, 0, len);
            auto t2 = Clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
            ItrExeTime.push_back(duration);

        }
        printf("Quick Sort Iterative Approach\n\n");
        printf("Array Length\t\tExecution Time(nanoSeconds)\n");
        for (int i = 0; i < ItrExeTime.size(); i++) {
            printf("%d\t\t\t%d\n", arrayLengths[i], ItrExeTime[i]);
        }


        result.push_back(ItrExeTime);

        return result;
    
}


int main() {

    //vector<int> array = getUserArray();
    //int start = 0;
    //int end = array.size();


    vector<vector<int>> result = GetExecutionTimes();

    //for (auto e : result[1]) {
    //    cout << e << " ";
    //}

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    //Set plot title
	gp << "set title 'Array Length vs Execution Time'\n";

    gp << "set xrange [0:50]\nset yrange [0:0100000]\n";
    gp << "set xlabel 'Array Length'\nset ylabel 'Execution Time (nanoseconds)'\n";
    
      
    gp << "plot '-' with lines title 'Recursive', '-' with lines title 'iterative\n";
    gp.send1d(boost::make_tuple(result[0], result[1]));
    gp.send1d(boost::make_tuple(result[0], result[2]));
    

    //To avoid imediate closure of prompt
    #ifdef _WIN32
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
    #endif

    return 0;
}
