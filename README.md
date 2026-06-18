# AIBalance - TrafficMonitor Plugin

> [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) 插件，用于在任务栏显示 AI 提供商账户余额。

## 支持的 AI 提供商

- DeepSeek
- Kimi
- 智谱
- 硅基流动

## 推荐开发环境

- **操作系统**: Windows 10/11
- **IDE**: Visual Studio Community 2026
  - 工作负荷勾选`使用 C++ 的桌面开发`
  - 单个组件勾选：
	- `C++ MFC for x64/x86 (最新 MSVC)`
	- `MSVC v143 - VS 2022 C++ x64/x86 生成工具 (v14.44-17.14)`
	- `适用于 v143 生成工具的 C++ v14.44 (17.14) ATL (x86 和 x64)`
	- `适用于 v143 生成工具的 C++ v14.44 (17.14) MFC (x86 和 x64)`

## 开发构建

1. 克隆仓库到本地
2. 使用 Visual Studio 2026 打开 `AIBalance.vcxproj`
3. 选择 `Release|x64` 配置
4. 开始执行(不调试)（Crtl+F5）

## 使用方法

1. 将生成的 `Bin\x64\Release\plugins\AIBalance.dll` 复制到 TrafficMonitor 的 `plugins` 目录
2. 重启 TrafficMonitor
3. 右键 TrafficMonitor → 插件管理：可看到插件加载成功
4. 右键 TrafficMonitor → AI账单余额 → 插件选项 → 配置 API Key 和刷新频率
5. 右键 TrafficMonitor → 选项 → 任务栏窗口设置 → 显示设置：勾选需要显示的 AI提供商

## 文档
- [TrafficMonitor Wiki | 插件开发指南](https://github.com/zhongyang219/TrafficMonitor/wiki/%E6%8F%92%E4%BB%B6%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97)

## 许可证

MIT License
