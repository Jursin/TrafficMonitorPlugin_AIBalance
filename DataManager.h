#pragma once
#include <string>
#include <vector>
#include <map>
#include <afx.h>

#define g_data CDataManager::Instance()

struct BalanceProvider
{
    std::wstring name;
    std::wstring host;
    std::wstring path;
    std::wstring balance_path;
    std::wstring id;
    std::wstring auth_prefix = L"Bearer ";
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
    std::map<std::wstring, std::wstring> api_keys;
    int refresh_interval{};
};

class CDataManager
{
private:
    CDataManager();
    ~CDataManager();

public:
    static CDataManager& Instance();
    void LoadConfig(const std::wstring& config_dir);
    void ReloadConfig();
    void SaveConfig() const;
    const CString& StringRes(UINT id);

public:
    SettingData m_setting_data;

private:
    static CDataManager m_instance;
    std::wstring m_config_path;
    std::map<UINT, CString> m_string_table;
};
