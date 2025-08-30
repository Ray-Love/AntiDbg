#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <algorithm>

// 检测结果和处理方式类型定义
typedef BOOL(CALLBACK* DETECTION_CALLBACK)(const char* toolName, BOOL isDetected);
typedef void(CALLBACK* ACTION_CALLBACK)(const char* toolName);

// 工具名称和进程名列表
const std::vector<std::pair<std::string, std::string>> tools = {
    {"OllyDbg", "ollydbg.exe"},
    {"CheatEngine", "cheatengine.exe"},
    {"Sunny抓包工具", "sunny.exe"},
    {"x64dbg", "x64dbg.exe"},
    {"x32dbg", "x32dbg.exe"},
    {"Wireshark", "wireshark.exe"},
    {"OllyICE", "ollyice.exe"},
    {"API_Monitor", "apimonitor.exe"},
    {"Nanomite", "nanomite.exe"},
    {"WinDbg", "windbg.exe"},
    {"Burp_Suite_Pro", "burpsuite.exe"},
    {"Microsoft Network Monitor", "netmon.exe"}
};

// 处理方式枚举
enum ActionType {
    BLUE_SCREEN,
    FORCE_EXIT,
    MESSAGE_BOX,
    CUSTOM_CALLBACK
};

// 全局变量
ActionType globalAction = MESSAGE_BOX;
std::string customMessage = "检测到调试工具正在运行！";
ACTION_CALLBACK customCallback = nullptr;

// 检测进程是否存在
BOOL IsProcessRunning(const std::string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return FALSE;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            char currentProcess[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, pe.szExeFile, -1, currentProcess, MAX_PATH, NULL, NULL);
            
            std::string narrowProcess(currentProcess);
            std::transform(narrowProcess.begin(), narrowProcess.end(), narrowProcess.begin(), ::tolower);

            std::string targetName = processName;
            std::transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);

            if (narrowProcess.find(targetName) != std::string::npos) {
                CloseHandle(hSnapshot);
                return TRUE;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return FALSE;
}

// 蓝屏处理
void CauseBlueScreen() {
    // 注意：这种方法在现代Windows系统中可能无效或导致不稳定
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (ntdll) {
        typedef void(*RtlSetProcessIsCritical)(
            BOOLEAN bNew,
            BOOLEAN* pbOld,
            BOOLEAN bNeedScb);

        RtlSetProcessIsCritical setCritical = (RtlSetProcessIsCritical)
            GetProcAddress(ntdll, "RtlSetProcessIsCritical");
        if (setCritical) {
            setCritical(TRUE, nullptr, FALSE);
        }
        FreeLibrary(ntdll);
    }
}

// 强制结束进程
void ForceExitProcess() {
    ExitProcess(0);
}

// 弹出信息框
void ShowMessageBox(const std::string& toolName) {
    std::string message = customMessage + "\n检测到的工具: " + toolName;
    MessageBoxA(nullptr, message.c_str(), "安全警告", MB_ICONWARNING | MB_OK);
}

// 主要检测函数
extern "C" __declspec(dllexport) BOOL CheckDebuggers(DETECTION_CALLBACK callback) {
    BOOL anyDetected = FALSE;

    for (const auto& tool : tools) {
        BOOL detected = IsProcessRunning(tool.second);
        if (detected) {
            anyDetected = TRUE;
            
            if (callback) {
                if (!callback(tool.first.c_str(), detected)) {
                    break;
                }
            }
            else {
                // 使用全局处理方式
                switch (globalAction) {
                case BLUE_SCREEN:
                    CauseBlueScreen();
                    break;
                case FORCE_EXIT:
                    ForceExitProcess();
                    break;
                case MESSAGE_BOX:
                    ShowMessageBox(tool.first);
                    break;
                case CUSTOM_CALLBACK:
                    if (customCallback) {
                        customCallback(tool.first.c_str());
                    }
                    break;
                }
            }
        }
    }

    return anyDetected;
}

// 设置处理方式
extern "C" __declspec(dllexport) void SetAction(ActionType action, const char* message, ACTION_CALLBACK callback) {
    globalAction = action;
    if (message) {
        customMessage = message;
    }
    customCallback = callback;
}

// 获取检测工具列表
extern "C" __declspec(dllexport) int GetToolCount() {
    return static_cast<int>(tools.size());
}

// 获取工具名称
extern "C" __declspec(dllexport) const char* GetToolName(int index) {
    if (index >= 0 && index < static_cast<int>(tools.size())) {
        return tools[index].first.c_str();
    }
    return nullptr;
}

// DLL入口点
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
