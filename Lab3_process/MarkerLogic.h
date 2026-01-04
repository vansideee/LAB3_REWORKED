#pragma once
#include <windows.h>
#include "ArrayManager.h"
#include "ThreadManager.h"

// —труктура аргументов дл€ передачи в поток
struct ThreadArgs {
    int id;               
    int threadNum;        
    ArrayManager* am;
    ThreadManager* tm;
};


DWORD WINAPI MarkerThread(LPVOID lpParam);