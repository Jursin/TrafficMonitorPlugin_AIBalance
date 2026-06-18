# AIBalance - TrafficMonitor Plugin

> [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) 插件，用于在任务栏显示 AI 提供商账户余额。

## 支持的 AI 提供商

- [DeepSeek](https://platform.deepseek.com/api_keys)
- [Xiaomi MiMo](https://platform.xiaomimimo.com/console/api-keys)
- [Kimi](https://platform.kimi.com/console/api-keys)
- [智谱](https://bigmodel.cn/apikey/platform)
- [硅基流动](https://cloud.siliconflow.cn/me/account/ak)

> [!note]
> 调用 Xiaomi MiMo 接口需要自行提供 Cookie，且只有 1 天时效

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

## 注意
- 插件本身只是通过请求**官方 API 接口**获取余额数据并显示
- 插件不会收集你的敏感信息，所有配置保存在本地
- 插件为 **AI 辅助编写**，如介意请不要使用

## 文档
- [TrafficMonitor Wiki | 插件开发指南](https://github.com/zhongyang219/TrafficMonitor/wiki/%E6%8F%92%E4%BB%B6%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97)

## 许可证

MIT License
