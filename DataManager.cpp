#include "pch.h"
#include "DataManager.h"

const RefreshPreset g_refresh_presets[] = {
    { L"1 min",   60 },
    { L"3 min",   180 },
    { L"5 min",   300 },
    { L"10 min",  600 },
    { L"30 min",  1800 },
    { L"60 min",  3600 },
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
    GetPrivateProfileString(_T("config"), _T("deepseek_key"), _T(""), buf, _countof(buf), m_config_path.c_str());
    m_setting_data.deepseek_key = buf;
    GetPrivateProfileString(_T("config"), _T("siliconcloud_key"), _T(""), buf, _countof(buf), m_config_path.c_str());
    m_setting_data.siliconcloud_key = buf;
    m_setting_data.refresh_interval = GetPrivateProfileInt(_T("config"), _T("refresh_interval"), g_default_refresh, m_config_path.c_str());
}

void CDataManager::SaveConfig() const
{
    WritePrivateProfileString(_T("config"), _T("deepseek_key"), m_setting_data.deepseek_key.c_str(), m_config_path.c_str());
    WritePrivateProfileString(_T("config"), _T("siliconcloud_key"), m_setting_data.siliconcloud_key.c_str(), m_config_path.c_str());
    wchar_t buf[16];
    swprintf_s(buf, L"%d", m_setting_data.refresh_interval);
    WritePrivateProfileString(_T("config"), _T("refresh_interval"), buf, m_config_path.c_str());
}
