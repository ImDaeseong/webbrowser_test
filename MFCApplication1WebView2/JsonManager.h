#pragma once

#include <afx.h>
#include <vector>
#include <string>
#include "./json/json.h"

class CJsonManager
{
public:
    CJsonManager();
    ~CJsonManager();

    bool LoadFromString(const CString& jsonString);
    bool RemoveKey(const CString& key);

    CString GetValueAsCString(const CString& key) const;
    int GetValueAsInt(const CString& key) const;
    double GetValueAsDouble(const CString& key) const;
    bool GetValueAsBool(const CString& key) const;

    template<typename T>
    bool SetValue(const CString& key, const T& value)
    {
        if (key.IsEmpty())
        {
            return false;
        }

        if constexpr (std::is_convertible_v<T, CString>)
        {
            CString cstrValue = value;  // 명시적 변환
            m_json[CStringToUtf8(key)] = CStringToUtf8(cstrValue);
        }
        else if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, bool>)
        {
            m_json[CStringToUtf8(key)] = value;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool SetJsonArray(const CString& key, const std::vector<Json::Value>& array);
    std::vector<Json::Value> GetJsonArray(const CString& key) const;
    Json::Value GetJsonObject(const CString& key) const;

    static Json::Value CreateJsonObject();
   
    template<typename T>
    static void AddToJsonObject(Json::Value& obj, const CString& key, const T& value)
    {
        if (key.IsEmpty())
        {
            return;
        }

        if constexpr (std::is_convertible_v<T, CString>)
        {
            CString cstrValue = value; 
            obj[CStringToUtf8(key)] = CStringToUtf8(cstrValue);
        }
        else if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, bool>)
        {
            obj[CStringToUtf8(key)] = value;
        }
        else
        {
            static_assert(always_false<T>, "Unsupported type for AddToJsonObject");
        }
    }

    static CString ConvertUtf8ToCString(const std::string& str);

    CString GetJsonString() const;

private:
    Json::Value m_json;

    static std::string CStringToUtf8(const CString& str);
    static CString Utf8ToCString(const std::string& str);

    template<typename T>
    static constexpr bool always_false = false;
};