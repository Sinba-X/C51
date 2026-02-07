@echo off
chcp 65001 >nul
echo ============================================
echo EIDE 项目诊断工具
echo ============================================
echo.

cd /d "%~dp0"

echo [1] 检查当前目录
pwd
echo.

echo [2] 检查 .eide 配置文件
if exist ".eide\eide.yml" (
    echo ✓ .eide\eide.yml 存在
) else (
    echo ✗ .eide\eide.yml 不存在
)
echo.

echo [3] 检查 workspace 文件
if exist "C51.code-workspace" (
    echo ✓ C51.code-workspace 存在
) else (
    echo ✗ C51.code-workspace 不存在
)
echo.

echo [4] 检查 EIDE 插件
code --list-extensions | findstr "eide"
echo.

echo [5] 显示 eide.yml 前10行
type .eide\eide.yml | more /E
echo.

echo ============================================
echo 诊断完成
echo.
echo 如果所有检查都通过，请：
echo 1. 关闭所有VSCode窗口
echo 2. 双击 "打开项目.bat"
echo 3. 在EIDE面板查看项目
echo ============================================
echo.
pause
