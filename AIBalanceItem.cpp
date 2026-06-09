#include "pch.h"
#include "AIBalanceItem.h"

CAIBalanceItem::CAIBalanceItem(const BalanceProvider& provider)
    : m_provider(provider)
{
    m_value_text = L": N/A";
}

const wchar_t* CAIBalanceItem::GetItemName() const
{
    return m_provider.name.c_str();
}

const wchar_t* CAIBalanceItem::GetItemId() const
{
    return m_provider.id.c_str();
}

const wchar_t* CAIBalanceItem::GetItemLableText() const
{
    return m_provider.name.c_str();
}

const wchar_t* CAIBalanceItem::GetItemValueText() const
{
    return m_value_text.c_str();
}

const wchar_t* CAIBalanceItem::GetItemValueSampleText() const
{
    return L": \xFFE5" L"99999.99";
}

void CAIBalanceItem::SetBalance(double balance)
{
    wchar_t buff[64];
    swprintf_s(buff, L": \xFFE5%.2f", balance);
    m_value_text = buff;
}

void CAIBalanceItem::SetBalanceFailed()
{
    m_value_text = L": N/A";
}
