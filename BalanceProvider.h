#pragma once
#include <string>
#include <vector>

struct BalanceProvider
{
    std::wstring name;
    std::wstring host;
    std::wstring path;
    std::wstring balance_path;
    std::wstring id;
};

const std::vector<BalanceProvider>& GetPresetProviders();
