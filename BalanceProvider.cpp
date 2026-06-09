#include "pch.h"
#include "BalanceProvider.h"

const std::vector<BalanceProvider>& GetPresetProviders()
{
    static const std::vector<BalanceProvider> presets = {
        { L"DeepSeek", L"api.deepseek.com", L"/user/balance",
          L"balance_infos.0.total_balance", L"deepseek" },
        { L"SiliconCloud", L"api.siliconflow.cn", L"/v1/user/info",
          L"data.balance", L"siliconcloud" },
    };
    return presets;
}
