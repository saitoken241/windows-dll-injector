// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include <windows.h>
#include "framework.h"
#include <iostream>

using namespace std;

EXTERN_C void funnyFunction(void) {
    
    cout << "inside of funnyFunction\n";
    system("start calc.exe");

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        cout << "inside of DLL infected\n";
        funnyFunction();
        return TRUE;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


