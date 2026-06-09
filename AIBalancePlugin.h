#pragma once
#include "PluginInterface.h"
#include "AIBalanceItem.h"

class CAIBalancePlugin : public ITMPlugin
{
private:
    CAIBalancePlugin();

public:
    static CAIBalancePlugin& Instance();

    IPluginItem* GetItem(int index) override;
    void DataRequired() override;
    const wchar_t* GetInfo(PluginInfoIndex index) override;
    OptionReturn ShowOptionsDialog(void* hParent) override;
    void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;

private:
    void FetchBalance(CAIBalanceItem& item);

    CAIBalanceItem m_deepseek;
    CAIBalanceItem m_siliconcloud;
    CAIBalanceItem m_kimi;
    static CAIBalancePlugin m_instance;
    DWORD m_last_fetch_time{};
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();
#ifdef __cplusplus
}
#endif
