#include "pch.h"
#include "AIBalancePlugin.h"
#include "DataManager.h"
#include "HttpHelper.h"
#include "OptionsDlg.h"

CAIBalancePlugin CAIBalancePlugin::m_instance;

CAIBalancePlugin::CAIBalancePlugin()
    : m_deepseek(GetPresetProviders()[0])
    , m_siliconcloud(GetPresetProviders()[1])
{
}

CAIBalancePlugin& CAIBalancePlugin::Instance()
{
    return m_instance;
}

IPluginItem* CAIBalancePlugin::GetItem(int index)
{
    switch (index)
    {
    case 0: return &m_deepseek;
    case 1: return &m_siliconcloud;
    default: return nullptr;
    }
}

void CAIBalancePlugin::DataRequired()
{
    DWORD now = GetTickCount();
    DWORD interval_ms = (DWORD)g_data.m_setting_data.refresh_interval * 1000;
    if (m_last_fetch_time != 0 && (now - m_last_fetch_time) < interval_ms)
        return;
    m_last_fetch_time = now;

    if (!g_data.m_setting_data.deepseek_key.empty())
        FetchBalance(m_deepseek);
    else
        m_deepseek.SetBalanceFailed();

    if (!g_data.m_setting_data.siliconcloud_key.empty())
        FetchBalance(m_siliconcloud);
    else
        m_siliconcloud.SetBalanceFailed();
}

void CAIBalancePlugin::FetchBalance(CAIBalanceItem& item)
{
    const auto& provider = item.GetProvider();
    const std::wstring* key = nullptr;
    if (provider.id == L"deepseek")
        key = &g_data.m_setting_data.deepseek_key;
    else if (provider.id == L"siliconcloud")
        key = &g_data.m_setting_data.siliconcloud_key;

    if (!key || key->empty())
    {
        item.SetBalanceFailed();
        return;
    }

    std::wstring auth = L"Bearer " + *key;
    std::string response = HttpGet(provider.host, provider.path, auth);
    if (response.empty())
    {
        item.SetBalanceFailed();
        return;
    }

    std::string path_a;
    for (wchar_t wc : provider.balance_path)
        path_a += (char)wc;
    double balance{};
    if (ExtractJsonDouble(response, path_a, balance))
        item.SetBalance(balance);
    else
        item.SetBalanceFailed();
}

const wchar_t* CAIBalancePlugin::GetInfo(PluginInfoIndex index)
{
    switch (index)
    {
    case TMI_NAME:        return L"AI账单余额";
    case TMI_DESCRIPTION: return L"显示 AI 提供商账单余额";
    case TMI_AUTHOR:      return L"Jursin";
    case TMI_COPYRIGHT:   return L"Copyright (C) 2026";
    case TMI_VERSION:     return L"1.0";
    case TMI_URL:         return L"https://github.com/Jursin/TrafficMonitorPlugin_AIBalance";
    default: return L"";
    }
}

ITMPlugin::OptionReturn CAIBalancePlugin::ShowOptionsDialog(void* hParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    COptionsDlg dlg(CWnd::FromHandle((HWND)hParent));
    dlg.m_data = g_data.m_setting_data;
    if (dlg.DoModal() == IDOK)
    {
        g_data.m_setting_data = dlg.m_data;
        m_last_fetch_time = 0;
        DataRequired();
        return ITMPlugin::OR_OPTION_CHANGED;
    }
    return ITMPlugin::OR_OPTION_UNCHANGED;
}

void CAIBalancePlugin::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
    if (index == ITMPlugin::EI_CONFIG_DIR)
        g_data.LoadConfig(std::wstring(data));
}

ITMPlugin* TMPluginGetInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return &CAIBalancePlugin::Instance();
}
