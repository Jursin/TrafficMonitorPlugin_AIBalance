#include "pch.h"
#include "AIBalancePlugin.h"
#include "DataManager.h"
#include "HttpHelper.h"
#include "OptionsDlg.h"

CAIBalancePlugin CAIBalancePlugin::m_instance;

CAIBalancePlugin::CAIBalancePlugin()
{
    for (const auto& provider : GetPresetProviders())
        m_items.emplace_back(provider);
}

CAIBalancePlugin& CAIBalancePlugin::Instance()
{
    return m_instance;
}

IPluginItem* CAIBalancePlugin::GetItem(int index)
{
    if (index >= 0 && index < (int)m_items.size())
        return &m_items[index];
    return nullptr;
}

const wchar_t* CAIBalancePlugin::GetTooltipInfo()
{
    m_tooltip_info.clear();
    for (const auto& item : m_items)
    {
        if (!m_tooltip_info.empty())
            m_tooltip_info += L"\n";
        m_tooltip_info += item.GetItemName();
        m_tooltip_info += item.GetItemValueText();
    }
    return m_tooltip_info.c_str();
}

void CAIBalancePlugin::DataRequired()
{
    ULONGLONG now = GetTickCount64();
    ULONGLONG interval_ms = (ULONGLONG)g_data.m_setting_data.refresh_interval * 1000;
    if (m_last_fetch_time != 0 && (now - m_last_fetch_time) < interval_ms)
        return;
    m_last_fetch_time = now;

    for (auto& item : m_items)
    {
        const auto& provider = item.GetProvider();
        auto it = g_data.m_setting_data.api_keys.find(provider.id);
        if (it != g_data.m_setting_data.api_keys.end() && !it->second.empty())
            FetchBalance(item);
        else
            item.SetBalanceFailed();
    }
}

void CAIBalancePlugin::FetchBalance(CAIBalanceItem& item)
{
    const auto& provider = item.GetProvider();
    auto it = g_data.m_setting_data.api_keys.find(provider.id);
    if (it == g_data.m_setting_data.api_keys.end() || it->second.empty())
    {
        item.SetBalanceFailed();
        return;
    }

    std::wstring header;
    if (provider.auth_prefix == L"Cookie ")
        header = L"Cookie: " + it->second;
    else
        header = L"Authorization: Bearer " + it->second;
    std::string response = HttpGet(provider.host, provider.path, header);
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
    static CString str;
    switch (index)
    {
    case TMI_NAME:        return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
    case TMI_DESCRIPTION: return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
    case TMI_AUTHOR:      return L"Jursin";
    case TMI_COPYRIGHT:   return L"Copyright (C) 2026";
    case TMI_VERSION:     return L"1.1";
    case TMI_URL:         return L"https://github.com/Jursin/TrafficMonitorPlugin_AIBalance";
    default: return L"";
    }
}

ITMPlugin::OptionReturn CAIBalancePlugin::ShowOptionsDialog(void* hParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    g_data.ReloadConfig();
    CWnd* pParent = CWnd::FromHandle((HWND)hParent);
    COptionsDlg dlg(pParent);
    dlg.m_data = g_data.m_setting_data;
    if (dlg.DoModal() == IDOK)
    {
        g_data.m_setting_data = dlg.m_data;
        g_data.SaveConfig();
        m_last_fetch_time = 0;
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
