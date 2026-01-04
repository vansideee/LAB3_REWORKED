#pragma once
#include <iostream>
#include <vector>
#include <windows.h>

class ArrayManager {
private:
    int* arr;
    int size;
    CRITICAL_SECTION printCS;

public:
    ArrayManager(int n);
    ~ArrayManager();

    int* getArray();
    int getSize();
    void printArray();
    void cleanElement(int threadNum); 

    // Для потокобезопасного вывода сообщений
    void synchronizedPrint(const char* msg, int threadNum, int index = -1);
};