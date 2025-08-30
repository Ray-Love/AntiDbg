#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef BOOL(CALLBACK* DETECTION_CALLBACK)(const char* toolName, BOOL isDetected);
    typedef void(CALLBACK* ACTION_CALLBACK)(const char* toolName);

    enum ActionType {
        BLUE_SCREEN,
        FORCE_EXIT,
        MESSAGE_BOX,
        CUSTOM_CALLBACK
    };

    __declspec(dllexport) BOOL CheckDebuggers(DETECTION_CALLBACK callback);
    __declspec(dllexport) void SetAction(ActionType action, const char* message, ACTION_CALLBACK callback);
    __declspec(dllexport) int GetToolCount();
    __declspec(dllexport) const char* GetToolName(int index);

#ifdef __cplusplus
}
#endif
