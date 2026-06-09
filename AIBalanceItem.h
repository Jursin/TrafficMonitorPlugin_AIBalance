#pragma once
#include "PluginInterface.h"
#include "DataManager.h"

class CAIBalanceItem : public IPluginItem
{
public:
    CAIBalanceItem(const BalanceProvider& provider);

    const wchar_t* GetItemName() const override;
    const wchar_t* GetItemId() const override;
    const wchar_t* GetItemLableText() const override;
    const wchar_t* GetItemValueText() const override;
    const wchar_t* GetItemValueSampleText() const override;

    void SetBalance(double balance);
    void SetBalanceFailed();
    const BalanceProvider& GetProvider() const { return m_provider; }

private:
    BalanceProvider m_provider;
    std::wstring m_value_text;
};
