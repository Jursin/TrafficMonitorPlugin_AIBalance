#pragma once
#include <string>
#include <vector>

#define g_data CDataManager::Instance()

struct BalanceProvider
{
    std::wstring name;
    std::wstring host;
    std::wstring path;
    std::wstring balance_path;
    std::wstring id;
};

const std::vector<BalanceProvider>& GetPresetProviders();

struct RefreshPreset
{
    const wchar_t* label;
    int seconds;
};

extern const RefreshPreset g_refresh_presets[];
extern const int g_preset_count;
extern const int g_default_refresh;

struct SettingData
{
    std::wstring deepseek_key;
    std::wstring siliconcloud_key;
    std::wstring kimi_key;
    int refresh_interval;
};

class CDataManager
{
private:
    CDataManager();
    ~CDataManager();

public:
    static CDataManager& Instance();
    void LoadConfig(const std::wstring& config_dir);
    void SaveConfig() const;

public:
    SettingData m_setting_data;

private:
    static CDataManager m_instance;
    std::wstring m_config_path;
};
