#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
EIDE Lesson切换工具
自动扫描src目录下的所有lesson项目，并更新EIDE配置
"""

import os
import re
from pathlib import Path

# 项目根目录
PROJECT_ROOT = Path(__file__).parent
EIDE_CONFIG = PROJECT_ROOT / ".eide" / "eide.yml"
SRC_DIR = PROJECT_ROOT / "src"

def find_all_projects():
    """扫描所有lesson项目"""
    projects = []

    # 遍历src目录
    for lesson_dir in sorted(SRC_DIR.iterdir()):
        if not lesson_dir.is_dir() or not lesson_dir.name.startswith("lesson"):
            continue

        # 检查是否有子项目目录
        has_subprojects = False
        for item in lesson_dir.iterdir():
            if item.is_dir() and (item.name.startswith("lesson") or item.name == "source"):
                has_subprojects = True
                break

        if has_subprojects:
            # 有子项目，遍历子目录
            for subproject in sorted(lesson_dir.iterdir()):
                if subproject.is_dir():
                    # 查找源文件
                    c_files = list(subproject.glob("*.c"))
                    asm_files = list(subproject.glob("*.a51")) or list(subproject.glob("*.A51"))

                    if c_files or asm_files:
                        rel_path = subproject.relative_to(PROJECT_ROOT)
                        projects.append({
                            'name': f"{lesson_dir.name}/{subproject.name}",
                            'path': rel_path,
                            'c_files': [f.relative_to(PROJECT_ROOT) for f in c_files],
                            'asm_files': [f.relative_to(PROJECT_ROOT) for f in asm_files]
                        })
        else:
            # 直接在lesson目录下查找源文件
            c_files = list(lesson_dir.glob("*.c"))
            asm_files = list(lesson_dir.glob("*.a51")) or list(lesson_dir.glob("*.A51"))

            if c_files or asm_files:
                rel_path = lesson_dir.relative_to(PROJECT_ROOT)
                projects.append({
                    'name': lesson_dir.name,
                    'path': rel_path,
                    'c_files': [f.relative_to(PROJECT_ROOT) for f in c_files],
                    'asm_files': [f.relative_to(PROJECT_ROOT) for f in asm_files]
                })

    return projects

def display_menu(projects):
    """显示项目菜单"""
    print("\n" + "="*60)
    print("  EIDE Lesson 切换工具")
    print("="*60)
    print(f"\n找到 {len(projects)} 个项目:\n")

    for i, proj in enumerate(projects, 1):
        print(f"  [{i:3d}] {proj['name']}")

    print(f"\n  [  0] 退出")
    print("="*60)

def update_eide_config(project):
    """更新EIDE配置文件"""
    # 读取现有配置
    with open(EIDE_CONFIG, 'r', encoding='utf-8') as f:
        content = f.read()

    # 准备文件列表YAML格式
    files_yaml = []

    # 添加汇编文件
    for asm_file in project['asm_files']:
        path = str(asm_file).replace('\\', '/')
        files_yaml.append(f"        - path: {path}")

    # 添加C文件
    for c_file in project['c_files']:
        path = str(c_file).replace('\\', '/')
        files_yaml.append(f"        - path: {path}")

    files_section = '\n'.join(files_yaml)

    # 构建新的virtualFolder部分
    new_virtual_folder = f"""virtualFolder:
  name: <virtual_root>
  files: []
  folders:
    - name: {project['name']}
      files:
{files_section}
      folders: []"""

    # 使用正则表达式替换virtualFolder部分
    pattern = r'virtualFolder:.*?(?=dependenceList:|$)'
    content = re.sub(pattern, new_virtual_folder + '\n', content, flags=re.DOTALL)

    # 更新项目名称
    project_name = project['name'].replace('/', '_')
    content = re.sub(r'^name: .*$', f'name: {project_name}', content, flags=re.MULTILINE)

    # 写回配置文件
    with open(EIDE_CONFIG, 'w', encoding='utf-8') as f:
        f.write(content)

    print(f"\n✓ 已切换到项目: {project['name']}")
    print(f"  路径: {project['path']}")
    print(f"  包含文件:")
    for asm_file in project['asm_files']:
        print(f"    - {asm_file}")
    for c_file in project['c_files']:
        print(f"    - {c_file}")
    print("\n请在VSCode中重新加载窗口 (Ctrl+Shift+P -> Reload Window)")

def main():
    # 检查配置文件是否存在
    if not EIDE_CONFIG.exists():
        print(f"错误: EIDE配置文件不存在: {EIDE_CONFIG}")
        return

    # 扫描项目
    projects = find_all_projects()

    if not projects:
        print("未找到任何lesson项目")
        return

    while True:
        # 显示菜单
        display_menu(projects)

        # 获取用户输入
        try:
            choice = input("\n请选择要编译的项目 [1-{}]: ".format(len(projects)))
            choice = int(choice.strip())

            if choice == 0:
                print("\n再见!")
                break

            if 1 <= choice <= len(projects):
                selected = projects[choice - 1]
                update_eide_config(selected)
                break
            else:
                print(f"\n错误: 请输入 0-{len(projects)} 之间的数字")
        except ValueError:
            print("\n错误: 请输入有效的数字")
        except KeyboardInterrupt:
            print("\n\n已取消")
            break

if __name__ == "__main__":
    main()
