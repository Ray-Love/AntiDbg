# 生成目录
$root = "AntiDbg-VS2022"
New-Item -ItemType Directory $root
Set-Location $root

# 创建解决方案
& "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com" /command "File.NewProject AntiDbg.sln"

# 生成 Driver 工程
$drv = "Driver\AntiDbg"
New-Item -ItemType Directory -Path $drv -Force
Set-Content "$drv\Driver.c" (Get-Content -Raw -Path "Driver.c")   # 见下方源码
Set-Content "$drv\Driver.h" (Get-Content -Raw -Path "Driver.h")
Set-Content "$drv\Driver.inf" (Get-Content -Raw -Path "Driver.inf")
Set-Content "$drv\AntiDbg.vcxproj" (Get-Content -Raw -Path "Driver.vcxproj")  # 见下方

# 生成用户态 DLL
$lib = "UserLib\AntiDbgLib"
New-Item -ItemType Directory -Path $lib -Force
Set-Content "$lib\AntiDbgLib.cpp" (Get-Content -Raw -Path "AntiDbgLib.cpp")
Set-Content "$lib\AntiDbgLib.h" (Get-Content -Raw -Path "AntiDbgLib.h")
Set-Content "$lib\AntiDbgLib.vcxproj" (Get-Content -Raw -Path "UserLib.vcxproj")

# 生成 Demo
$demo = "UserLib\Demo"
New-Item -ItemType Directory -Path $demo -Force
Set-Content "$demo\UserDemo.cpp" (Get-Content -Raw -Path "UserDemo.cpp")
Set-Content "$demo\UserDemo.vcxproj" (Get-Content -Raw -Path "UserDemo.vcxproj")

# 生成脚本
$sc = "Scripts"
New-Item -ItemType Directory -Path $sc -Force
Set-Content "$sc\sign.bat" "@echo off`ncd /d %~dp0`nmakecert -r -pe -ss PrivateCertStore -n CN=TestCert test.cer`nsigntool sign /s PrivateCertStore /n TestCert /t http://timestamp.digicert.com ..\Driver\AntiDbg\x64\Release\AntiDbg.sys`npause"
Set-Content "$sc\install.bat" "@echo off`ncd /d %~dp0`nsc create AntiDbg binPath=%~dp0..\Driver\AntiDbg\x64\Release\AntiDbg.sys type=kernel`nsc start AntiDbg`npause"

Write-Host "模板已生成到 $root"
