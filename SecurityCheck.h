#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef BOOL(CALLBACK* DETECTION_CALLBACK)(const char* toolName, BOOL isDetected);
    typedef void(CALLBACK* ACTION_CALLBACK)(const char* toolName);

    enum class ActionType {
        BLUE_SCREEN,
        FORCE_EXIT,
        MESSAGE_BOX,
        CUSTOM_CALLBACK
    };

    __declspec(dllexport) BOOL __cdecl CheckDebuggers(DETECTION_CALLBACK callback);
    __declspec(dllexport) void __cdecl SetAction(ActionType action, const char* message, ACTION_CALLBACK callback);
    __declspec(dllexport) int __cdecl GetToolCount();
    __declspec(dllexport) const char* __cdecl GetToolName(int index);

#ifdef __cplusplus
}
#endif
