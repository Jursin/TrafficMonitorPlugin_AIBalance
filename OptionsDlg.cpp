#include "pch.h"
#include "AIBalancePlugin.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent)
	: CDialog(IDD_OPTIONS_DIALOG, pParent)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
    ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

static const struct { const wchar_t* id; UINT ctrl; } s_key_map[] = {
    { L"deepseek",     IDC_DEEPSEEK_KEY_EDIT },
    { L"kimi",         IDC_KIMI_KEY_EDIT },
    { L"zhipu",        IDC_ZHIPU_KEY_EDIT },
    { L"siliconcloud", IDC_SILICONCLOUD_KEY_EDIT },
};

BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    for (const auto& m : s_key_map)
    {
        auto it = m_data.api_keys.find(m.id);
        if (it != m_data.api_keys.end())
            SetDlgItemText(m.ctrl, it->second.c_str());
    }

    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_REFRESH_COMBO);
    int sel = -1;
    for (int i = 0; i < g_preset_count; i++)
    {
        pCombo->AddString(g_refresh_presets[i].label);
        if (g_refresh_presets[i].seconds == m_data.refresh_interval)
            sel = i;
    }
    if (sel >= 0)
        pCombo->SetCurSel(sel);
    else
    {
        wchar_t buf[16];
        swprintf_s(buf, L"%d", m_data.refresh_interval);
        pCombo->SetWindowText(buf);
    }

	return TRUE;
}

void COptionsDlg::OnBnClickedOk()
{
    CString str;
    for (const auto& m : s_key_map)
    {
        GetDlgItemText(m.ctrl, str);
        m_data.api_keys[m.id] = str.GetString();
    }

    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_REFRESH_COMBO);
    int sel = pCombo->GetCurSel();
    if (sel >= 0 && sel < g_preset_count)
        m_data.refresh_interval = g_refresh_presets[sel].seconds;
    else
    {
        CString text;
        pCombo->GetWindowText(text);
        int val = _wtoi(text);
        if (val > 0) m_data.refresh_interval = val;
    }

    CDialog::OnOK();
}
