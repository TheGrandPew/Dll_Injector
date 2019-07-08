#include <windows.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    string ex;
    string d;
    cout << "Enter executable: ";
	cin >> ex;
    cout << "\n";
    cout << "Enter dll Name:  ";
	cin >> d;
    cout << "\n";

    cout << "\n";

    char *executable = &ex[0u];
    char *dll = &d[0u];
    

    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION ProcessInfo = { 0 };
    si.cb = sizeof(STARTUPINFO);
    CreateProcess(NULL, executable, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &ProcessInfo);
    HANDLE process = ProcessInfo.hProcess;

    if (process) {
        int length = strlen(dll) + 1;


        void *page = VirtualAllocEx(process, NULL, length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(process, page, dll, length, NULL);
        HANDLE hThread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, page, 0, NULL);

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        CloseHandle(process);
        VirtualFreeEx(process, page, MAX_PATH, MEM_RELEASE);
        cout << "DONE";
    }
    else {
        cout << "ERR";
    };
    return 0;
}