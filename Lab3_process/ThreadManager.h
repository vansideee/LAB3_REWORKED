#pragma once
#include <windows.h>
#include <vector>

class ThreadManager {
public:
    // сигналы
    HANDLE hStart;
    HANDLE hContinue;

    
    std::vector<HANDLE> hThreads;
    std::vector<HANDLE> hCantWork; // Сигнал что он встал
    std::vector<HANDLE> hStop;     // Сигнал умереть
    std::vector<bool>   isAlive;   

    ThreadManager(int count);
    ~ThreadManager();

    // Функция, запускающая событие Continue на короткое время 
    void pulseContinue();
};