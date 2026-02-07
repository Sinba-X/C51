# EIDE Lesson 切换工具使用说明

## 工具简介

这个工具可以帮助你快速切换要编译的lesson项目，自动更新EIDE配置文件。

## 文件说明

- **switch_lesson.py** - Python切换脚本（核心）
- **switch_lesson.bat** - Windows批处理文件（双击运行）

## 使用方法

### 方法1：双击运行（推荐）

1. 双击 `switch_lesson.bat` 文件
2. 会显示所有88个lesson项目的列表
3. 输入要编译的项目编号（1-88）
4. 按回车确认
5. 脚本会自动更新 `.eide/eide.yml` 配置文件
6. 在VSCode中重新加载窗口（Ctrl+Shift+P → "Reload Window"）
7. 使用EIDE编译新选择的项目

### 方法2：命令行运行

打开终端，在项目根目录执行：
```bash
"D:\Program Files (x86)\python\python.exe" switch_lesson.py
```

## 项目列表示例

```
  [ 45] lesson2
  [ 46] lesson3/lesson3_1
  [ 47] lesson3/lesson3_t4
  [ 48] lesson4/lesson4_1
  ...
```

## 当前配置的项目

当前EIDE配置为编译：**lesson2**

包含文件：
- src/lesson2/STARTUP.A51
- src/lesson2/LED.c

## 切换后的操作

1. 在VSCode中按 `Ctrl+Shift+P`
2. 输入 "Developer: Reload Window" 并回车
3. 打开EIDE面板查看新项目
4. 点击构建按钮编译项目

## EIDE使用提示

### 编译项目
- **方法1**: 点击EIDE面板的构建按钮（锤子图标）
- **方法2**: 按 `Ctrl+Shift+B` 选择 "build"
- **方法3**: 命令面板 → "EIDE: Build"

### 烧录到单片机
- **方法1**: 点击EIDE面板的烧录按钮（闪电图标）
- **方法2**: 选择 "flash" 任务
- **注意**: 确保COM5端口连接正常

### 清理构建
- 选择 "clean" 任务清理build目录

### 重新构建
- 选择 "rebuild" 任务进行完全重新编译

## 编译器配置

- **工具链**: Keil C51
- **优化级别**: level-8（最高优化）
- **RAM模式**: SMALL
- **ROM模式**: LARGE
- **输出格式**: ELF

## 头文件路径

已配置的C51头文件路径：
- `D:/Program Files (x86)/C51/INC`

包含以下常用头文件：
- reg52.h
- reg51.h
- intrins.h
等

## 烧录配置

- **端口**: COM5
- **波特率**: 115200
- **工具**: stcgal (通过Python)

如需修改端口，请编辑 `.eide/stc.flash.json` 文件。

## 故障排查

### 1. 脚本无法运行
确保Python已正确安装在：`D:\Program Files (x86)\python\`

### 2. EIDE无法识别项目
在切换后需要重新加载VSCode窗口（Reload Window）

### 3. 编译失败
- 检查Keil C51是否正确安装
- 在EIDE中配置工具链路径为：`D:\Program Files (x86)\C51`

### 4. 烧录失败
- 检查COM端口是否正确
- 确认单片机已连接
- 检查stcgal是否已安装：`python -m stcgal --version`

## 常见问题

**Q: 如何知道当前配置的是哪个项目？**
A: 查看 `.eide/eide.yml` 文件的第2行 `name:` 字段

**Q: 可以同时编译多个lesson吗？**
A: 不可以，每次只能配置一个项目。如需编译多个，请依次切换。

**Q: 切换项目后原来的编译输出会被删除吗？**
A: build目录下的文件可能会被新的编译覆盖，建议每次编译后及时备份hex文件。

**Q: 脚本会修改源代码吗？**
A: 不会，脚本只修改 `.eide/eide.yml` 配置文件，不会改动任何源代码。

## 项目结构

```
C51/
├── src/                    # 源代码目录
│   ├── lesson2/           # 单个项目
│   ├── lesson3/
│   │   ├── lesson3_1/     # 子项目1
│   │   └── lesson3_t4/    # 子项目2
│   └── ...
├── .vscode/               # VSCode配置
│   ├── c_cpp_properties.json
│   ├── settings.json
│   └── tasks.json
├── .eide/                 # EIDE配置
│   ├── eide.yml          # 主配置文件（切换工具会修改这个）
│   ├── env.ini
│   ├── files.options.yml
│   └── stc.flash.json
├── build/                 # 编译输出目录
├── switch_lesson.py       # 切换脚本
├── switch_lesson.bat      # 批处理启动器
└── README_切换工具使用说明.md  # 本文档
```

## 更新日志

**2026-02-07**
- 创建lesson切换工具
- 自动扫描88个lesson项目
- 支持一键切换EIDE配置
