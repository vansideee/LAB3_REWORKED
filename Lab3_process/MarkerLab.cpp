#include <iostream>
#include "ArrayManager.h"
#include "ThreadManager.h"
#include "MarkerLogic.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    int arrSize, threadCount;
    cout << "Введите размер массива: ";
    cin >> arrSize;
    cout << "Введите количество потоков: ";
    cin >> threadCount;

    // Инициализация менеджеров
    ArrayManager am(arrSize);
    ThreadManager tm(threadCount);

    // Запуск потоков
    vector<ThreadArgs*> argsList; 
    for (int i = 0; i < threadCount; ++i) {
        // Выделяем память под аргументы для каждого потока
        ThreadArgs* args = new ThreadArgs{ i, i + 1, &am, &tm };
        argsList.push_back(args);

        tm.hThreads.push_back(CreateThread(NULL, 0, MarkerThread, args, 0, NULL));
    }

    // Стартуем
    SetEvent(tm.hStart);

    int activeThreads = threadCount;

   
    while (activeThreads > 0) {
        // Сбор дескрипторов "застрявших"  потоков
        vector<HANDLE> activeWaiters;
        for (int i = 0; i < threadCount; i++) {
            if (tm.isAlive[i]) {
                activeWaiters.push_back(tm.hCantWork[i]);
            }
        }

        // Ждем всех живых
        if (!activeWaiters.empty()) {
            WaitForMultipleObjects((DWORD)activeWaiters.size(), activeWaiters.data(), TRUE, INFINITE);
        }

        am.printArray();

        // Логика убийства потока
        int idToKill = -1;
        while (true) {
            cout << "Введите номер потока для остановки (1-" << threadCount << "): ";
            int k;
            cin >> k;
            if (k >= 1 && k <= threadCount && tm.isAlive[k - 1]) {
                idToKill = k - 1;
                break;
            }
            cout << "Неверный номер или поток уже мертв.\n";
        }

        // Убиваем
        SetEvent(tm.hStop[idToKill]);
        WaitForSingleObject(tm.hThreads[idToKill], INFINITE); // Ждем пока реально выйдет
        tm.isAlive[idToKill] = false;
        activeThreads--;

        am.printArray(); // Показать массив после очистки

        // Отпускаем остальных 
        if (activeThreads > 0)
            tm.pulseContinue();
    }

    cout << "Все потоки завершены.\n";

   
    for (auto args : argsList) delete args;

   
    return 0;
}