
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <wchar.h>
#include <TlHelp32.h>
using namespace std;

#define DLL_PATH "C:\\Users\\dudue\\source\\repos\\estudo_Dll1\\x64\\Debug\\estudo_Dll1.dll"

#define TARGET_BINARY L"notepad.exe"


typedef int(__stdcall* f_funnyFunction)();

DWORD findProcessID();

int main()
{
	DWORD pid;
	HANDLE hProcess;
	LPVOID lpBaseAddress;
	size_t  sz = strlen(DLL_PATH);
	int output_val = 0;

	cout << "Beginning attach sequence now!\n";
	//get pid for target binary
	pid = findProcessID();

	//get handle to target process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);

	//allocate memory in target process
	lpBaseAddress = VirtualAllocEx(hProcess, NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//write dll path to the target binary memory
	output_val = WriteProcessMemory(hProcess, lpBaseAddress, DLL_PATH, sz, NULL);

	//get handle to kernel32.dll 
	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	
	//get address of LoadLibraryA
	LPVOID lpStartAdress = GetProcAddress(hKernel32, "LoadLibraryA");

	//create remote thread in target process to load our dll

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAdress, lpBaseAddress, 0, NULL);

	if (hThread == NULL) {
		cout << "Failed to create remote thread\n";
	}
	else {
		cout << "Remote thread created successfully\n";
	}


}

DWORD findProcessID() {
	HANDLE hProcessSnap;
	HANDLE hProcessl;
	PROCESSENTRY32 pe32;
	DWORD DwPriorityClass;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	
	
	}

	do {
		if (!wcscmp(pe32.szExeFile, TARGET_BINARY)) {

			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	return 0;

}
