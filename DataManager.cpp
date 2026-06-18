#include "pch.h"
#include "DataManager.h"
#include "resource.h"

const std::vector<BalanceProvider>& GetPresetProviders()
{
    static const std::vector<BalanceProvider> presets = {
        { L"DeepSeek", L"api.deepseek.com", L"/user/balance",
          L"balance_infos.0.total_balance", L"deepseek" },
        { L"Xiaomi MiMo", L"platform.xiaomimimo.com", L"/api/v1/balance",
          L"data.balance", L"xiaomimimo", L"Cookie " },
        { L"Kimi", L"api.moonshot.cn", L"/v1/users/me/balance",
          L"data.available_balance", L"kimi" },
        { L"智谱", L"www.bigmodel.cn", L"/api/biz/account/query-customer-account-report",
          L"data.availableBalance", L"zhipu" },
        { L"硅基流动", L"api.siliconflow.cn", L"/v1/user/info",
          L"data.totalBalance", L"siliconcloud" },
    };
    return presets;
}

const RefreshPreset g_refresh_presets[] = {
    { L"5 min",   300 },
    { L"10 min",  600 },
    { L"30 min",  1800 },
};
const int g_preset_count = _countof(g_refresh_presets);
const int g_default_refresh = 300;

CDataManager CDataManager::m_instance;

CDataManager::CDataManager()
{
    m_setting_data.refresh_interval = g_default_refresh;
}

CDataManager::~CDataManager()
{
    SaveConfig();
}

const CString& CDataManager::StringRes(UINT id)
{
    auto iter = m_string_table.find(id);
    if (iter != m_string_table.end())
    {
        return iter->second;
    }
    else
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        CString str;
        (void)str.LoadString(id);
        m_string_table[id] = str;
        return m_string_table[id];
    }
}

CDataManager& CDataManager::Instance()
{
    return m_instance;
}

void CDataManager::LoadConfig(const std::wstring& config_dir)
{
    HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(hModule, path, MAX_PATH);
    std::wstring module_path = path;
    m_config_path = module_path;
    if (!config_dir.empty())
    {
        size_t pos = module_path.find_last_of(L"\\/");
        m_config_path = config_dir + module_path.substr(pos + 1);
    }
    m_config_path += L".ini";

    wchar_t buf[512];
    for (const auto& provider : GetPresetProviders())
    {
        std::wstring config_key = provider.id + L"_key";
        GetPrivateProfileString(_T("config"), config_key.c_str(), _T(""), buf, _countof(buf), m_config_path.c_str());
        m_setting_data.api_keys[provider.id] = buf;
    }
    m_setting_data.refresh_interval = GetPrivateProfileInt(_T("config"), _T("refresh_interval"), g_default_refresh, m_config_path.c_str());
}

void CDataManager::ReloadConfig()
{
    if (m_config_path.empty()) return;

    wchar_t buf[512];
    for (const auto& provider : GetPresetProviders())
    {
        std::wstring config_key = provider.id + L"_key";
        GetPrivateProfileString(_T("config"), config_key.c_str(), _T(""), buf, _countof(buf), m_config_path.c_str());
        m_setting_data.api_keys[provider.id] = buf;
    }
    m_setting_data.refresh_interval = GetPrivateProfileInt(_T("config"), _T("refresh_interval"), g_default_refresh, m_config_path.c_str());
}

void CDataManager::SaveConfig() const
{
    for (const auto& provider : GetPresetProviders())
    {
        std::wstring config_key = provider.id + L"_key";
        auto it = m_setting_data.api_keys.find(provider.id);
        const wchar_t* value = (it != m_setting_data.api_keys.end()) ? it->second.c_str() : L"";
        WritePrivateProfileString(_T("config"), config_key.c_str(), value, m_config_path.c_str());
    }
    wchar_t buf[16];
    swprintf_s(buf, L"%d", m_setting_data.refresh_interval);
    WritePrivateProfileString(_T("config"), _T("refresh_interval"), buf, m_config_path.c_str());
}
