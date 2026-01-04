#include "MarkerLogic.h"

DWORD WINAPI MarkerThread(LPVOID lpParam) {
    ThreadArgs* args = (ThreadArgs*)lpParam;
    int id = args->id;
    int num = args->threadNum;
    ArrayManager* am = args->am;
    ThreadManager* tm = args->tm;
    int* arr = am->getArray();
    int size = am->getSize();

    // 1. Ждем старт
    WaitForSingleObject(tm->hStart, INFINITE);

    srand(num);
    int markedCount = 0;

    while (true) {
        int index = rand() % size;

        // Попытка пометить элемент
        if (arr[index] == 0) {
            Sleep(5);
            arr[index] = num;
            Sleep(5);
            markedCount++;
        }
        else {
            // Неудача
            am->synchronizedPrint("Не могу пометить, занято.", num, index);

            // Сигналим мейну
            SetEvent(tm->hCantWork[id]);

            // Ждем команды (с или прод)
            HANDLE waitFor[] = { tm->hStop[id], tm->hContinue };
            DWORD waitResult = WaitForMultipleObjects(2, waitFor, FALSE, INFINITE);

            if (waitResult == WAIT_OBJECT_0) {
                // Получили hStop
                am->synchronizedPrint("Завершаю работу.", num);
                am->cleanElement(num);
                return 0;
            }
            // Иначе hContinue
        }
    }
    return 0;
}