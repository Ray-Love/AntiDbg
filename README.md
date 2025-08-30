# SecurityCheck DLL

用于检测调试器和分析工具的安全检测DLL。

## dll命令

' 主要检测函数
.DLL命令 CheckDebuggers, 逻辑型, "SecurityCheck.dll", "CheckDebuggers"
    .参数 callback, 整数型, , , 0

' 设置处理方式
.DLL命令 SetAction, , "SecurityCheck.dll", "SetAction"
    .参数 action, 整数型
    .参数 message, 文本型
    .参数 callback, 整数型, , , 0

' 获取工具数量
.DLL命令 GetToolCount, 整数型, "SecurityCheck.dll", "GetToolCount"

' 获取工具名称
.DLL命令 GetToolName, 文本型, "SecurityCheck.dll", "GetToolName"
    .参数 index, 整数型

## 下载预编译版本

前往 [Releases](https://github.com/Ray-Love/SecurityCheck/releases) 页面下载最新版本的DLL。
