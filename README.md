# SecurityCheck DLL

用于检测调试器和分析工具的安全检测DLL。

## GitHub Actions 状态

![Build Status](https://github.com/your-username/SecurityCheck/actions/workflows/build.yml/badge.svg)

## 下载预编译版本

前往 [Releases](https://github.com/your-username/SecurityCheck/releases) 页面下载最新版本的DLL。

## 本地编译

### 使用MSVC
```bash
cl /LD SecurityCheck.cpp /link /OUT:SecurityCheck.dll user32.lib kernel32.lib advapi32.lib
