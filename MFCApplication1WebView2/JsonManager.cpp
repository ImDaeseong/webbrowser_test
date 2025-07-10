#include "pch.h"
#include "JsonManager.h"

CJsonManager::CJsonManager()
{
    m_json = Json::Value(Json::objectValue);
}

CJsonManager::~CJsonManager() 
{
    m_json.clear();
}

bool CJsonManager::LoadFromString(const CString& jsonString) 
{
    if (jsonString.IsEmpty()) {
        return false;
    }

    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string errors;

    std::string utf8String = CStringToUtf8(jsonString);
    return reader->parse(utf8String.c_str(), utf8String.c_str() + utf8String.length(), &m_json, &errors);
}

bool CJsonManager::RemoveKey(const CString& key)
{
    if (key.IsEmpty()) {
        return false;
    }

    m_json.removeMember(CStringToUtf8(key));
    return true;
}

CString CJsonManager::GetValueAsCString(const CString& key) const 
{
    if (key.IsEmpty()) {
        return CString();
    }

    if (m_json.isMember(CStringToUtf8(key)) && m_json[CStringToUtf8(key)].isString()) {
        return Utf8ToCString(m_json[CStringToUtf8(key)].asString());
    }
    return CString();
}

int CJsonManager::GetValueAsInt(const CString& key) const 
{
    if (key.IsEmpty()) {
        return 0;
    }

    if (m_json.isMember(CStringToUtf8(key)) && m_json[CStringToUtf8(key)].isInt()) {
        return m_json[CStringToUtf8(key)].asInt();
    }
    return 0;
}

double CJsonManager::GetValueAsDouble(const CString& key) const 
{
    if (key.IsEmpty()) {
        return 0.0;
    }

    if (m_json.isMember(CStringToUtf8(key)) && m_json[CStringToUtf8(key)].isDouble()) {
        return m_json[CStringToUtf8(key)].asDouble();
    }
    return 0.0;
}

bool CJsonManager::GetValueAsBool(const CString& key) const 
{
    if (key.IsEmpty()) {
        return false; 
    }

    if (m_json.isMember(CStringToUtf8(key)) && m_json[CStringToUtf8(key)].isBool()) {
        return m_json[CStringToUtf8(key)].asBool();
    }
    return false;
}

bool CJsonManager::SetJsonArray(const CString& key, const std::vector<Json::Value>& array)
{
    if (key.IsEmpty()) {
        return false;
    }

    Json::Value jsonArray(Json::arrayValue);
    for (const auto& item : array) {
        jsonArray.append(item);
    }
    m_json[CStringToUtf8(key)] = jsonArray;
    return true;
}

std::vector<Json::Value> CJsonManager::GetJsonArray(const CString& key) const 
{
    std::vector<Json::Value> result;
    if (key.IsEmpty()) {
        return result; 
    }

    if (m_json.isMember(CStringToUtf8(key)) && m_json[CStringToUtf8(key)].isArray()) {
        for (const auto& item : m_json[CStringToUtf8(key)]) {
            result.push_back(item);
        }
    }
    return result;
}

Json::Value CJsonManager::GetJsonObject(const CString& key) const
{
    if (key.IsEmpty()) {
        return Json::nullValue;
    }

    std::string utf8Key = CStringToUtf8(key);

    if (m_json.isMember(utf8Key) && m_json[utf8Key].isObject()) {
        return m_json[utf8Key];
    }

    return Json::nullValue;
}

CString CJsonManager::GetJsonString() const 
{
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, m_json);
    return Utf8ToCString(jsonString);
}

Json::Value CJsonManager::CreateJsonObject() 
{
    return Json::Value(Json::objectValue);
}

std::string CJsonManager::CStringToUtf8(const CString& str) 
{
    if (str.IsEmpty()) 
        return std::string();

    int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) {
        // 변환 실패
        return std::string();
    }         

    std::string utf8(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, &utf8[0], len, nullptr, nullptr);
    return utf8;
}

CString CJsonManager::Utf8ToCString(const std::string& str) 
{
    if (str.empty()) 
        return CString();

    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (len <= 0) {
        // 변환 실패
        return CString(); 
    }

    CString wstr;
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.GetBuffer(len), len);
    wstr.ReleaseBuffer();
    return wstr;
}

CString CJsonManager::ConvertUtf8ToCString(const std::string& str) {
    return Utf8ToCString(str); 
}
