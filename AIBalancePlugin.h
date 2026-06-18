#pragma once
#include "PluginInterface.h"
#include "AIBalanceItem.h"
#include <string>

class CAIBalancePlugin : public ITMPlugin
{
private:
    CAIBalancePlugin();

public:
    static CAIBalancePlugin& Instance();

    IPluginItem* GetItem(int index) override;
    void DataRequired() override;
    const wchar_t* GetTooltipInfo() override;
    const wchar_t* GetInfo(PluginInfoIndex index) override;
    OptionReturn ShowOptionsDialog(void* hParent) override;
    void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;

private:
    void FetchBalance(CAIBalanceItem& item);

    std::vector<CAIBalanceItem> m_items;
    static CAIBalancePlugin m_instance;
    ULONGLONG m_last_fetch_time{};
    std::wstring m_tooltip_info;
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();
#ifdef __cplusplus
}
#endif
