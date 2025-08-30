// AntiDbgLib.cpp
#include "AntiDbgLib.h"
#include <windows.h>
#include <winioctl.h>

#define IOCTL_ANTIDBG_DETECT CTL_CODE(0x8000, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS)

extern "C" __declspec(dllexport)
bool __stdcall DetectTools(int action, const char* msg, void(*callback)())
{
    HANDLE h = CreateFileW(L"\\\\.\\AntiDbg", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;

    struct { ULONG action; ULONG pid; } in = { (ULONG)action, 0 };
    BOOLEAN detected = FALSE;
    DWORD ret = 0;
    DeviceIoControl(h, IOCTL_ANTIDBG_DETECT, &in, sizeof(in), &detected, sizeof(detected), &ret, NULL);
    CloseHandle(h);

    if (detected && action == 2 && callback) callback();
    return detected;
}
