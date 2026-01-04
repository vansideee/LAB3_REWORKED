#include "ArrayManager.h"
#include <algorithm> 

using namespace std;

ArrayManager::ArrayManager(int n) : size(n) {
    arr = new int[size];
    std::fill(arr, arr + size, 0); 
    InitializeCriticalSection(&printCS);
}

ArrayManager::~ArrayManager() {
    DeleteCriticalSection(&printCS);
    delete[] arr;
}

int* ArrayManager::getArray() { return arr; }

int ArrayManager::getSize() { return size; }

void ArrayManager::printArray() {
    EnterCriticalSection(&printCS);
    cout << "\nМассив: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    LeaveCriticalSection(&printCS);
}

void ArrayManager::cleanElement(int threadNum) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == threadNum) {
            arr[i] = 0;
        }
    }
}

void ArrayManager::synchronizedPrint(const char* msg, int threadNum, int index) {
    EnterCriticalSection(&printCS);
    cout << "Поток " << threadNum << ": " << msg;
    if (index != -1) cout << " (индекс " << index << ")";
    cout << endl;
    LeaveCriticalSection(&printCS);
}