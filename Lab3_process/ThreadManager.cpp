#include "ThreadManager.h"

ThreadManager::ThreadManager(int count) {
    // Manual Reset - мы сами включаем и выключаем их
    hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
    hContinue = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (int i = 0; i < count; ++i) {
        // Auto Reset - сбрасываются сами после waitFor
        hCantWork.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));
        hStop.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));
        isAlive.push_back(true);
    }
}

ThreadManager::~ThreadManager() {
    CloseHandle(hStart);
    CloseHandle(hContinue);

    // Закрываем все созданные дескрипторы
    for (HANDLE h : hThreads) CloseHandle(h);
    for (HANDLE h : hCantWork) CloseHandle(h);
    for (HANDLE h : hStop) CloseHandle(h);
}

void ThreadManager::pulseContinue() {
    SetEvent(hContinue);
    Sleep(15);
    ResetEvent(hContinue);
}