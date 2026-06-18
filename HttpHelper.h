#pragma once
#include <string>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

inline std::string HttpGet(const std::wstring& host, const std::wstring& path, const std::wstring& header)
{
    std::string result;
    HINTERNET hSession = WinHttpOpen(L"AIBalance/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, nullptr, nullptr, 0);
    if (!hSession) return result;

    HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return result; }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(),
        nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return result; }

    if (!header.empty())
    {
        WinHttpAddRequestHeaders(hRequest, header.c_str(), (DWORD)-1L, WINHTTP_ADDREQ_FLAG_ADD);
    }

    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)
        && WinHttpReceiveResponse(hRequest, nullptr))
    {
        DWORD bytes_available{};
        while (WinHttpQueryDataAvailable(hRequest, &bytes_available) && bytes_available > 0)
        {
            char buf[4096]{};
            DWORD bytes_read{};
            WinHttpReadData(hRequest, buf, min(bytes_available, sizeof(buf) - 1), &bytes_read);
            result.append(buf, bytes_read);
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return result;
}

// 从 JSON 中按点分隔路径提取数值，支持数组索引（如 "data.0.value"）
inline bool ExtractJsonDouble(const std::string& json, const std::string& path, double& out)
{
    std::string cur = json;
    std::string rem = path;

    while (!rem.empty())
    {
        std::string key;
        size_t dot = rem.find('.');
        if (dot != std::string::npos) { key = rem.substr(0, dot); rem = rem.substr(dot + 1); }
        else { key = rem; rem.clear(); }

        bool is_index = true;
        for (char c : key) { if (!isdigit((unsigned char)c)) { is_index = false; break; } }

        if (is_index)
        {
            int index = std::stoi(key);
            size_t bracket = cur.find('[');
            if (bracket == std::string::npos) return false;
            cur = cur.substr(bracket + 1);

            for (int i = 0; i < index; i++)
            {
                int depth = 0; bool in_str = false, started = false;
                size_t j = 0;
                for (; j < cur.size(); j++)
                {
                    char c = cur[j];
                    if (c == '"' && (j == 0 || cur[j - 1] != '\\')) in_str = !in_str;
                    if (in_str) continue;
                    if (c == '{' || c == '[') { depth++; started = true; }
                    else if (c == '}' || c == ']') depth--;
                    if (started && depth <= 0) break;
                }
                j++;
                while (j < cur.size() && (cur[j] == ' ' || cur[j] == ',' || cur[j] == '\r' || cur[j] == '\n' || cur[j] == '\t'))
                    j++;
                cur = cur.substr(j);
            }
        }
        else
        {
            std::string search = "\"" + key + "\"";
            size_t pos = cur.find(search);
            if (pos == std::string::npos) return false;
            size_t colon = cur.find(':', pos + search.size());
            if (colon == std::string::npos) return false;
            cur = cur.substr(colon + 1);
        }
    }

    size_t start = cur.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return false;

    if (cur[start] == '"')
    {
        size_t end = cur.find('"', start + 1);
        if (end == std::string::npos) return false;
        out = std::stod(cur.substr(start + 1, end - start - 1));
        return true;
    }

    size_t end = start;
    while (end < cur.size() && (isdigit((unsigned char)cur[end]) || cur[end] == '.' || cur[end] == '-' || cur[end] == '+' || cur[end] == 'e' || cur[end] == 'E'))
        end++;
    if (end == start) return false;
    out = std::stod(cur.substr(start, end - start));
    return true;
}
